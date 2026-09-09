#!/usr/bin/env python3
"""Unit tests for the tracked-path sanity check."""

import unittest

import check_paths


class OffenderTests(unittest.TestCase):
    def why(self, path):
        found = check_paths.offenders([path])
        return found[0][1] if found else None

    def test_a_path_that_is_only_a_space_is_caught(self):
        # The case that actually happened: an empty file named " " that every
        # other gate passed, surfacing only as a GitHub listing omission.
        self.assertEqual(self.why(" "), "path component is only whitespace")

    def test_a_trailing_space_is_caught(self):
        self.assertIsNotNone(self.why("src/main/thing.c "))

    def test_a_control_character_is_caught(self):
        self.assertIsNotNone(self.why("docs/note\tname.md"))

    def test_editor_and_merge_leftovers_are_caught(self):
        for name in ("src/a.c.swp", "src/a.c.orig", "src/a.c.rej"):
            self.assertIsNotNone(self.why(name), name)

    def test_build_artifacts_are_caught(self):
        self.assertIsNotNone(self.why("core"))
        self.assertIsNotNone(self.why("a.out"))


class NonOffenderTests(unittest.TestCase):
    """A false positive blocks a commit, so the ordinary tree must be clean."""

    def test_real_repository_paths_are_accepted(self):
        ordinary = [
            "src/main/models_5B300.c",
            "docs/matching-triage-handoffs/func_8005A948.md",
            "config/nonmatching-ranking.us.json",
            "mk/overlays.mk",
            ".github/workflows/scoreboard.yml",
            "tools/check_paths.py",
            "symbol_addrs.us.txt",
            "src/overlays/o101/func_overlay_101_F0003A58_18DF278.c",
            "include/PR/ultratypes.h",
            "README.md",
            ".gitignore",
        ]
        self.assertEqual(check_paths.offenders(ordinary), [])

    def test_a_leading_dot_directory_is_not_a_dot_component(self):
        self.assertEqual(check_paths.offenders([".githooks/pre-commit"]), [])

    def test_a_name_merely_containing_core_is_accepted(self):
        self.assertEqual(check_paths.offenders(["src/main/scorecard.c"]), [])


if __name__ == "__main__":
    unittest.main()
