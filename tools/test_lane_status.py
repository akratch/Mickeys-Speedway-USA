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


class LaneStatusAssignmentTests(unittest.TestCase):
    def setUp(self) -> None:
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
    def test_lane_scan_filters_refs_already_merged_into_base(self) -> None:
        with mock.patch.object(ls, "git", return_value="") as git:
            self.assertEqual(
                ls.lane_refs(containing="source", unmerged_into="campaign/unchain"),
                [],
            )
        self.assertIn("--contains=source", git.call_args.args)
        self.assertIn("--no-merged=campaign/unchain", git.call_args.args)

    def test_equal_source_blob_skips_expensive_symbol_resolution(self) -> None:
        calls = []

        def fake_git(*args, **_kwargs):
            calls.append(args)
            if args[0] == "for-each-ref":
                return "lane/history\x00deadbeef\n"
            raise AssertionError(f"unexpected Git query: {args}")

        with mock.patch.object(ls, "git", side_effect=fake_git), mock.patch.object(
            ls, "blob_ids", return_value={"lane/history": "same-blob"}
        ), mock.patch.object(
            ls, "source_identity", side_effect=AssertionError("should be skipped")
        ):
            active = ls.active_lanes_for_source(
                "campaign/unchain", "symbol", "path.c", "same-blob", "source"
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


if __name__ == "__main__":
    unittest.main()
