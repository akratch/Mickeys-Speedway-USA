#!/usr/bin/env python3
"""Regression tests for fail-closed lane assignment classification."""

from __future__ import annotations

import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import lane_status as ls  # noqa: E402


TOOL = Path(__file__).with_name("lane_status.py").resolve()
SYMBOL = "overlay43FilterImage"
SOURCE_PATH = Path("src/overlays/o043/overlay43FilterImage.c")
SHARD_PATH = Path("docs/matching-triage-handoffs") / f"{SYMBOL}.md"


def candidate(*, plateau: bool = False) -> str:
    text = f"""#ifdef NON_MATCHING
void {SYMBOL}(void) {{
}}
#else
#pragma GLOBAL_ASM(\"asm/nonmatchings/overlays/o043/{SYMBOL}.s\")
#endif
"""
    if plateau:
        text += f"""
/* PLATEAU-HANDOFF:{SYMBOL}:start
 * symbol: {SYMBOL}
 * score: 8/43 words
 * frame: frameless
 * relocations: 0
 * first-mismatch: +0x4
 * PLATEAU-HANDOFF:{SYMBOL}:end
 */
"""
    return text


def shard(*, source: str = SOURCE_PATH.as_posix(), symbol: str = SYMBOL) -> str:
    return ls.finalize_plateau.markdown_handoff(
        symbol,
        source,
        ls.finalize_plateau.Metrics(
            "35/43 words", "frameless", 0, "+0x4", "allocator web remains",
        ),
    )


