#!/usr/bin/env python3

from __future__ import annotations

from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import integration_base as ib  # noqa: E402


class IntegrationBaseTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.repo = Path(self.temporary.name)
        self.git("init", "-q", "-b", ib.LOCAL_BASE)
        self.git("config", "user.email", "test@example.invalid")
        self.git("config", "user.name", "Test")
        self.commit("base")

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def git(self, *args: str) -> str:
        return subprocess.run(
            ["git", "-C", str(self.repo), *args],
            check=True,
            text=True,
            stdout=subprocess.PIPE,
        ).stdout.strip()

    def commit(self, message: str) -> str:
        self.git("commit", "-q", "--allow-empty", "-m", message)
        return self.git("rev-parse", "HEAD")

    def remote_ref(self, commit: str) -> None:
        self.git("update-ref", f"refs/remotes/{ib.REMOTE_BASE}", commit)

    def test_local_only_and_equal_refs_choose_local(self) -> None:
        self.assertEqual(ib.LOCAL_BASE, ib.resolve(self.repo))
        self.remote_ref(self.git("rev-parse", "HEAD"))
        self.assertEqual(ib.LOCAL_BASE, ib.resolve(self.repo))

    def test_newer_remote_ref_wins_over_stale_local_branch(self) -> None:
        base = self.git("rev-parse", "HEAD")
        newer = self.commit("remote newer")
        self.remote_ref(newer)
        self.git("reset", "-q", "--hard", base)
        self.assertEqual(ib.REMOTE_BASE, ib.resolve(self.repo))

    def test_newer_local_ref_wins_over_stale_remote(self) -> None:
        self.remote_ref(self.git("rev-parse", "HEAD"))
        self.commit("local newer")
        self.assertEqual(ib.LOCAL_BASE, ib.resolve(self.repo))

    def test_divergence_fails_closed(self) -> None:
        base = self.git("rev-parse", "HEAD")
        local = self.commit("local")
        self.git("reset", "-q", "--hard", base)
        remote = self.commit("remote")
        self.remote_ref(remote)
        self.git("update-ref", f"refs/heads/{ib.LOCAL_BASE}", local)
        with self.assertRaisesRegex(ib.IntegrationBaseError, "diverged"):
            ib.resolve(self.repo)

    def test_repository_without_campaign_refs_uses_head(self) -> None:
        self.git("branch", "-m", "master")
        self.assertEqual("HEAD", ib.resolve(self.repo))


if __name__ == "__main__":
    unittest.main()
