#!/usr/bin/env python3
"""Guards on what a conflict resolution is allowed to produce.

Every case here is a resolution that left no conflict marker, so the tree
looked clean and the failure landed several gates downstream.
"""
import tempfile
import unittest
from pathlib import Path

import resolve_lane_conflicts as rlc


class ValidateResolved(unittest.TestCase):
    def write(self, name, text):
        path = Path(self.tmp.name) / name
        path.write_text(text)
        return str(path)

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)

    def test_two_json_records_spliced_without_a_delimiter_are_caught(self):
        """The measured incident: keep-both concatenated two records."""
        path = self.write("pins.json", '{"a": 1}{"b": 2}')
        message = rlc.validate_resolved(path)
        self.assertIsNotNone(message)
        self.assertIn("INVALID JSON", message)
        self.assertIn("pins.json", message)

    def test_a_function_tail_without_its_head_is_caught(self):
        """The measured incident: a stray brace closing nothing."""
        path = self.write("tool.py", 'def f():\n    return 1\n\n'
                                     '        "python": digest(),\n    }\n')
        message = rlc.validate_resolved(path)
        self.assertIsNotNone(message)
        self.assertIn("INVALID PYTHON", message)
        self.assertIn("line", message)

    def test_the_message_names_the_line_so_the_cause_is_findable(self):
        path = self.write("tool.py", "ok = 1\n" * 40 + "def broken(:\n")
        message = rlc.validate_resolved(path)
        self.assertIn("line 41", message)

    def test_valid_files_of_both_kinds_pass(self):
        self.assertIsNone(rlc.validate_resolved(self.write("a.json", '{"a": [1, 2]}')))
        self.assertIsNone(rlc.validate_resolved(
            self.write("b.py", "import os\n\n\ndef f(x):\n    return {'k': x}\n")))

    def test_unknown_suffixes_are_not_claimed_to_be_checked(self):
        """A .c file is not parse-checked here; do not imply otherwise."""
        self.assertIsNone(rlc.validate_resolved(self.write("x.c", "void f( {")))

    def test_an_undecodable_file_does_not_crash_the_resolver(self):
        path = Path(self.tmp.name) / "bin.py"
        path.write_bytes(b"\xff\xfe def f(:\n")
        self.assertIsNotNone(rlc.validate_resolved(str(path)))


if __name__ == "__main__":
    unittest.main()
