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
