#!/usr/bin/env python3
"""Regression tests for comment-only merge conflict resolution."""

from pathlib import Path
import sys
import tempfile
import unittest


TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import resolve_comment_hunks as resolver  # noqa: E402


class ResolveCommentHunksTests(unittest.TestCase):
    def resolve(self, text: str) -> tuple[int, str]:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "source.c"
            path.write_text(text)
            remaining = resolver.resolve(str(path))
            return remaining, path.read_text()

    def test_preserves_distinct_eof_plateau_blocks(self) -> None:
        conflict = """int value;
<<<<<<< HEAD
/* PLATEAU-HANDOFF:first:start
 * score: 9/10 words
 * PLATEAU-HANDOFF:first:end
=======
/* PLATEAU-HANDOFF:second:start
 * score: 8/10 words
 * PLATEAU-HANDOFF:second:end
>>>>>>> lane/second
 */
"""
        remaining, resolved = self.resolve(conflict)
        self.assertEqual(remaining, 0)
        self.assertNotIn("<<<<<<<", resolved)
        self.assertEqual(resolved.count("/* PLATEAU-HANDOFF:"), 2)
        self.assertEqual(resolved.count(" */"), 2)
        self.assertLess(resolved.index("first:start"), resolved.index("second:start"))

    def test_leaves_same_symbol_plateau_conflict_for_review(self) -> None:
        conflict = """<<<<<<< HEAD
/* PLATEAU-HANDOFF:same:start
 * score: 8/10 words
 * PLATEAU-HANDOFF:same:end
=======
/* PLATEAU-HANDOFF:same:start
 * score: 9/10 words
 * PLATEAU-HANDOFF:same:end
>>>>>>> lane/same
 */
"""
        remaining, resolved = self.resolve(conflict)
        self.assertEqual(remaining, 1)
        self.assertIn("<<<<<<<", resolved)


if __name__ == "__main__":
    unittest.main()