class LaneStatusAssignmentTests(unittest.TestCase):
    def setUp(self) -> None:
        ls.show_file.cache_clear()
        ls.blob_id.cache_clear()
        self.temporary = tempfile.TemporaryDirectory()
        self.repo = Path(self.temporary.name)
        self.command("git", "init", "-q", "-b", "campaign/unchain")
        self.command("git", "config", "user.email", "lane-status@example.invalid")
        self.command("git", "config", "user.name", "Lane Status Test")
        (self.repo / SOURCE_PATH.parent).mkdir(parents=True)
        (self.repo / "docs").mkdir()
        (self.repo / SOURCE_PATH).write_text(candidate(), encoding="utf-8")
        (self.repo / "docs/matching-triage.md").write_text(
            f"| `{SYMBOL}` | clean source is pending |\n", encoding="utf-8",
        )
        self.commit("Seed overlay43FilterImage evidence")

    def tearDown(self) -> None:
        ls.show_file.cache_clear()
        ls.blob_id.cache_clear()
        ls.merge_base.cache_clear()
        self.temporary.cleanup()

    def command(self, *command: str, check: bool = True) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            command, cwd=self.repo, text=True, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, check=check,
        )

    def commit(self, subject: str) -> str:
        self.command("git", "add", ".")
        self.command("git", "commit", "-q", "-m", subject)
        return self.command("git", "rev-parse", "HEAD").stdout.strip()

    def status(self) -> tuple[subprocess.CompletedProcess[str], dict[str, object]]:
        result = self.command(
            sys.executable, str(TOOL), "--base", "campaign/unchain",
            "--symbol", SYMBOL, "--json", check=False,
        )
        return result, json.loads(result.stdout)

    def test_base_only_is_the_only_assignable_state(self) -> None:
        result, report = self.status()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(report["assignment"]["state"], "base-only")

    def test_call_followed_by_block_is_not_a_second_definition(self) -> None:
        caller = self.repo / "src/main/caller.c"
        caller.parent.mkdir(parents=True)
        caller.write_text(
            f"""extern void {SYMBOL}(void);
extern int accepts_value(int value);

void caller(void) {{
    if (accepts_value(({SYMBOL}(), 1))) {{
    }}
}}
""",
            encoding="utf-8",
        )
        self.commit("Add caller with a following control block")

        result, report = self.status()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(report["assignment"]["state"], "base-only")
        self.assertEqual(report["assignment"]["source_path"], SOURCE_PATH.as_posix())

    def test_unintegrated_source_blob_is_active(self) -> None:
        self.command("git", "switch", "-q", "-c", "lane/o43-active")
        (self.repo / SOURCE_PATH).write_text(
            candidate().replace("void overlay43FilterImage", "static void overlay43FilterImage"),
            encoding="utf-8",
        )
        self.commit("Work overlay43FilterImage allocator")
        self.command("git", "switch", "-q", "campaign/unchain")

        result, report = self.status()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(report["assignment"]["state"], "active")
        self.assertEqual(report["assignment"]["active_lanes"], ["lane/o43-active"])

    def test_unrelated_candidate_in_same_tu_does_not_reserve_target(self) -> None:
        second = """
#ifdef NON_MATCHING
void unrelatedFunction(void) {
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o043/unrelatedFunction.s")
#endif
"""
        (self.repo / SOURCE_PATH).write_text(candidate() + second, encoding="utf-8")
        self.commit("Add mixed overlay 43 candidates")
        self.command("git", "switch", "-q", "-c", "lane/o43-unrelated")
        (self.repo / SOURCE_PATH).write_text(
            (candidate() + second).replace(
                "void unrelatedFunction(void) {\n}",
                "static void unrelatedFunction(void) {\n    int value = 1;\n}",
            ),
            encoding="utf-8",
        )
        self.commit("Work unrelatedFunction allocator")
        self.command("git", "switch", "-q", "campaign/unchain")

        result, report = self.status()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(report["assignment"]["state"], "base-only")

    def test_unintegrated_target_handoff_is_active(self) -> None:
        self.command("git", "switch", "-q", "-c", "lane/o43-handoff")
        (self.repo / SOURCE_PATH).write_text(candidate(plateau=True), encoding="utf-8")
        self.commit("Plateau overlay43FilterImage allocator")
        self.command("git", "switch", "-q", "campaign/unchain")

        result, report = self.status()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(report["assignment"]["state"], "active")
        self.assertEqual(report["assignment"]["active_lanes"], ["lane/o43-handoff"])

    def test_unintegrated_target_shard_without_source_edit_is_active(self) -> None:
        self.command("git", "switch", "-q", "-c", "lane/o43-shard")
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text(shard(), encoding="utf-8")
        self.commit("Plateau overlay43FilterImage shard")
        self.command("git", "switch", "-q", "campaign/unchain")

        result, report = self.status()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(report["assignment"]["state"], "active")
        self.assertEqual(report["assignment"]["active_lanes"], ["lane/o43-shard"])

    def test_unintegrated_legacy_block_metric_edit_is_active(self) -> None:
        ledger = self.repo / "docs/matching-triage.md"
        ledger.write_text(shard(), encoding="utf-8")
        self.commit("Record overlay43FilterImage ledger block")
        self.command("git", "switch", "-q", "-c", "lane/o43-ledger-metric")
        ledger.write_text(
            shard().replace("35/43 words", "36/43 words"), encoding="utf-8",
        )
        self.commit("Update overlay43FilterImage plateau metric")
        self.command("git", "switch", "-q", "campaign/unchain")

        result, report = self.status()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(report["assignment"]["state"], "active")
        self.assertEqual(
            report["assignment"]["active_lanes"], ["lane/o43-ledger-metric"],
        )

    def test_unrelated_symbol_shard_does_not_reserve_target(self) -> None:
        self.command("git", "switch", "-q", "-c", "lane/other-shard")
        directory = self.repo / SHARD_PATH.parent
        directory.mkdir()
        (directory / "unrelatedFunction.md").write_text(
            shard(source="src/main/other.c", symbol="unrelatedFunction"),
            encoding="utf-8",
        )
        self.commit("Plateau unrelatedFunction")
        self.command("git", "switch", "-q", "campaign/unchain")

        result, report = self.status()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(report["assignment"]["state"], "base-only")

    def test_stale_pre_cleanup_triage_row_fails_closed(self) -> None:
        (self.repo / SOURCE_PATH).write_text(candidate(plateau=True), encoding="utf-8")
        source_commit = self.commit("Plateau overlay43FilterImage temp FIFO reproof")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "stale-ledger")
        self.assertEqual(assignment["source_commit"], source_commit)
        self.assertIn("predates", assignment["reason"])

    def test_reconciled_plateau_is_already_exhausted(self) -> None:
        (self.repo / SOURCE_PATH).write_text(candidate(plateau=True), encoding="utf-8")
        source_commit = self.commit("Plateau overlay43FilterImage temp FIFO reproof")
        (self.repo / "docs/matching-triage.md").write_text(
            f"| `{SYMBOL}` | bounded plateau; route exhausted |\n", encoding="utf-8",
        )
        ledger_commit = self.commit("Reconcile overlay43FilterImage plateau evidence")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], source_commit)
        self.assertEqual(assignment["ledger_commit"], ledger_commit)

    def test_legacy_generated_block_metric_refresh_is_current(self) -> None:
        source = candidate(plateau=True)
        ledger = self.repo / "docs/matching-triage.md"
        (self.repo / SOURCE_PATH).write_text(source, encoding="utf-8")
        ledger.write_text(shard(), encoding="utf-8")
        self.commit("Plateau overlay43FilterImage initial")
        (self.repo / SOURCE_PATH).write_text(
            source.replace("8/43 words", "9/43 words"), encoding="utf-8",
        )
        ledger.write_text(
            shard().replace("35/43 words", "36/43 words"), encoding="utf-8",
        )
        refreshed = self.commit("Plateau overlay43FilterImage refresh")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], refreshed)
        self.assertEqual(assignment["ledger_commit"], refreshed)

    def test_symbol_shard_reconciles_plateau_without_shared_ledger_edit(self) -> None:
        (self.repo / SOURCE_PATH).write_text(candidate(plateau=True), encoding="utf-8")
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text(shard(), encoding="utf-8")
        plateau_commit = self.commit("Plateau overlay43FilterImage allocator")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], plateau_commit)
        self.assertEqual(assignment["ledger_commit"], plateau_commit)

    def test_base_only_shard_repair_does_not_activate_historical_lane(self) -> None:
        (self.repo / SOURCE_PATH).write_text(candidate(plateau=True), encoding="utf-8")
        source_commit = self.commit("Plateau overlay43FilterImage allocator")
        self.command("git", "switch", "-q", "-c", "lane/historical")
        (self.repo / "lane-note.txt").write_text("unrelated\n", encoding="utf-8")
        self.commit("Record unrelated lane note")
        self.command("git", "switch", "-q", "campaign/unchain")
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text(shard(), encoding="utf-8")
        ledger_commit = self.commit("Reconcile overlay43FilterImage evidence")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], source_commit)
        self.assertEqual(assignment["ledger_commit"], ledger_commit)
        self.assertEqual(assignment["active_lanes"], [])

    def test_malformed_symbol_shard_fails_closed(self) -> None:
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text("# malformed\n", encoding="utf-8")
        self.commit("Add malformed overlay43FilterImage shard")

        result, report = self.status()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(report["assignment"]["state"], "stale-ledger")
        self.assertIn("malformed or foreign", report["assignment"]["reason"])

    def test_malformed_target_block_in_legacy_ledger_fails_closed(self) -> None:
        (self.repo / "docs/matching-triage.md").write_text(
            f"<!-- plateau-handoff:{SYMBOL}:start -->\n"
            f"### `{SYMBOL}` plateau handoff\n",
            encoding="utf-8",
        )
        ledger_commit = self.commit("Damage overlay43FilterImage handoff block")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "stale-ledger")
        self.assertEqual(assignment["ledger_commit"], ledger_commit)
        self.assertIn("malformed target-specific", assignment["reason"])

    def test_symbol_shard_source_mismatch_fails_closed(self) -> None:
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text(
            shard(source="src/main/wrong.c"), encoding="utf-8",
        )
        shard_commit = self.commit("Add wrong-source overlay43FilterImage shard")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "stale-ledger")
        self.assertEqual(assignment["ledger_commit"], shard_commit)
        self.assertIn("expected", assignment["reason"])

    def test_symbol_shard_older_than_source_plateau_is_stale(self) -> None:
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text(shard(), encoding="utf-8")
        shard_commit = self.commit("Add overlay43FilterImage plateau shard")
        (self.repo / SOURCE_PATH).write_text(candidate(plateau=True), encoding="utf-8")
        source_commit = self.commit("Plateau overlay43FilterImage reproof")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "stale-ledger")
        self.assertEqual(assignment["source_commit"], source_commit)
        self.assertEqual(assignment["ledger_commit"], shard_commit)
        self.assertIn("predates", assignment["reason"])

    def test_target_named_plateau_commit_is_evidence_without_marker(self) -> None:
        (self.repo / SOURCE_PATH).write_text(
            candidate().replace("void overlay43FilterImage", "static void overlay43FilterImage"),
            encoding="utf-8",
        )
        (self.repo / "docs/matching-triage.md").write_text(
            f"| `{SYMBOL}` | bounded plateau; route exhausted |\n", encoding="utf-8",
        )
        plateau_commit = self.commit("Plateau overlay43FilterImage allocator")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], plateau_commit)
        self.assertEqual(assignment["ledger_commit"], plateau_commit)

    def test_single_guard_path_plateau_without_symbol_fails_closed(self) -> None:
        (self.repo / SOURCE_PATH).write_text(
            "/* Plateau: bounded allocator route. */\n" + candidate(),
            encoding="utf-8",
        )
        source_commit = self.commit("Record overlay 043 Phase A plateaus")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "stale-ledger")
        self.assertEqual(assignment["source_commit"], source_commit)
        self.assertIn("predates", assignment["reason"])

    def test_single_guard_path_plateau_reconciles_by_ledger_row_edit(self) -> None:
        (self.repo / SOURCE_PATH).write_text(
            "/* Plateau: bounded allocator route. */\n" + candidate(),
            encoding="utf-8",
        )
        source_commit = self.commit("Record overlay 043 Phase A plateaus")
        (self.repo / "docs/matching-triage.md").write_text(
            f"| `{SYMBOL}` | bounded plateau; route exhausted |\n",
            encoding="utf-8",
        )
        ledger_commit = self.commit("Reconcile legacy overlay 043 evidence")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], source_commit)
        self.assertEqual(assignment["ledger_commit"], ledger_commit)

    def test_mixed_tu_plateau_uses_exact_row_and_source_commit(self) -> None:
        second = """\n#ifdef NON_MATCHING
void unrelatedFunction(void) {
}
#else
#pragma GLOBAL_ASM(\"asm/nonmatchings/overlays/o043/unrelatedFunction.s\")
#endif
"""
        (self.repo / SOURCE_PATH).write_text(
            candidate() + second, encoding="utf-8",
        )
        (self.repo / "docs/matching-triage.md").write_text(
            f"| `{SYMBOL}` | bounded plateau; route exhausted |\n",
            encoding="utf-8",
        )
        plateau_commit = self.commit("Record overlay 43 allocation plateau")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(
            assignment["state"], "already-integrated/exhausted",
        )
        self.assertEqual(assignment["source_commit"], plateau_commit)
        self.assertEqual(assignment["ledger_commit"], plateau_commit)

    def test_mixed_tu_plateau_uses_exact_symbol_shard(self) -> None:
        second = """\n#ifdef NON_MATCHING
void unrelatedFunction(void) {
}
#else
#pragma GLOBAL_ASM(\"asm/nonmatchings/overlays/o043/unrelatedFunction.s\")
#endif
"""
        (self.repo / SOURCE_PATH).write_text(
            candidate(plateau=True) + second, encoding="utf-8",
        )
        (self.repo / SHARD_PATH.parent).mkdir()
        (self.repo / SHARD_PATH).write_text(shard(), encoding="utf-8")
        plateau_commit = self.commit("Record overlay 43 allocation plateau")

        result, report = self.status()
        assignment = report["assignment"]
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(assignment["state"], "already-integrated/exhausted")
        self.assertEqual(assignment["source_commit"], plateau_commit)
        self.assertEqual(assignment["ledger_commit"], plateau_commit)

    def test_mixed_tu_plateau_row_without_source_change_is_stale(self) -> None:
        second = """\n#ifdef NON_MATCHING
void unrelatedFunction(void) {
}
#else
#pragma GLOBAL_ASM(\"asm/nonmatchings/overlays/o043/unrelatedFunction.s\")
#endif
"""
        (self.repo / SOURCE_PATH).write_text(
            candidate() + second, encoding="utf-8",
        )
        self.commit("Create mixed overlay 43 translation unit")
        (self.repo / "docs/matching-triage.md").write_text(
            f"| `{SYMBOL}` | bounded plateau; route exhausted |\n",
            encoding="utf-8",
        )
        self.commit("Record overlay 43 allocation plateau")

        result, report = self.status()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(report["assignment"]["state"], "base-only")

    def test_definition_without_fallback_is_already_integrated(self) -> None:
        (self.repo / SOURCE_PATH).write_text(
            f"void {SYMBOL}(void) {{\n}}\n", encoding="utf-8",
        )
        self.commit("Match overlay43FilterImage")

        result, report = self.status()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(
            report["assignment"]["state"], "already-integrated/exhausted",
        )


