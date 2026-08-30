#!/usr/bin/env python3
"""Focused tests for deterministic ranked ready-queue selection."""

from __future__ import annotations

import json
import contextlib
import io
from pathlib import Path
import sys
import time
import unittest
from unittest import mock


sys.path.insert(0, str(Path(__file__).resolve().parent))
import lane_status  # noqa: E402
import nm_ranking  # noqa: E402
import ready_queue as rq  # noqa: E402


def row(file_name: str, symbol: str, differing: int) -> dict[str, object]:
    return {
        "name": symbol,
        "file": file_name,
        "overlay": None,
        "tu": "main",
        "size_bytes": 40,
        "objdiff_match_pct": None,
        "differing_words": differing,
        "first_mismatch_offset": 0,
        "size_delta": 0,
        "category": "register-only",
    }


def document(rows: list[dict[str, object]]) -> dict[str, object]:
    return {
        "queue_size": len(rows),
        "resolved": len(rows),
        "unresolved": 0,
        "objdiff_report_used": False,
        "objdiff_match_pct_coverage": 0,
        "functions": rows,
        "unresolved_functions": [],
    }


class Item:
    def __init__(self, file_name: str, symbol: str):
        self.rel_c_file = file_name
        self.func = symbol


def assignment(
    symbol: str,
    file_name: str | None,
    state: str = "base-only",
    *,
    active_lanes: list[str] | None = None,
    reason_code: str | None = None,
) -> lane_status.Assignment:
    if reason_code is None:
        reason_code = {
            "base-only": "ready",
            "active": "lane-owned",
            "already-integrated/exhausted": "current-plateau",
            "stale-ledger": "unclassified",
        }.get(state)
    return lane_status.Assignment(
        symbol=symbol,
        state=state,
        source_path=file_name,
        source_commit="source" if state != "base-only" else None,
        ledger_commit="ledger" if state == "already-integrated/exhausted" else None,
        active_lanes=active_lanes or [],
        reason=f"{state} reason",
        reason_code=reason_code,
    )


