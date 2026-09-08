#!/usr/bin/env python3
"""Unit tests for the multiply-scheduler target scan."""

import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

import mul_scheduler_scan as scan


def listing(*mnemonics: str) -> str:
    """A splat-shaped disassembly listing carrying these mnemonics."""
    rows = ["glabel some_function"]
    for i, mnemonic in enumerate(mnemonics):
        rows.append(f"    /* {i:05X} 8000{i:04X} 00000000 */  {mnemonic}    $f0, $f2")
    return "\n".join(rows) + "\n"


class ScanTests(unittest.TestCase):
    def scan(self, text: str):
        with TemporaryDirectory() as tmp:
            path = Path(tmp) / "f.s"
            path.write_text(text)
            return scan.scan(path)

    def test_separated_pair_counts_as_a_pair_and_a_nop(self):
        self.assertEqual(self.scan(listing("mul.s", "nop", "mul.s")), (1, 1))

    def test_bare_adjacent_pair_counts_as_a_pair_with_no_nop(self):
        # The discriminating case: this is what a unit compiled *without*
        # the flag looks like. No TU in this ROM shows it.
        self.assertEqual(self.scan(listing("mul.s", "mul.s")), (1, 0))

    def test_multiplies_far_apart_are_not_a_pair(self):
        self.assertEqual(
            self.scan(listing("mul.s", "add.s", "sub.s", "mul.s")), (0, 0)
        )

    def test_a_lone_multiply_is_not_a_pair(self):
        self.assertEqual(self.scan(listing("mul.s")), (0, 0))

    def test_trailing_nop_without_a_second_multiply_is_not_a_pair(self):
        self.assertEqual(self.scan(listing("mul.s", "nop")), (0, 0))

    def test_a_run_of_three_counts_each_adjacent_pair(self):
        self.assertEqual(
            self.scan(listing("mul.s", "nop", "mul.s", "nop", "mul.s")), (2, 2)
        )

    def test_double_precision_multiply_is_not_counted(self):
        # The flag's signature is specifically the single-precision pair.
        self.assertEqual(self.scan(listing("mul.d", "nop", "mul.d")), (0, 0))

    def test_rows_without_the_comment_prefix_are_ignored(self):
        self.assertEqual(self.scan("mul.s $f0, $f2\nmul.s $f0, $f2\n"), (0, 0))


class TranslationUnitTests(unittest.TestCase):
    def unit(self, relative: str) -> str:
        return scan.translation_unit(scan.ASM / relative)

    def test_resident_function_maps_to_its_tu(self):
        self.assertEqual(self.unit("main/shadows/func_80017140.s"), "src/main/shadows.c")

    def test_overlay_function_keeps_its_overlay_directory(self):
        self.assertEqual(
            self.unit("overlays/o079/overlay_079/func_a.s"),
            "src/overlays/o079/overlay_079.c",
        )


class MakefileFlagTests(unittest.TestCase):
    def test_the_scan_agrees_with_the_makefile_about_a_known_flagged_tu(self):
        # objects.c has carried the flag since long before this tool; if the
        # parse breaks, every verdict inverts silently.
        self.assertIn("src/main/objects.c", scan.makefile_flagged())

    def test_an_unflagged_tu_is_not_reported_as_flagged(self):
        self.assertNotIn("src/main/matrix.c", scan.makefile_flagged())


if __name__ == "__main__":
    unittest.main()
