#!/usr/bin/env python3
"""Focused tests for bounded stale-plateau remeasurement."""

from __future__ import annotations

import importlib.util
import json
import unittest
from pathlib import Path
from unittest import mock


MODULE_PATH = Path(__file__).with_name("plateau_remeasure.py")
SPEC = importlib.util.spec_from_file_location("plateau_remeasure", MODULE_PATH)
assert SPEC and SPEC.loader
pr = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(pr)


class PlateauRemeasureTests(unittest.TestCase):
    def queue(self) -> dict[str, object]:
        return {
            "schema_version": 4,
            "skipped": [
                {
                    "symbol": "close_one",
                    "file": "src/main/example.c",
                    "rank": 2,
                    "differing_words": 3,
                    "relocation_masked_differing_words": 2,
                    "maintenance_class": pr.MAINTENANCE_CLASS,
                },
                {
                    "symbol": "current_one",
                    "file": "src/main/example.c",
                    "rank": 3,
                    "differing_words": 1,
                    "relocation_masked_differing_words": 1,
                    "maintenance_class": "current-plateau",
                },
            ],
        }

    def report(self) -> dict[str, object]:
        return {
            "schema": "mickey-function-evidence-preflight-v1",
            "candidate_symbol": "close_one",
            "source": "src/main/example.c",
            "workbench": {
                "differing_words": 2,
                "target_words": 20,
                "candidate_words": 20,
                "first_mismatch": "+0x8",
                "verdict": "register-only",
                "target_frame": 32,
                "candidate_frame": 32,
            },
            "preflight": {
                "status": "complete",
                "counts": {
                    "target_relocations": 3,
                    "candidate_static_relocations": 3,
                    "candidate_identities_resolved": 3,
                    "candidate_identities_unresolved": 0,
                    "offset_type_aligned": 3,
                },
            },
        }

    def test_selects_only_exact_stale_class(self) -> None:
        rows = pr.select_stale_rows(self.queue(), 10)
        self.assertEqual(["close_one"], [row["symbol"] for row in rows])
        self.assertEqual(2, rows[0]["retained_masked_differing_words"])

    def test_rejects_unknown_queue_schema(self) -> None:
        with self.assertRaisesRegex(pr.RemeasureError, "unsupported ready-queue schema"):
            pr.select_stale_rows({"schema_version": 3, "skipped": []}, 1)

    def test_summarizes_only_scalar_evidence(self) -> None:
        row = pr.summarize_preflight(self.report())
        self.assertEqual("complete", row["status"])
        self.assertEqual(2, row["differing_words"])
        self.assertEqual(3, row["resolved_identities"])
        self.assertNotIn("relocation_comparison", row)

    def test_rejects_malformed_first_mismatch(self) -> None:
        report = self.report()
        report["workbench"]["first_mismatch"] = "8"
        with self.assertRaisesRegex(pr.RemeasureError, "invalid first mismatch"):
            pr.summarize_preflight(report)

    def test_no_build_is_forwarded_to_preflight(self) -> None:
        args = pr.build_parser().parse_args(["--no-build", "close_one"])
        with mock.patch.object(pr, "_run_json", return_value=self.report()) as run:
            row = pr.measure({"symbol": "close_one"}, args)
        self.assertIn("--no-build", run.call_args.args[0])
        self.assertEqual("close_one", row["symbol"])

    def test_rejects_inconsistent_relocation_counts(self) -> None:
        report = self.report()
        report["preflight"]["counts"]["candidate_identities_unresolved"] = 1
        with self.assertRaisesRegex(pr.RemeasureError, "identity counts disagree"):
            pr.summarize_preflight(report)

    def test_json_main_is_one_document(self) -> None:
        measured = pr.summarize_preflight(self.report())
        with (
            mock.patch.object(pr, "measure", return_value=measured),
            mock.patch("builtins.print") as output,
        ):
            self.assertEqual(0, pr.main(["--format", "json", "close_one"]))
        payload = json.loads(output.call_args.args[0])
        self.assertEqual(pr.SCHEMA, payload["schema"])
        self.assertEqual(1, payload["count"])


if __name__ == "__main__":
    unittest.main()