class ReadyQueueTests(unittest.TestCase):
    def report(
        self, rows, items, states, *, scan=10, top=10, selection="default",
        freshness=None, collect_maintenance=False,
    ):
        def classify(_base: str, symbol: str) -> lane_status.Assignment:
            return states[symbol]

        return rq.build_report(
            document(rows), items, base="base", base_commit="abc",
            ranking_name="ranking.json", scan=scan, top=top, classify=classify,
            selection=selection, freshness=freshness,
            collect_maintenance=collect_maintenance,
        )

    def test_preserves_rank_while_skipping_every_fail_closed_state(self) -> None:
        rows = [
            row("src/main/a.c", "a", 1),
            row("src/main/b.c", "b", 2),
            row("src/main/c.c", "c", 3),
            row("src/main/d.c", "d", 4),
            row("src/main/e.c", "e", 5),
        ]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            "a": assignment("a", "src/main/a.c", "active", active_lanes=["lane/a"]),
            "b": assignment("b", "src/main/b.c", "already-integrated/exhausted"),
            "c": assignment(
                "c", "src/main/c.c", "stale-ledger",
                reason_code="prose-needs-remeasurement",
            ),
            "d": assignment("d", "src/main/d.c"),
            "e": assignment("e", "src/main/e.c"),
        }
        report = self.report(rows, items, states)
        self.assertEqual([value["symbol"] for value in report["ready"]], ["d", "e"])
        self.assertEqual([value["rank"] for value in report["ready"]], [4, 5])
        self.assertEqual(
            report["summary"]["skipped"],
            {
                "dirty-worktree": 0,
                "active": 1,
                "already-integrated/exhausted": 1,
                "stale-ledger": 1,
                "not-live": 0,
            },
        )
        self.assertEqual(report["skipped"][0]["active_lanes"], ["lane/a"])
        maintenance = rq.render_maintenance(report)
        self.assertIn("lane ref(s)", maintenance)
        self.assertIn("stale-ledger", maintenance)
        self.assertIn("prose-needs-remeasurement", maintenance)
        self.assertEqual(
            report["summary"]["maintenance_classes"],
            {
                "current-plateau": 1,
                "lane-owned": 1,
                "prose-needs-remeasurement": 1,
            },
        )
        self.assertNotIn("already-integrated/exhausted  b", maintenance)

    def test_scan_and_top_are_hard_bounds(self) -> None:
        rows = [row(f"src/main/{name}.c", name, index) for index, name in enumerate("abcd", 1)]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            "a": assignment("a", "src/main/a.c", "active"),
            "b": assignment("b", "src/main/b.c"),
            "c": assignment("c", "src/main/c.c"),
            "d": assignment("d", "src/main/d.c"),
        }
        scan_limited = self.report(rows, items, states, scan=2, top=2)
        self.assertEqual(scan_limited["scanned"], 2)
        self.assertEqual([value["symbol"] for value in scan_limited["ready"]], ["b"])
        self.assertTrue(scan_limited["summary"]["scan_limit_reached"])

        top_limited = self.report(rows, items, states, scan=4, top=1)
        self.assertEqual(top_limited["scanned"], 2)
        self.assertEqual([value["symbol"] for value in top_limited["ready"]], ["b"])
        self.assertTrue(top_limited["summary"]["top_limit_reached"])

    def test_parallel_classification_preserves_ranking_order(self) -> None:
        rows = [
            row(f"src/main/{name}.c", name, index)
            for index, name in enumerate("abcd", 1)
        ]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            name: assignment(name, f"src/main/{name}.c")
            for name in "abcd"
        }

        def classify(_base: str, symbol: str) -> lane_status.Assignment:
            time.sleep({"a": 0.04, "b": 0.03, "c": 0.02, "d": 0.01}[symbol])
            return states[symbol]

        report = rq.build_report(
            document(rows), items, base="base", base_commit="abc",
            ranking_name="ranking.json", scan=4, top=4, jobs=4,
            classify=classify,
        )
        self.assertEqual(
            [value["symbol"] for value in report["ready"]],
            ["a", "b", "c", "d"],
        )

    def test_same_tu_rows_expose_reusable_worker_batch(self) -> None:
        rows = [
            row("src/main/shared.c", "a", 1),
            row("src/main/other.c", "b", 2),
            row("src/main/shared.c", "c", 3),
        ]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            str(value["name"]): assignment(
                str(value["name"]), str(value["file"])
            )
            for value in rows
        }
        report = self.report(rows, items, states)
        self.assertEqual(
            [
                (
                    value["symbol"], value["file_batch_position"],
                    value["file_batch_size"],
                )
                for value in report["ready"]
            ],
            [("a", 1, 2), ("b", 1, 1), ("c", 2, 2)],
        )
        self.assertIn("TU batch", rq.render_table(report))

    def test_quality_ranking_can_promote_small_other_mismatch(self) -> None:
        allocator = row("src/main/a.c", "a", 30)
        other = row("src/main/b.c", "b", 1)
        other["category"] = "other"
        rows = [allocator, other]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            "a": assignment("a", "src/main/a.c"),
            "b": assignment("b", "src/main/b.c"),
        }
        report = self.report(rows, items, states)
        self.assertEqual([value["symbol"] for value in report["ready"]], ["b", "a"])
        self.assertEqual(report["ready"][0]["snapshot_rank"], 2)
        self.assertEqual(report["ready"][0]["proof_quality"], "codegen")

    def test_default_selection_retains_legacy_effort_order(self) -> None:
        rows = [
            row("src/main/a.c", "a", 8),
            dict(row("src/main/b.c", "b", 1), category="other"),
            dict(row("src/main/c.c", "c", 2), size_delta=8),
        ]
        freshness = {
            ("src/main/a.c", "a"): rq.RankingEvidence("a", False),
            ("src/main/b.c", "b"): rq.RankingEvidence("b", True),
            ("src/main/c.c", "c"): rq.RankingEvidence("c", True),
        }
        expected = sorted(
            enumerate(rows, 1),
            key=lambda value: (
                rq.effort_score(value[1])
                + (
                    rq.STALE_EVIDENCE_PENALTY
                    if not freshness[(value[1]["file"], value[1]["name"])].fresh
                    else 0
                ),
                value[0],
            ),
        )
        actual = rq.prioritized_rows(
            rows, freshness, selection="default",
        )
        self.assertEqual(
            [value[2]["name"] for value in actual],
            [value[1]["name"] for value in expected],
        )

    def test_relocation_mismatch_is_prioritized_as_a_low_cost_mechanism(self) -> None:
        relocation = row("src/main/reloc.c", "reloc", 3)
        relocation["category"] = "reloc-mismatch"
        other = row("src/main/other.c", "other", 3)
        other["category"] = "other"
        ranked = rq.prioritized_rows([other, relocation])
        self.assertEqual([value[2]["name"] for value in ranked], ["reloc", "other"])

    def test_masked_residual_drives_effort_and_raw_evidence_is_preserved(self) -> None:
        relocation_heavy = row("src/main/a.c", "a", 20)
        relocation_heavy["relocation_masked_differing_words"] = 1
        relocation_heavy["relocation_masked_first_mismatch_offset"] = 8
        plain = row("src/main/b.c", "b", 4)
        plain["relocation_masked_differing_words"] = 4
        plain["relocation_masked_first_mismatch_offset"] = 0
        ranked = rq.prioritized_rows([plain, relocation_heavy])
        self.assertEqual(
            [value[2]["name"] for value in ranked], ["a", "b"],
        )
        report = self.report(
            [relocation_heavy], [Item("src/main/a.c", "a")],
            {"a": assignment("a", "src/main/a.c")},
        )
        self.assertEqual(report["ready"][0]["differing_words"], 20)
        self.assertEqual(
            report["ready"][0]["relocation_masked_differing_words"], 1,
        )
        self.assertIn("1/20/10", rq.render_table(report))

    def test_expected_yield_uses_all_six_retrospective_factors(self) -> None:
        baseline = row("src/main/a.c", "a", 4)
        baseline.update({
            "size_bytes": 256,
            "relocation_masked_differing_words": 4,
            "relocation_masked_first_mismatch_offset": 0,
        })
        fresh = rq.RankingEvidence("fresh", True)
        baseline_score = rq.expected_yield_evidence(baseline, fresh).score

        non_exact = dict(baseline, size_delta=4)
        high_residual = dict(
            baseline, differing_words=40,
            relocation_masked_differing_words=40,
        )
        broad_category = dict(baseline, category="other")
        larger = dict(
            baseline, size_bytes=1024, differing_words=16,
            relocation_masked_differing_words=16,
        )
        raw_only = dict(baseline)
        raw_only["relocation_masked_differing_words"] = None
        raw_only["relocation_masked_first_mismatch_offset"] = None

        self.assertGreater(
            baseline_score, rq.expected_yield_evidence(non_exact, fresh).score,
        )
        self.assertGreater(
            baseline_score, rq.expected_yield_evidence(high_residual, fresh).score,
        )
        self.assertGreater(
            baseline_score, rq.expected_yield_evidence(broad_category, fresh).score,
        )
        self.assertGreater(
            baseline_score, rq.expected_yield_evidence(larger, fresh).score,
        )
        self.assertGreater(
            baseline_score,
            rq.expected_yield_evidence(
                baseline, rq.RankingEvidence("stale", False),
            ).score,
        )
        self.assertGreater(
            baseline_score, rq.expected_yield_evidence(raw_only, fresh).score,
        )

    def test_expected_yield_orders_fail_closed_tiers_before_score(self) -> None:
        high = row("src/main/high.c", "high", 3)
        high["size_bytes"] = 256
        high["relocation_masked_differing_words"] = 3
        broad = row("src/main/broad.c", "broad", 0)
        broad["category"] = "other"
        broad["relocation_masked_differing_words"] = 0
        broad["first_mismatch_offset"] = None
        broad["relocation_masked_first_mismatch_offset"] = None
        stale = row("src/main/stale.c", "stale", 0)
        stale["relocation_masked_differing_words"] = 0
        stale["first_mismatch_offset"] = None
        stale["relocation_masked_first_mismatch_offset"] = None
        freshness = {
            ("src/main/high.c", "high"): rq.RankingEvidence("h", True),
            ("src/main/broad.c", "broad"): rq.RankingEvidence("b", True),
            ("src/main/stale.c", "stale"): rq.RankingEvidence("s", False),
        }
        ranked = rq.prioritized_rows(
            [stale, broad, high], freshness, selection="expected-yield",
        )
        self.assertEqual(
            [value[2]["name"] for value in ranked],
            ["high", "broad", "stale"],
        )

    def test_high_confidence_selection_requires_every_proof(self) -> None:
        def candidate(name: str) -> dict[str, object]:
            value = row(f"src/main/{name}.c", name, 20)
            value["size_bytes"] = 256
            value["relocation_masked_differing_words"] = 20
            value["relocation_masked_first_mismatch_offset"] = 0
            return value

        good = candidate("good")
        stale = candidate("stale")
        unknown = candidate("unknown")
        non_exact = candidate("non_exact")
        non_exact["size_delta"] = 4
        raw_only = candidate("raw_only")
        raw_only["relocation_masked_differing_words"] = None
        raw_only["relocation_masked_first_mismatch_offset"] = None
        broad = candidate("broad")
        broad["category"] = "other"
        high_residual = candidate("high_residual")
        high_residual["differing_words"] = 24
        high_residual["relocation_masked_differing_words"] = 24
        high_residual["size_bytes"] = 64
        rows = [good, stale, unknown, non_exact, raw_only, broad, high_residual]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            str(value["name"]): assignment(
                str(value["name"]), str(value["file"]),
            )
            for value in rows
        }
        freshness = {
            (str(value["file"]), str(value["name"])): rq.RankingEvidence(
                str(value["name"]), value is not stale,
            )
            for value in rows if value is not unknown
        }
        report = self.report(
            rows, items, states, selection="high-confidence",
            freshness=freshness,
        )
        self.assertEqual([value["symbol"] for value in report["ready"]], ["good"])
        self.assertEqual(report["selected_ranking_rows"], 1)
        self.assertEqual(report["ready"][0]["selection_class"], "high-confidence")

    def test_broad_structural_selection_excludes_high_confidence_and_reproof(self) -> None:
        high = row("src/main/high.c", "high", 1)
        high["relocation_masked_differing_words"] = 1
        broad = row("src/main/broad.c", "broad", 1)
        broad["category"] = "other"
        broad["relocation_masked_differing_words"] = 1
        stale = row("src/main/stale.c", "stale", 1)
        stale["relocation_masked_differing_words"] = 1
        freshness = {
            ("src/main/high.c", "high"): rq.RankingEvidence("h", True),
            ("src/main/broad.c", "broad"): rq.RankingEvidence("b", True),
            ("src/main/stale.c", "stale"): rq.RankingEvidence("s", False),
        }
        rows = [high, broad, stale]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            str(value["name"]): assignment(str(value["name"]), str(value["file"]))
            for value in rows
        }
        report = self.report(
            rows, items, states, selection="broad-structural",
            freshness=freshness,
        )
        self.assertEqual([value["symbol"] for value in report["ready"]], ["broad"])

    def test_maintenance_scan_finds_reproof_after_top_and_prose_rows(self) -> None:
        rows = [
            row("src/main/a.c", "a", 1),
            row("src/main/b.c", "b", 2),
            row("src/main/c.c", "c", 3),
        ]
        items = [Item(str(value["file"]), str(value["name"])) for value in rows]
        states = {
            "a": assignment("a", "src/main/a.c"),
            "b": assignment("b", "src/main/b.c"),
            "c": assignment(
                "c", "src/main/c.c", "stale-ledger",
                reason_code="prose-needs-remeasurement",
            ),
        }
        freshness = {
            ("src/main/a.c", "a"): rq.RankingEvidence("a", True),
            ("src/main/b.c", "b"): rq.RankingEvidence("b", False),
            ("src/main/c.c", "c"): rq.RankingEvidence("c", True),
        }
        report = self.report(
            rows, items, states, top=1, freshness=freshness,
            collect_maintenance=True,
        )
        self.assertEqual(report["scanned"], 3)
        self.assertEqual([value["symbol"] for value in report["ready"]], ["a"])
        self.assertEqual(
            {
                value["maintenance_class"]
                for value in report["maintenance"]
            },
            {"ranking-reproof", "prose-needs-remeasurement"},
        )
        maintenance = rq.render_maintenance(report)
        self.assertIn("--refresh-stale --jobs 2", maintenance)
        self.assertIn("tools/finalize_plateau.py", maintenance)
        self.assertTrue(report["summary"]["maintenance_scan_complete"])

        bounded = self.report(
            rows, items, states, top=1, freshness=freshness,
            collect_maintenance=False,
        )
        self.assertEqual(bounded["scanned"], 1)
        self.assertEqual(bounded["maintenance"], [])

    def test_retained_data_focus_filters_to_relocation_rows(self) -> None:
        relocation = row("src/main/reloc.c", "reloc", 3)
        relocation["category"] = "reloc-mismatch"
        allocator = row("src/main/alloc.c", "alloc", 1)
        items = [Item("src/main/reloc.c", "reloc"), Item("src/main/alloc.c", "alloc")]
        states = {
            "reloc": assignment("reloc", "src/main/reloc.c"),
            "alloc": assignment("alloc", "src/main/alloc.c"),
        }

        def classify(_base: str, symbol: str) -> lane_status.Assignment:
            return states[symbol]

        report = rq.build_report(
            document([allocator, relocation]), items, base="base",
            base_commit="abc", ranking_name="ranking.json", scan=10, top=10,
            focus="retained-data", classify=classify,
        )
        self.assertEqual([value["symbol"] for value in report["ready"]], ["reloc"])
        self.assertEqual(report["focus"], "retained-data")
        self.assertEqual(report["focused_ranking_rows"], 1)

    def test_stale_ranking_is_assignable_only_as_reproof(self) -> None:
        rows = [row("src/main/a.c", "a", 1)]
        called = False

        def classify(_base: str, _symbol: str) -> lane_status.Assignment:
            nonlocal called
            called = True
            return assignment("a", "src/main/a.c")

        report = rq.build_report(
            document(rows), [Item("src/main/a.c", "a")], base="base",
            base_commit="abc", ranking_name="ranking.json", scan=1, top=1,
            freshness={
                ("src/main/a.c", "a"): rq.RankingEvidence("deadbeef", False),
            },
            classify=classify,
        )
        self.assertTrue(called)
        self.assertEqual([row["symbol"] for row in report["ready"]], ["a"])
        self.assertTrue(report["ready"][0]["requires_reproof"])
        self.assertEqual(report["ready"][0]["proof_quality"], "reproof-allocator")
        self.assertIn("refresh configured baseline", report["ready"][0]["reason"])

    def test_dirty_owning_path_is_not_classified_or_assigned(self) -> None:
        rows = [row("src/main/a.c", "a", 1)]
        called = False

        def classify(_base: str, _symbol: str) -> lane_status.Assignment:
            nonlocal called
            called = True
            return assignment("a", "src/main/a.c")

        report = rq.build_report(
            document(rows), [Item("src/main/a.c", "a")], base="base",
            base_commit="abc", ranking_name="ranking.json", scan=1, top=1,
            dirty_paths={"src/main/a.c"}, classify=classify,
        )
        self.assertFalse(called)
        self.assertEqual(report["ready"], [])
        self.assertEqual(report["summary"]["skipped"]["dirty-worktree"], 1)

    def test_primary_worktree_comes_from_shared_git_registry(self) -> None:
        completed = mock.Mock(
            returncode=0,
            stdout=(
                "worktree /repo/main\nHEAD deadbeef\nbranch refs/heads/main\n\n"
                "worktree /repo/lane\nHEAD cafe\nbranch refs/heads/lane/x\n"
            ),
            stderr="",
        )
        with mock.patch.object(rq.subprocess, "run", return_value=completed):
            self.assertEqual(Path("/repo/main"), rq.primary_worktree())

    def test_dirty_paths_are_read_from_explicit_primary_worktree(self) -> None:
        calls: list[list[str]] = []

        def run(command, **_kwargs):
            calls.append(command)
            output = "src/main/a.c\n" if "--cached" not in command else "docs/a.md\n"
            return mock.Mock(returncode=0, stdout=output, stderr="")

        with mock.patch.object(rq.subprocess, "run", side_effect=run):
            paths = rq.dirty_worktree_paths(Path("/repo/main"))
        self.assertEqual({"src/main/a.c", "docs/a.md"}, paths)
        self.assertTrue(all(call[:3] == ["git", "-C", "/repo/main"] for call in calls))

    def test_freshness_ignores_comments_and_other_candidate_bodies(self) -> None:
        old = """extern int shared;
#ifdef NON_MATCHING
void a(void) { shared++; }
#else
#pragma GLOBAL_ASM("asm/a.s")
#endif
#ifdef NON_MATCHING
void b(void) { shared++; }
#else
#pragma GLOBAL_ASM("asm/b.s")
#endif
"""
        current = """/* evidence note */
extern int shared;
#ifdef NON_MATCHING
void a(void) { shared++; }
#else
#pragma GLOBAL_ASM("asm/a.s")
#endif
#ifdef NON_MATCHING
void b(void) { shared += 2; }
#else
#pragma GLOBAL_ASM("asm/b.s")
#endif
"""
        doc = document([row("src/main/a.c", "a", 1)])
        with mock.patch.object(
            rq.nm_ranking, "ranking_evidence_commits",
            return_value={("src/main/a.c", "a"): "measured"},
        ), mock.patch.object(
            lane_status, "show_file",
            side_effect=lambda ref, _path: current if ref == "base" else old,
        ):
            freshness = rq.ranking_freshness("base", "ranking.json", doc)
        self.assertTrue(freshness[("src/main/a.c", "a")].fresh)

    def test_freshness_covers_shared_declarations(self) -> None:
        old = """extern int shared;
#ifdef NON_MATCHING
void a(void) { shared++; }
#else
#pragma GLOBAL_ASM("asm/a.s")
#endif
"""
        current = old.replace("extern int shared", "extern short shared")
        doc = document([row("src/main/a.c", "a", 1)])
        with mock.patch.object(
            rq.nm_ranking, "ranking_evidence_commits",
            return_value={("src/main/a.c", "a"): "measured"},
        ), mock.patch.object(
            lane_status, "show_file",
            side_effect=lambda ref, _path: current if ref == "base" else old,
        ):
            freshness = rq.ranking_freshness("base", "ranking.json", doc)
        self.assertFalse(freshness[("src/main/a.c", "a")].fresh)

    def test_live_source_path_disagreement_fails_closed(self) -> None:
        rows = [row("src/main/ranked.c", "target", 1)]
        with self.assertRaisesRegex(rq.ReadyQueueError, "source-path disagreement"):
            self.report(
                rows,
                [Item("src/main/live.c", "target")],
                {"target": assignment("target", "src/main/live.c")},
            )

    def test_base_source_path_disagreement_fails_closed(self) -> None:
        rows = [row("src/main/ranked.c", "target", 1)]
        with self.assertRaisesRegex(rq.ReadyQueueError, "source-path disagreement"):
            self.report(
                rows,
                [Item("src/main/ranked.c", "target")],
                {"target": assignment("target", "src/main/base.c")},
            )

    def test_absent_live_identity_is_summarized_without_classification(self) -> None:
        rows = [row("src/main/gone.c", "gone", 1)]
        report = self.report(rows, [], {})
        self.assertEqual(report["ready"], [])
        self.assertEqual(report["summary"]["skipped"]["not-live"], 1)
        self.assertEqual(report["skipped"][0]["state"], "not-live")

    def test_canonical_ranking_validation_is_mandatory(self) -> None:
        malformed = document([row("src/main/a.c", "a", 1)])
        malformed["resolved"] = 0
        with self.assertRaisesRegex(
            nm_ranking.RankingDocumentError, "resolved is 0, expected 1"
        ):
            rq.build_report(
                malformed, [], base="base", base_commit="abc",
                ranking_name="ranking.json", scan=1, top=1,
            )

    def test_json_and_human_outputs_are_deterministic(self) -> None:
        rows = [row("src/main/a.c", "a", 1)]
        report = self.report(
            rows, [Item("src/main/a.c", "a")],
            {"a": assignment("a", "src/main/a.c")},
        )
        first = json.dumps(report, indent=2, sort_keys=True)
        second = json.dumps(report, indent=2, sort_keys=True)
        self.assertEqual(first, second)
        self.assertEqual(rq.render_table(report), rq.render_table(report))
        self.assertEqual(rq.render_markdown(report), rq.render_markdown(report))
        self.assertIn("`a`", rq.render_markdown(report))

    def test_yield_output_exposes_score_tier_and_residual(self) -> None:
        candidate = row("src/main/a.c", "a", 1)
        candidate["relocation_masked_differing_words"] = 1
        report = self.report(
            [candidate], [Item("src/main/a.c", "a")],
            {"a": assignment("a", "src/main/a.c")},
            selection="expected-yield",
            freshness={
                ("src/main/a.c", "a"): rq.RankingEvidence("fresh", True),
            },
        )
        table = rq.render_table(report)
        self.assertIn("yield", table)
        self.assertIn("high-confidence", table)
        self.assertIn("0.100", table)
        self.assertEqual(report["schema_version"], 5)
        self.assertEqual(report["ready"][0]["residual_basis"], "relocation-masked")

    def test_duplicate_live_identity_fails_closed(self) -> None:
        with self.assertRaisesRegex(rq.ReadyQueueError, "duplicate identity"):
            rq.live_identities([
                Item("src/main/a.c", "a"), Item("src/main/a.c", "a")
            ])


class ArgumentTests(unittest.TestCase):
    def test_default_scan_reaches_the_hard_bound(self) -> None:
        args = rq.parse_args([])
        self.assertEqual(rq.MAX_SCAN, args.scan)
        self.assertEqual(rq.DEFAULT_TOP, args.top)

    def test_bounds_reject_zero_and_top_above_scan(self) -> None:
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                rq.parse_args(["--scan", "0"])
            with self.assertRaises(SystemExit):
                rq.parse_args(["--scan", "2", "--top", "3"])
            with self.assertRaises(SystemExit):
                rq.parse_args(["--jobs", "0"])

    def test_selection_modes_are_explicit_and_default_is_unchanged(self) -> None:
        self.assertEqual(rq.parse_args([]).selection, "default")
        for mode in rq.SELECTION_MODES:
            self.assertEqual(rq.parse_args(["--selection", mode]).selection, mode)
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                rq.parse_args(["--selection", "guess"])


if __name__ == "__main__":
    unittest.main()