class LaneRefQueryTests(unittest.TestCase):
    def tearDown(self) -> None:
        ls.show_file.cache_clear()
        ls.blob_id.cache_clear()
        ls.merge_base.cache_clear()

    def test_lane_scan_filters_refs_already_merged_into_base(self) -> None:
        with mock.patch.object(ls, "git", return_value="") as git:
            self.assertEqual(
                ls.lane_refs(containing="source", unmerged_into="campaign/unchain"),
                [],
            )
        self.assertIn("--contains=source", git.call_args.args)
        self.assertIn("--no-merged=campaign/unchain", git.call_args.args)

    def test_equal_source_blob_skips_candidate_resolution(self) -> None:
        calls = []

        def fake_git(*args, **_kwargs):
            calls.append(args)
            if args[0] == "for-each-ref":
                return "lane/history\x00deadbeef\n"
            raise AssertionError(f"unexpected Git query: {args}")

        def fake_blobs(_refs, path):
            if path == "path.c":
                return {"lane/history": ("same-blob", "base")}
            return {"lane/history": None}

        with mock.patch.object(ls, "git", side_effect=fake_git), mock.patch.object(
            ls,
            "blob_contents",
            side_effect=fake_blobs,
        ), mock.patch.object(
            ls, "show_file", return_value=None,
        ), mock.patch.object(
            ls.finalize_plateau,
            "require_guarded_candidate",
            side_effect=AssertionError("should be skipped"),
        ):
            active = ls.active_lanes_for_source(
                "campaign/unchain", "symbol", "path.c", "same-blob", "source", "base"
            )
        self.assertEqual(active, [])

    def test_blob_ids_uses_one_batch_object_query(self) -> None:
        completed = subprocess.CompletedProcess(
            [], 0, "a" * 40 + " blob\n" + "b" * 40 + " blob\n", ""
        )
        with mock.patch.object(subprocess, "run", return_value=completed) as run:
            rows = ls.blob_ids(["lane/one", "lane/two"], "src/a.c")
        self.assertEqual(rows["lane/one"], "a" * 40)
        self.assertEqual(rows["lane/two"], "b" * 40)
        self.assertEqual(run.call_count, 1)

    def test_blob_contents_uses_one_batch_object_query(self) -> None:
        first = b"one\n"
        second = b"two\n"
        output = (
            ("a" * 40 + f" blob {len(first)}\n").encode() + first + b"\n"
            + ("b" * 40 + f" blob {len(second)}\n").encode() + second + b"\n"
        )
        completed = subprocess.CompletedProcess([], 0, output, b"")
        with mock.patch.object(subprocess, "run", return_value=completed) as run:
            rows = ls.blob_contents(["lane/one", "lane/two"], "src/a.c")
        self.assertEqual(rows["lane/one"], ("a" * 40, "one\n"))
        self.assertEqual(rows["lane/two"], ("b" * 40, "two\n"))
        self.assertEqual(run.call_count, 1)

    def test_batch_source_identity_uses_one_grep_and_one_object_batch(self) -> None:
        completed = subprocess.CompletedProcess(
            [], 0, "HEAD:src/a.c\nHEAD:src/b.c\n", "",
        )
        sources = {
            "src/a.c": ("a" * 40, "void alpha(void) { }\n"),
            "src/b.c": ("b" * 40, "void beta(void) { alpha(); }\n"),
        }
        with mock.patch.object(
            subprocess, "run", return_value=completed,
        ) as run, mock.patch.object(
            ls, "blob_contents_by_path", return_value=sources,
        ) as batch:
            identities = ls.source_identity_index("HEAD", ["alpha", "beta"])
        self.assertEqual(identities["alpha"], ("src/a.c", None))
        self.assertEqual(identities["beta"], ("src/b.c", None))
        self.assertEqual(run.call_count, 1)
        batch.assert_called_once_with("HEAD", ["src/a.c", "src/b.c"])

    def test_lane_index_filters_shared_legacy_edits_by_exact_symbol(self) -> None:
        base = ls.LanePathIndex(
            base="base",
            refs_by_path={"src/a.c": (("lane/source", "a" * 40),)},
            legacy_refs_by_symbol={
                "alpha": (("lane/legacy", "b" * 40),),
            },
            common_by_branch={
                "lane/source": "c" * 40,
                "lane/legacy": "d" * 40,
            },
        )
        self.assertEqual(
            base.refs_for(["src/a.c"], symbol="alpha"),
            [("lane/legacy", "b" * 40), ("lane/source", "a" * 40)],
        )
        self.assertEqual(
            base.refs_for(["src/other.c"], symbol="beta"), [],
        )

    def test_indexed_active_scan_skips_unrelated_lane_paths(self) -> None:
        index = ls.LanePathIndex(
            base="base", refs_by_path={}, legacy_refs_by_symbol={},
            common_by_branch={},
        )
        with mock.patch.object(
            ls, "lane_refs", side_effect=AssertionError("must use index"),
        ), mock.patch.object(
            ls, "is_ancestor", side_effect=AssertionError("no candidates"),
        ):
            active = ls.active_lanes_for_source(
                "base", "alpha", "src/a.c", "blob", "commit", "source",
                index,
            )
        self.assertEqual(active, [])


if __name__ == "__main__":
    unittest.main()
