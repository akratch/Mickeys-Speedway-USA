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
) -> lane_status.Assignment:
    return lane_status.Assignment(
        symbol=symbol,
        state=state,
        source_path=file_name,
        source_commit="source" if state != "base-only" else None,
        ledger_commit="ledger" if state == "already-integrated/exhausted" else None,
        active_lanes=active_lanes or [],
        reason=f"{state} reason",
    )


class ReadyQueueTests(unittest.TestCase):
    def report(self, rows, items, states, *, scan=10, top=10):
        def classify(_base: str, symbol: str) -> lane_status.Assignment:
            return states[symbol]

        return rq.build_report(
            document(rows), items, base="base", base_commit="abc",
            ranking_name="ranking.json", scan=scan, top=top, classify=classify,
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
            "c": assignment("c", "src/main/c.c", "stale-ledger"),
            "d": assignment("d", "src/main/d.c"),
            "e": assignment("e", "src/main/e.c"),
        }
        report = self.report(rows, items, states)
        self.assertEqual([value["symbol"] for value in report["ready"]], ["d", "e"])
        self.assertEqual([value["rank"] for value in report["ready"]], [4, 5])
        self.assertEqual(
            report["summary"]["skipped"],
            {
                "active": 1,
                "already-integrated/exhausted": 1,
                "stale-ledger": 1,
                "not-live": 0,
            },
        )
        self.assertEqual(report["skipped"][0]["active_lanes"], ["lane/a"])

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
            rq, "ranking_evidence_commits",
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
            rq, "ranking_evidence_commits",
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

    def test_duplicate_live_identity_fails_closed(self) -> None:
        with self.assertRaisesRegex(rq.ReadyQueueError, "duplicate identity"):
            rq.live_identities([
                Item("src/main/a.c", "a"), Item("src/main/a.c", "a")
            ])


class ArgumentTests(unittest.TestCase):
    def test_bounds_reject_zero_and_top_above_scan(self) -> None:
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                rq.parse_args(["--scan", "0"])
            with self.assertRaises(SystemExit):
                rq.parse_args(["--scan", "2", "--top", "3"])
            with self.assertRaises(SystemExit):
                rq.parse_args(["--jobs", "0"])


if __name__ == "__main__":
    unittest.main()
