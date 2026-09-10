#!/usr/bin/env python3
"""The lane-facing scorer must agree with the ranking, and must refuse
to invent a measurement for a function that is not queued."""
import unittest
from unittest import mock

import score_symbol


class QueueGuardTests(unittest.TestCase):
    def test_a_symbol_outside_the_queue_is_reported_not_scored(self):
        """A matched function has no target .s to compare against; saying so
        is the useful answer, and silently scoring it would be a lie."""
        rows, errors = score_symbol.score(["func_definitely_not_queued"])
        self.assertEqual(rows, [])
        self.assertEqual(len(errors), 1)
        self.assertIn("not in the NON_MATCHING queue", errors[0])

    def test_nothing_is_compiled_when_no_symbol_resolves(self):
        """Do not pay for a TU compile to answer a question about a typo."""
        with mock.patch.object(score_symbol.nr, "configured_compile_commands") as commands:
            rows, errors = score_symbol.score(["nope_not_real"])
        commands.assert_not_called()
        self.assertEqual(rows, [])
        self.assertTrue(errors)

    def test_a_missing_symbol_does_not_suppress_a_real_one(self):
        queue = {item.func for item in score_symbol.pb.discover_queue()}
        self.assertTrue(queue, "queue discovery returned nothing")
        real = sorted(queue)[0]
        rows, errors = score_symbol.score([real, "nope_not_real"])
        self.assertEqual([row["symbol"] for row in rows], [real])
        self.assertEqual(len(errors), 1)


class IsolationTests(unittest.TestCase):
    """This tool is meant to run in a loop while a build is going.

    nm_ranking's work directory is a fixed path under build/, which is right
    for a whole-queue pass that owns the tree and wrong for a per-symbol call.
    Two callers, or a caller and a concurrent gmake, would write the same
    object paths. A lane hit that and wrote its own scorer instead, which is
    the outcome this tool exists to prevent.
    """

    def test_the_work_directory_is_swapped_for_a_private_one(self):
        outer = score_symbol.nr.WORK_DIR
        with score_symbol._isolated_workdir():
            inner = score_symbol.nr.WORK_DIR
            self.assertNotEqual(inner, outer)
            self.assertTrue(inner.is_dir())
            self.assertNotIn("nm_ranking", str(inner))
        self.assertEqual(score_symbol.nr.WORK_DIR, outer, "must be restored")

    def test_two_runs_do_not_share_a_directory(self):
        seen = []
        for _ in range(2):
            with score_symbol._isolated_workdir():
                seen.append(score_symbol.nr.WORK_DIR)
        self.assertNotEqual(seen[0], seen[1])

    def test_the_scratch_is_removed_afterwards(self):
        with score_symbol._isolated_workdir():
            scratch = score_symbol.nr.WORK_DIR
        self.assertFalse(scratch.exists(), "scratch must not accumulate")

    def test_the_work_directory_is_restored_even_on_failure(self):
        outer = score_symbol.nr.WORK_DIR
        with self.assertRaises(RuntimeError):
            with score_symbol._isolated_workdir():
                raise RuntimeError("boom")
        self.assertEqual(score_symbol.nr.WORK_DIR, outer)


class AgreementTests(unittest.TestCase):
    def test_the_scorer_reports_the_masked_count_the_ranking_stores(self):
        """Agreement with docs/nm-ranking.md must hold by construction."""
        import json
        from pathlib import Path
        artifact = Path(__file__).resolve().parents[1] / "config" / "nonmatching-ranking.us.json"
        stored = {row["name"]: row for row in json.loads(artifact.read_text())["functions"]}
        subject = next(iter(sorted(stored)))
        rows, errors = score_symbol.score([subject])
        self.assertEqual(errors, [])
        self.assertEqual(len(rows), 1)
        for field in ("differing_words", "relocation_masked_differing_words",
                      "size_bytes", "size_delta"):
            self.assertEqual(rows[0][field], stored[subject][field], field)

    def test_the_artifact_column_is_the_difference_of_the_two_counts(self):
        rows, _ = score_symbol.score([sorted(
            item.func for item in score_symbol.pb.discover_queue())[0]])
        row = rows[0]
        self.assertEqual(row["relocation_artifact_words"],
                         row["differing_words"] - row["relocation_masked_differing_words"])


if __name__ == "__main__":
    unittest.main()
