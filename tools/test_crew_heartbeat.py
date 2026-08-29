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
            "schema": 1,
            "worker": "worker-1",
            "target": "demo_symbol",
            "base": "a" * 40,
            "last_progress": "baseline compared",
            "last_progress_at": "2026-08-29T10:00:00Z",
            "last_commit": "b" * 40,
            "deadline_unix": 1787998500,
            "state": "active",
            "updated_at": "2026-08-29T10:00:00Z",
        }
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

    def test_heartbeat_update_preserves_assignment_and_refreshes_commit(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(
                worker="worker-1", target=None, base=None, deadline_unix=None,
                progress="candidate improved", state="active",
            )
            with mock.patch.object(crew, "run_git", return_value="c" * 40):
                with contextlib.redirect_stdout(io.StringIO()):
                    crew.command_heartbeat(args, root)
            updated = crew.load_heartbeat(path)
            self.assertEqual(updated["target"], "demo_symbol")
            self.assertEqual(updated["base"], "a" * 40)
            self.assertEqual(updated["last_commit"], "c" * 40)
            self.assertEqual(updated["last_progress"], "candidate improved")

    def test_stale_status_gives_guidance_and_never_kills(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(worker=None, stale_after_minutes=1, check=False)
            now = dt.datetime(2026, 8, 29, 11, 0, tzinfo=dt.timezone.utc)
            output = io.StringIO()
            with mock.patch.object(crew, "utc_now", return_value=now):
                with mock.patch("os.kill") as kill:
                    with contextlib.redirect_stdout(output):
                        crew.command_heartbeat_status(args, root)
            self.assertIn("No process was stopped automatically", output.getvalue())
            kill.assert_not_called()

    def test_stale_check_is_enforcing_but_still_non_destructive(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "heartbeats" / "worker-1.json"
            crew.atomic_write(path, json.dumps(self.record()) + "\n")
            args = argparse.Namespace(worker=None, stale_after_minutes=1, check=True)
            now = dt.datetime(2026, 8, 29, 11, 0, tzinfo=dt.timezone.utc)
            with mock.patch.object(crew, "utc_now", return_value=now):
                with mock.patch("os.kill") as kill:
                    with contextlib.redirect_stdout(io.StringIO()):
                        with self.assertRaisesRegex(crew.CrewError, "stale heartbeat"):
                            crew.command_heartbeat_status(args, root)
            kill.assert_not_called()


if __name__ == "__main__":
    unittest.main()
