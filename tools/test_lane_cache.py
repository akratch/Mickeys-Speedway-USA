#!/usr/bin/env python3
"""Focused tests for the verified lane bootstrap cache."""

from __future__ import annotations

import hashlib
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import lane_cache as lc  # noqa: E402


class LaneCacheTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="lane-cache-")
        self.root = Path(self.temporary.name) / "repo"
        self.root.mkdir()
        self.git("init", "-q", "-b", "master")
        self.git("config", "user.email", "cache@example.invalid")
        self.git("config", "user.name", "Cache Test")
        rom = b"verified-rom"
        (self.root / "mickey.us.sha1").write_text(hashlib.sha1(rom).hexdigest() + "\n")
        (self.root / "Makefile").write_text(
            "all:\n\t@:\nbuild/mickey.us.elf:\n\t@:\n"
        )
        self.git("add", "Makefile", "mickey.us.sha1")
        self.git("commit", "-q", "-m", "fixture")
        self.commit = self.git("rev-parse", "HEAD").strip()
        self.rom = rom

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def git(self, *args: str) -> str:
        return subprocess.run(
            ["git", *args], cwd=self.root, check=True, text=True,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        ).stdout

    def make_cache(self) -> Path:
        cache = lc._cache_path(self.root, self.commit)
        paths = ["asm", "assets", "build/.splat-stamp", "build/mickey.us.elf", "build/mickey.us.z64"]
        for relative in paths:
            path = cache / relative
            if "." not in path.name:
                path.mkdir(parents=True)
                (path / "item").write_bytes(b"item")
            else:
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(self.rom if relative.endswith(".z64") else b"item")
        (cache / lc.MANIFEST).write_text(json.dumps({
            "schema": lc.SCHEMA,
            "commit": self.commit,
            "rom_sha1": hashlib.sha1(self.rom).hexdigest(),
            "artifacts": paths,
            "proof": "fixture",
        }))
        return cache

    def test_dirty_tracked_tree_cannot_publish(self) -> None:
        (self.root / "Makefile").write_text("dirty\n")
        with self.assertRaisesRegex(lc.CacheError, "not clean"):
            lc._assert_clean(self.root)

    def test_restore_requires_exact_commit_and_empty_destinations(self) -> None:
        cache = self.make_cache()
        restored = lc.restore(self.root)
        self.assertEqual(restored, cache)
        self.assertEqual((self.root / "build/mickey.us.z64").read_bytes(), self.rom)
        with self.assertRaisesRegex(lc.CacheError, "already exist"):
            lc.restore(self.root)

    def test_manifest_rejects_unapproved_paths(self) -> None:
        cache = self.make_cache()
        payload = json.loads((cache / lc.MANIFEST).read_text())
        payload["artifacts"].append("../outside")
        (cache / lc.MANIFEST).write_text(json.dumps(payload))
        with self.assertRaisesRegex(lc.CacheError, "unapproved"):
            lc.restore(self.root)

    def test_build_neutral_descendant_reuses_verified_ancestor(self) -> None:
        cache = self.make_cache()
        (self.root / "docs").mkdir()
        (self.root / "docs" / "note.md").write_text("coordination note\n")
        self.git("add", "docs/note.md")
        self.git("commit", "-q", "-m", "Document coordination")
        self.assertEqual(lc.restore(self.root), cache)

    def test_build_input_change_rejects_ancestor_cache(self) -> None:
        self.make_cache()
        (self.root / "Makefile").write_text(
            "all:\n\t@:\nbuild/mickey.us.elf:\n\t@echo changed\n"
        )
        self.git("add", "Makefile")
        self.git("commit", "-q", "-m", "Change build")
        with self.assertRaisesRegex(
            FileNotFoundError, "no exact or build-compatible"
        ):
            lc.restore(self.root)


if __name__ == "__main__":
    unittest.main()
