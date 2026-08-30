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

    def report(self, symbol: str = "close_one") -> dict[str, object]:
        return {
            "schema": "mickey-function-evidence-preflight-v1",
            "candidate_symbol": symbol,
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

    def test_accepts_yield_queue_schema(self) -> None:
        queue = self.queue()
        queue["schema_version"] = 5
        self.assertEqual("close_one", pr.select_stale_rows(queue, 1)[0]["symbol"])

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

    def test_discovery_requests_expected_yield_order(self) -> None:
        args = pr.build_parser().parse_args([])
        with mock.patch.object(pr, "_run_json", return_value=self.queue()) as run:
            pr.discover(args)
        self.assertIn("expected-yield", run.call_args.args[0])

    def test_rejects_inconsistent_relocation_counts(self) -> None:
        report = self.report()
        report["preflight"]["counts"]["candidate_identities_unresolved"] = 1
        with self.assertRaisesRegex(pr.RemeasureError, "identity counts disagree"):
            pr.summarize_preflight(report)

    def test_explicit_batch_continues_after_middle_command_failure(self) -> None:
        reports = [
            self.report("first_one"),
            pr.MeasurementFailure(
                "command-failed", "preflight middle_one command exited with status 1"
            ),
            self.report("last_one"),
        ]
        with (
            mock.patch.object(pr, "_run_json", side_effect=reports) as run,
            mock.patch("builtins.print") as output,
        ):
            exit_code = pr.main(
                ["--format", "json", "first_one", "middle_one", "last_one"]
            )
        payload = json.loads(output.call_args.args[0])
        self.assertEqual(1, exit_code)
        self.assertEqual(3, run.call_count)
        self.assertEqual(
            ["first_one", "middle_one", "last_one"],
            [row["symbol"] for row in payload["functions"]],
        )
        self.assertEqual(
            ["complete", "failed", "complete"],
            [row["status"] for row in payload["functions"]],
        )
        self.assertEqual(
            {"selected": 3, "measured": 2, "complete": 2, "partial": 0, "failed": 1},
            payload["summary"],
        )

    def test_discovered_batch_uses_same_resilient_ordering(self) -> None:
        selected = [
            {"symbol": "first_one", "source": "src/main/example.c", "queue_rank": 1},
            {"symbol": "middle_one", "source": "src/main/example.c", "queue_rank": 2},
            {"symbol": "last_one", "source": "src/main/example.c", "queue_rank": 3},
        ]
        rows = [
            pr.summarize_preflight(self.report("first_one")),
            pr.failure_row(
                selected[1], category="timeout", reason="preflight middle_one timed out"
            ),
            pr.summarize_preflight(self.report("last_one")),
        ]
        with (
            mock.patch.object(pr, "discover", return_value=selected),
            mock.patch.object(pr, "measure_safely", side_effect=rows) as measure,
            mock.patch("builtins.print") as output,
        ):
            exit_code = pr.main(["--format", "json"])
        payload = json.loads(output.call_args.args[0])
        self.assertEqual(1, exit_code)
        self.assertEqual(selected, [call.args[0] for call in measure.call_args_list])
        self.assertEqual(
            ["first_one", "middle_one", "last_one"],
            [row["symbol"] for row in payload["functions"]],
        )

    def test_timeout_becomes_fail_closed_result(self) -> None:
        args = pr.build_parser().parse_args(["close_one"])
        expired = pr.subprocess.TimeoutExpired(
            ["preflight"], args.timeout, output="raw bytes", stderr="traceback"
        )
        with mock.patch.object(pr.subprocess, "run", side_effect=expired):
            row = pr.measure_safely({"symbol": "close_one"}, args)
        self.assertEqual("failed", row["status"])
        self.assertEqual("timeout", row["failure_category"])
        self.assertNotIn("raw bytes", row["reason"])
        self.assertNotIn("traceback", row["reason"])

    def test_malformed_json_becomes_fail_closed_result(self) -> None:
        args = pr.build_parser().parse_args(["close_one"])
        completed = pr.subprocess.CompletedProcess(
            ["preflight"], 0, stdout="not JSON /Users/example/private", stderr=""
        )
        with mock.patch.object(pr.subprocess, "run", return_value=completed):
            row = pr.measure_safely({"symbol": "close_one"}, args)
        self.assertEqual("failed", row["status"])
        self.assertEqual("malformed-json", row["failure_category"])
        self.assertNotIn("not JSON", row["reason"])
        self.assertNotIn("/Users/", row["reason"])

    def test_command_failure_never_copies_raw_diagnostic(self) -> None:
        args = pr.build_parser().parse_args(["close_one"])
        completed = pr.subprocess.CompletedProcess(
            ["preflight"],
            7,
            stdout="instruction bytes 0xDEADBEEF",
            stderr="Traceback at /Users/example/private.py\nraw disassembly",
        )
        with mock.patch.object(pr.subprocess, "run", return_value=completed):
            row = pr.measure_safely({"symbol": "close_one"}, args)
        self.assertEqual("command-failed", row["failure_category"])
        self.assertEqual(
            "preflight close_one command exited with status 7", row["reason"]
        )
        serialized = json.dumps(row)
        self.assertNotIn("DEADBEEF", serialized)
        self.assertNotIn("Traceback", serialized)
        self.assertNotIn("/Users/", serialized)
        self.assertNotIn("disassembly", serialized)

    def test_reason_sanitization_is_bounded_and_path_free(self) -> None:
        reason = pr.sanitize_failure_reason(
            "/Users/example/private/file.c\n" + "detail " * 100
        )
        self.assertNotIn("/Users/", reason)
        self.assertNotIn("\n", reason)
        self.assertLessEqual(len(reason), pr.FAILURE_REASON_LIMIT)

    def test_invalid_evidence_is_not_counted_as_measurement(self) -> None:
        bad_report = self.report()
        bad_report["preflight"]["counts"]["candidate_identities_unresolved"] = 1
        with (
            mock.patch.object(pr, "_run_json", return_value=bad_report),
            mock.patch("builtins.print") as output,
        ):
            exit_code = pr.main(["--format", "json", "close_one"])
        payload = json.loads(output.call_args.args[0])
        self.assertEqual(1, exit_code)
        self.assertEqual("invalid-evidence", payload["functions"][0]["failure_category"])
        self.assertEqual(0, payload["summary"]["measured"])
        self.assertEqual(1, payload["summary"]["failed"])

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
        self.assertEqual([measured], payload["functions"])
        self.assertEqual(1, payload["summary"]["measured"])
        self.assertEqual(0, payload["summary"]["failed"])

    def test_table_retains_success_rows_and_reports_failures_and_counts(self) -> None:
        measured = pr.summarize_preflight(self.report())
        failed = pr.failure_row(
            {"symbol": "middle_one"},
            category="command-failed",
            reason="preflight middle_one command exited with status 2",
        )
        table = pr.render_table([measured, failed])
        self.assertIn("close_one", table)
        self.assertIn("middle_one", table)
        self.assertIn("failure=command-failed", table)
        self.assertTrue(
            table.endswith("selected=2 measured=1 complete=1 partial=0 failed=1")
        )


if __name__ == "__main__":
    unittest.main()
