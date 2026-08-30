#!/usr/bin/env python3
"""Focused tests for campaign worker heartbeat state."""

from __future__ import annotations

import argparse
import contextlib
import datetime as dt
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock


TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import crew  # noqa: E402


class HeartbeatTests(unittest.TestCase):
    def record(self, **changes: object) -> dict[str, object]:
        row: dict[str, object] = {
            "schema": 2,
            "worker": "worker-1",
            "target": "demo_symbol",
            "base": "a" * 40,
            "last_progress": "baseline compared",
            "last_progress_at": "2026-08-29T10:00:00Z",
            "last_commit": "b" * 40,
            "deadline_unix": 1787998500,
            "attempt_count": 2,
            "best_score": "192/287 words differ",
            "mismatch_class": "register-allocation",
            "eta_unix": None,
            "state": "active",
            "updated_at": "2026-08-29T10:00:00Z",
        }
        row.update(changes)
        return row

    def legacy_record(self, **changes: object) -> dict[str, object]:
        row = self.record(schema=1)
        for field in ("attempt_count", "best_score", "mismatch_class", "eta_unix"):
            row.pop(field)
        row.update(changes)
        return row

    def test_staleness_reports_progress_and_deadline_independently(self) -> None:
        now = dt.datetime(2026, 8, 29, 10, 30, tzinfo=dt.timezone.utc)
        reasons = crew.heartbeat_staleness(
            self.record(deadline_unix=1787998200), now, dt.timedelta(minutes=15)
        )
        self.assertTrue(any("no progress" in reason for reason in reasons))
        self.assertTrue(any("deadline passed" in reason for reason in reasons))

    def test_terminal_heartbeat_is_not_stale(self) -> None:
        now = dt.datetime(2026, 8, 30, tzinfo=dt.timezone.utc)
        reasons = crew.heartbeat_staleness(
            self.record(state="handoff"), now, dt.timedelta(minutes=1)
        )
        self.assertEqual(reasons, [])

    def test_passed_eta_is_stale_independently_of_deadline(self) -> None:
        now = dt.datetime(2026, 8, 29, 10, 5, tzinfo=dt.timezone.utc)
        reasons = crew.heartbeat_staleness(
            self.record(
                last_progress_at="2026-08-29T10:04:00Z",
                updated_at="2026-08-29T10:04:00Z",
                deadline_unix=int(now.timestamp()) + 3600,
                eta_unix=int(now.timestamp()) - 60,
            ),
            now,
            dt.timedelta(minutes=15),
        )
        self.assertEqual(reasons, ["ETA passed 1m ago"])

    def test_schema_one_record_is_readable_with_checkpoint_defaults(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.legacy_record()) + "\n")
            loaded = crew.load_heartbeat(path)
        self.assertEqual(loaded["attempt_count"], 0)
        self.assertEqual(loaded["best_score"], "not recorded")
        self.assertEqual(loaded["mismatch_class"], "unclassified")
        self.assertIsNone(loaded["eta_unix"])

    def test_heartbeat_update_preserves_assignment_and_refreshes_commit(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(
                worker="worker-1", target=None, base=None, deadline_unix=None,
                progress="candidate improved", state="active", attempt_count=3,
                best_score="187/287 words differ", mismatch_class="frame-allocation",
                eta_unix=1787998400,
            )
            with mock.patch.object(crew, "run_git", return_value="c" * 40):
                with contextlib.redirect_stdout(io.StringIO()):
                    crew.command_heartbeat(args, root)
            updated = crew.load_heartbeat(path)
            self.assertEqual(updated["target"], "demo_symbol")
            self.assertEqual(updated["base"], "a" * 40)
            self.assertEqual(updated["last_commit"], "c" * 40)
            self.assertEqual(updated["last_progress"], "candidate improved")
            self.assertEqual(updated["attempt_count"], 3)
            self.assertEqual(updated["best_score"], "187/287 words differ")
            self.assertEqual(updated["mismatch_class"], "frame-allocation")
            self.assertEqual(updated["eta_unix"], 1787998400)

    def test_legacy_update_rewrites_schema_two(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.legacy_record()) + "\n")
            args = argparse.Namespace(
                worker="worker-1", target=None, base=None, deadline_unix=None,
                progress="baseline measured", state="active", attempt_count=1,
                best_score="200 words differ", mismatch_class="structure", eta_unix=None,
            )
            with mock.patch.object(crew, "run_git", return_value="c" * 40):
                with contextlib.redirect_stdout(io.StringIO()):
                    crew.command_heartbeat(args, root)
            raw = json.loads(path.read_text(encoding="utf-8"))
        self.assertEqual(raw["schema"], 2)
        self.assertEqual(raw["attempt_count"], 1)

    def test_attempt_count_cannot_regress_for_same_assignment(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record(attempt_count=4)) + "\n")
            args = argparse.Namespace(
                worker="worker-1", target=None, base=None, deadline_unix=None,
                progress="older checkpoint", state="active", attempt_count=3,
                best_score=None, mismatch_class=None, eta_unix=None,
            )
            with self.assertRaisesRegex(crew.CrewError, "cannot decrease"):
                crew.command_heartbeat(args, root)

    def test_json_status_is_concise_and_contains_matching_checkpoint(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(
                worker=None, stale_after_minutes=15, check=True, json=True,
            )
            now = dt.datetime(2026, 8, 29, 10, 5, tzinfo=dt.timezone.utc)
            output = io.StringIO()
            with mock.patch.object(crew, "utc_now", return_value=now):
                with contextlib.redirect_stdout(output):
                    crew.command_heartbeat_status(args, root)
            self.assertEqual(len(output.getvalue().splitlines()), 1)
            report = json.loads(output.getvalue())
        self.assertTrue(report["ok"])
        self.assertEqual(report["summary"], {
            "current": 1, "malformed": 0, "stale": 0, "terminal": 0, "total": 1,
        })
        worker = report["workers"][0]
        self.assertEqual(worker["attempt_count"], 2)
        self.assertEqual(worker["best_score"], "192/287 words differ")
        self.assertEqual(worker["mismatch_class"], "register-allocation")
        self.assertEqual(worker["health"], "current")

    def test_malformed_json_report_fails_closed_without_hiding_other_workers(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            directory = root / "heartbeats"
            crew.atomic_write(directory / "worker-1.json", json.dumps(self.record()) + "\n")
            crew.atomic_write(directory / "worker-2.json", "{not json\n")
            args = argparse.Namespace(
                worker=None, stale_after_minutes=15, check=False, json=True,
            )
            now = dt.datetime(2026, 8, 29, 10, 5, tzinfo=dt.timezone.utc)
            output = io.StringIO()
            with mock.patch.object(crew, "utc_now", return_value=now):
                with mock.patch("os.kill") as kill:
                    with contextlib.redirect_stdout(output):
                        with self.assertRaisesRegex(crew.CrewError, "malformed heartbeat"):
                            crew.command_heartbeat_status(args, root)
            report = json.loads(output.getvalue())
        self.assertFalse(report["ok"])
        self.assertEqual(report["summary"]["current"], 1)
        self.assertEqual(report["summary"]["malformed"], 1)
        self.assertEqual(report["workers"][1]["health"], "malformed")
        kill.assert_not_called()

    def test_stale_json_check_reports_before_failing_and_never_kills(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(
                worker=None, stale_after_minutes=1, check=False, json=True,
            )
            now = dt.datetime(2026, 8, 29, 11, 0, tzinfo=dt.timezone.utc)
            output = io.StringIO()
            with mock.patch.object(crew, "utc_now", return_value=now):
                with mock.patch("os.kill") as kill:
                    with contextlib.redirect_stdout(output):
                        with self.assertRaisesRegex(crew.CrewError, "stale heartbeat"):
                            crew.command_heartbeat_status(args, root)
            report = json.loads(output.getvalue())
        self.assertFalse(report["ok"])
        self.assertEqual(report["summary"]["stale"], 1)
        self.assertEqual(report["workers"][0]["health"], "stale")
        kill.assert_not_called()

    def test_worker_identity_must_match_heartbeat_filename(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "worker-2.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            with self.assertRaisesRegex(crew.CrewError, "expected 'worker-2'"):
                crew.load_heartbeat(path)

    def test_schema_two_rejects_noncanonical_or_unknown_checkpoint_state(self) -> None:
        malformed = (
            {"schema": True},
            {"attempt_count": True},
            {"last_progress": "two\nlines"},
            {"eta_unix": 10**100},
            {"unexpected": "field"},
        )
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "worker-1.json"
            for changes in malformed:
                with self.subTest(changes=changes):
                    crew.atomic_write(path, json.dumps(self.record(**changes)) + "\n")
                    with self.assertRaises(crew.CrewError):
                        crew.load_heartbeat(path)

    def test_checkpoint_alias_preserves_heartbeat_cli(self) -> None:
        parser = crew.build_parser()
        common = [
            "--worker", "worker-1", "--target", "demo_symbol", "--base", "a" * 40,
            "--deadline-unix", "1787998500", "--progress", "runner launched",
        ]
        heartbeat = parser.parse_args(["heartbeat", *common])
        checkpoint = parser.parse_args(["checkpoint", *common])
        self.assertIs(heartbeat.function, crew.command_heartbeat)
        self.assertIs(checkpoint.function, crew.command_heartbeat)

    def test_lane_wrapper_seeds_and_teaches_structured_checkpoints(self) -> None:
        wrapper = (TOOLS / "codex_lane.sh").read_text(encoding="utf-8")
        self.assertIn("--attempt-count 0", wrapper)
        self.assertIn("--best-score \"not measured\"", wrapper)
        self.assertIn("--mismatch-class unclassified", wrapper)
        self.assertIn("tools/crew.py checkpoint", wrapper)
        self.assertIn("heartbeat-status --worker $name --json --check", wrapper)

    def test_stale_status_gives_guidance_and_never_kills(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(worker=None, stale_after_minutes=1, check=False, json=False)
            now = dt.datetime(2026, 8, 29, 11, 0, tzinfo=dt.timezone.utc)
            output = io.StringIO()
            with mock.patch.object(crew, "utc_now", return_value=now):
                with mock.patch("os.kill") as kill:
                    with contextlib.redirect_stdout(output):
                        crew.command_heartbeat_status(args, root)
            self.assertIn("crew.py never signals or stops a process", output.getvalue())
            kill.assert_not_called()

    def test_stale_check_is_enforcing_but_still_non_destructive(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(worker=None, stale_after_minutes=1, check=True, json=False)
            now = dt.datetime(2026, 8, 29, 11, 0, tzinfo=dt.timezone.utc)
            with mock.patch.object(crew, "utc_now", return_value=now):
                with mock.patch("os.kill") as kill:
                    with contextlib.redirect_stdout(io.StringIO()):
                        with self.assertRaisesRegex(crew.CrewError, "stale heartbeat"):
                            crew.command_heartbeat_status(args, root)
            kill.assert_not_called()


if __name__ == "__main__":
    unittest.main()
