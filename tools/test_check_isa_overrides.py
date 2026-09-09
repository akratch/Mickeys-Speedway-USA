#!/usr/bin/env python3
"""Unit tests for the per-object ISA override invariant."""

import unittest

import check_isa_overrides as guard


class BranchLikelyTests(unittest.TestCase):
    def test_the_branch_likely_set_is_the_mips2_addition(self):
        # These are exactly the forms IDO cannot emit below -mips2, which is
        # what makes a -mips1 override falsifiable from the target alone.
        for name in ("beql", "bnel", "bc1tl", "bc1fl", "bgezl"):
            self.assertIn(name, guard.BRANCH_LIKELY)

    def test_ordinary_branches_are_not_in_the_set(self):
        for name in ("beq", "bne", "bc1t", "bgez", "j", "jal", "nop"):
            self.assertNotIn(name, guard.BRANCH_LIKELY)


class OverridePatternTests(unittest.TestCase):
    def match(self, line):
        return guard.OVERRIDE.match(line)

    def test_a_per_object_override_is_recognised(self):
        hit = self.match(
            "$(BUILD_DIR)/$(SRC_DIR)/overlays/o058/f.c.o: MIPSISET := -mips1 -32"
        )
        self.assertIsNotNone(hit)
        self.assertEqual(hit.group("obj"), "overlays/o058/f")
        self.assertEqual(hit.group("isa"), "-mips1")

    def test_a_group_default_is_matched_but_filtered_by_the_wildcard(self):
        # `overlays/%.c.o` is the group default, not a per-object claim; the
        # caller drops any object containing '%'.
        hit = self.match("$(BUILD_DIR)/$(SRC_DIR)/overlays/%.c.o: MIPSISET := -mips2 -32")
        self.assertIsNotNone(hit)
        self.assertIn("%", hit.group("obj"))

    def test_an_unrelated_variable_is_not_matched(self):
        self.assertIsNone(
            self.match("$(BUILD_DIR)/$(SRC_DIR)/main/a.c.o: CFLAGS += -Wab,-r4300_mul")
        )


class RowParsingTests(unittest.TestCase):
    def test_a_disassembly_row_yields_its_mnemonic(self):
        hit = guard.ROW.match("    /* 1234 80001234 5400FFFF */  beql      $a0, $a1, .L1")
        self.assertEqual(hit.group(1), "beql")

    def test_a_label_line_is_not_a_row(self):
        self.assertIsNone(guard.ROW.match("glabel someFunction"))


if __name__ == "__main__":
    unittest.main()
