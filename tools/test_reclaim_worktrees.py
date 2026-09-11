#!/usr/bin/env python3
"""Safety tests for worktree reclamation.

This tool deletes directories, so every test here is about a case where it must
*refuse*. The removal path itself is one `shutil.rmtree`; the judgement is the
part worth covering.
"""
import pathlib
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import reclaim_worktrees as rw  # noqa: E402


class RepoFixture:
    """A real git repo with a real worktree; the tool shells out to git."""

    def __init__(self, stack):
        self.root = pathlib.Path(stack.enter_context(tempfile.TemporaryDirectory()))
        self.repo = self.root / "main"
        self.repo.mkdir()
        self._git("init", "-q", "-b", "main")
        self._git("config", "user.email", "t@example.invalid")
        self._git("config", "user.name", "t")
        # The real repo gitignores the Codex runner files, which is why the
        # tool can remove a finished Codex lane at all -- an untracked file
        # would read as uncommitted work and it would refuse. The fixture has
        # to carry that or it tests a repo that does not exist.
        (self.repo / ".gitignore").write_text(".codex-*\n")
        (self.repo / "f.txt").write_text("one\n")
        self._git("add", "-A")
        self._git("commit", "-qm", "base")
        self._git("branch", "integration")

    def _git(self, *a, cwd=None):
        return subprocess.run(["git", *a], cwd=cwd or self.repo,
                              capture_output=True, text=True)

    def worktree(self, name, *, commits=0, dirty=False):
        path = self.root / f"mickey-lane-{name}"
        self._git("worktree", "add", "-q", "-b", f"lane/{name}", str(path), "integration")
        for i in range(commits):
            (path / f"c{i}.txt").write_text("x\n")
            self._git("add", "-A", cwd=path)
            self._git("commit", "-qm", f"c{i}", cwd=path)
        if dirty:
            (path / "f.txt").write_text("changed\n")
        return path


class SafetyTests(unittest.TestCase):
    def setUp(self):
        import contextlib
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.fx = RepoFixture(self.stack)
        self._root = rw.ROOT
        rw.ROOT = self.fx.repo
        self.addCleanup(lambda: setattr(rw, "ROOT", self._root))

    def test_a_merged_clean_worktree_is_removable(self) -> None:
        path = self.fx.worktree("clean")
        ok, why = rw.status(path, "integration", set())
        self.assertTrue(ok, why)

    def test_uncommitted_changes_block_removal(self) -> None:
        """The one case that would destroy work."""
        path = self.fx.worktree("dirty", dirty=True)
        ok, why = rw.status(path, "integration", set())
        self.assertFalse(ok)
        self.assertIn("uncommitted", why)

    def test_unmerged_commits_block_removal(self) -> None:
        path = self.fx.worktree("ahead", commits=2)
        ok, why = rw.status(path, "integration", set())
        self.assertFalse(ok)
        self.assertIn("not in integration", why)

    def test_a_running_codex_worker_blocks_removal(self) -> None:
        """A run log with no exit status means it is still being written to."""
        path = self.fx.worktree("running")
        (path / ".codex-run.log").write_text("working\n")
        ok, why = rw.status(path, "integration", set())
        self.assertFalse(ok)
        self.assertIn("still running", why)

    def test_a_finished_codex_worker_does_not_block(self) -> None:
        path = self.fx.worktree("finished")
        (path / ".codex-run.log").write_text("done\n")
        (path / ".codex-status").write_text("0\n")
        ok, _ = rw.status(path, "integration", set())
        self.assertTrue(ok)

    def test_an_excluded_name_is_never_removable(self) -> None:
        """In-process agents leave no filesystem trace, so the caller names them."""
        path = self.fx.worktree("agent")
        ok, why = rw.status(path, "integration", {"agent"})
        self.assertFalse(ok)
        self.assertIn("excluded", why)

    def test_the_lane_name_strips_prefix_and_noindex_suffix(self) -> None:
        self.assertEqual(rw.lane_name(pathlib.Path("/x/mickey-lane-w3-low.noindex")), "w3-low")
        self.assertEqual(rw.lane_name(pathlib.Path("/x/mickey-lane-w3-low")), "w3-low")


if __name__ == "__main__":
    unittest.main()
