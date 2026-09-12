#!/usr/bin/env python3
"""Unit tests for the per-address residual map.

Every fixture here is a synthetic MIPS word assembled by hand in the test from
an opcode and register numbers. Nothing is read from the ROM, a baserom, a
build, or a real reconstruction, so none of this needs a toolchain.

The properties worth pinning are the ones the reading depends on:

  1. register extraction agrees with `nm_ranking.instr_reg_mask`'s case split
     -- the mask erases exactly the fields this extracts, so a field the mask
     keeps must not be reported as a register;
  2. a closed cycle is found, reported once, and reported from a canonical
     starting point, because a cycle printed two different ways across two
     runs reads as two different facts;
  3. a mapping with no cycle produces none -- the tool must not manufacture a
     ring phase out of scattered colour;
  4. windowing buckets by target offset, omits empty windows, and the
     cumulative column is monotonic.
"""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import nm_ranking as nr   # noqa: E402
import residual_map as rm  # noqa: E402


def rtype(function: int, rs: int, rt: int, rd: int, shamt: int = 0) -> int:
    return (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | function


def itype(op: int, rs: int, rt: int, imm: int) -> int:
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


class RegisterFields(unittest.TestCase):
    def test_rtype_names_three_registers(self):
        # addu $t1, $t2, $t3  ->  rd=t1(9) rs=t2(10) rt=t3(11)
        word = rtype(0x21, rs=10, rt=11, rd=9)
        self.assertEqual(rm.register_fields(word), [10, 11, 9])

    def test_itype_names_two_registers(self):
        # addiu $t1, $t2, 24
        word = itype(0x09, rs=10, rt=9, imm=24)
        self.assertEqual(rm.register_fields(word), [10, 9])

    def test_jtype_names_none(self):
        for op in (0x02, 0x03):
            self.assertEqual(rm.register_fields(op << 26 | 0x1234), [])

    def test_cop1_names_no_gpr(self):
        # COP1 register fields are float registers, not GPR slots. Decoding
        # them as GPRs would report a float rotation as an integer one.
        word = (0x11 << 26) | (0x10 << 21) | (4 << 16) | (6 << 11)
        self.assertEqual(rm.register_fields(word), [])

    def test_agrees_with_the_masks_case_split(self):
        # Zeroing every field this extracts must leave the mask unchanged:
        # the two functions have to be reading the same layout.
        for word in (rtype(0x21, 10, 11, 9),
                     rtype(0x00, 0, 11, 9, shamt=3),      # sll, shamt kept
                     itype(0x23, 29, 4, 0x40),            # lw
                     itype(0x05, 17, 21, 0x20)):          # bne
            stripped = word
            op = (word >> 26) & 0x3F
            if op == 0x00:
                stripped &= ~((0x1F << 21) | (0x1F << 16) | (0x1F << 11))
            elif op not in (0x02, 0x03, 0x11):
                stripped &= ~((0x1F << 21) | (0x1F << 16))
            self.assertEqual(nr.instr_reg_mask(word),
                             nr.instr_reg_mask(stripped),
                             f"0x{word:08X}")

    def test_substitutions_report_only_differing_slots(self):
        ours = itype(0x09, rs=17, rt=17, imm=1)    # addiu $s1, $s1, 1
        theirs = itype(0x09, rs=23, rt=23, imm=1)  # addiu $s7, $s7, 1
        self.assertEqual(rm.substitutions(ours, theirs),
                         [("s1", "s7"), ("s1", "s7")])
        self.assertEqual(rm.substitutions(ours, ours), [])


class Cycles(unittest.TestCase):
    def test_closed_cycle_is_found_once_and_canonically(self):
        mapping = {"t1": "t5", "t5": "t2", "t2": "t6", "t6": "t1"}
        found = rm.cycles(mapping)
        self.assertEqual(len(found), 1)
        # canonical: starts at the alphabetically first member
        self.assertEqual(found[0][0], "t1")
        self.assertEqual(set(found[0]), {"t1", "t5", "t2", "t6"})

    def test_two_disjoint_cycles(self):
        mapping = {"s2": "s3", "s3": "s2", "t4": "t5", "t5": "t4"}
        self.assertEqual(len(rm.cycles(mapping)), 2)

    def test_no_cycle_in_a_scattered_mapping(self):
        # s1 -> s0 -> s2 -> s4 with nothing returning: per-web colour, not a
        # ring phase. The tool must say so.
        self.assertEqual(rm.cycles({"s1": "s0", "s0": "s2", "s2": "s4"}), [])

    def test_self_map_is_not_a_cycle(self):
        self.assertEqual(rm.cycles({"s1": "s1"}), [])

    def test_dominant_mapping_breaks_ties_deterministically(self):
        pairs = {("s1", "s7"): 4, ("s1", "s0"): 4, ("s2", "s3"): 1}
        first = rm.dominant_mapping(pairs)
        for _ in range(5):
            self.assertEqual(rm.dominant_mapping(pairs), first)
        self.assertIn(first["s1"], ("s0", "s7"))


class Windows(unittest.TestCase):
    def test_buckets_by_offset_and_omits_empty_windows(self):
        rows = [(0x10, "naming"), (0x1F0, "structural"),
                (0x600, "naming"), (0x604, "immediate")]
        got = rm.windows(rows, 0x200)
        self.assertEqual([w["offset"] for w in got], [0x0, 0x600])
        self.assertEqual(got[0]["total"], 2)
        self.assertEqual(got[0]["naming"], 1)
        self.assertEqual(got[0]["structural"], 1)
        self.assertEqual(got[1]["immediate"], 1)

    def test_cumulative_is_monotonic_and_totals(self):
        rows = [(i * 0x100, "naming") for i in range(8)]
        got = rm.windows(rows, 0x200)
        self.assertEqual([w["cumulative"] for w in got], [2, 4, 6, 8])
        self.assertEqual(got[-1]["cumulative"], len(rows))

    def test_empty_input(self):
        self.assertEqual(rm.windows([], 0x200), [])


class Rendering(unittest.TestCase):
    def test_render_reports_the_cycle_inside_its_window(self):
        data = {
            "symbol": "func_test", "file": "src/test.c", "size_bytes": 0x800,
            "size_delta": 0, "positional_masked": 4, "aligned_exact": 500,
            "rows": [(0x0, "naming"), (0x600, "naming"), (0x604, "naming")],
            "pairs": [(0x0, ("s1", "s2")),
                      (0x600, ("t1", "t5")), (0x604, ("t5", "t1"))],
            "insertions": [0x400], "deletions": [0x410],
        }
        text = rm.render(data, 0x200, 0, 1 << 30)
        self.assertIn("candidate-only words: +0x400", text)
        self.assertIn("target-only words:    +0x410", text)
        self.assertIn("cycle: t1 -> t5 -> t1", text)
        # the window with no cycle must not claim one: the census line for
        # +0x00000 must not be followed by a cycle line.
        census = text.split("per-window register substitutions")[1]
        lines = [l for l in census.split("\n") if l.strip()]
        zero = next(i for i, l in enumerate(lines) if l.startswith("   +0x00000"))
        self.assertNotIn("cycle:", lines[zero + 1])

    def test_lo_hi_restricts_every_section(self):
        data = {
            "symbol": "f", "file": "src/f.c", "size_bytes": 0x800,
            "size_delta": 0, "positional_masked": 2, "aligned_exact": 10,
            "rows": [(0x0, "naming"), (0x600, "naming")],
            "pairs": [(0x0, ("s1", "s2")), (0x600, ("t1", "t5"))],
            "insertions": [0x4], "deletions": [0x604],
        }
        text = rm.render(data, 0x200, 0x500, 0x700)
        self.assertIn("+0x00600", text)
        self.assertNotIn("+0x00000", text)
        self.assertNotIn("candidate-only words:", text)
        self.assertIn("target-only words:    +0x604", text)
        self.assertIn("excludes 0 candidate-only and 1 target-only", text)


class AlignedDeltaTests(unittest.TestCase):
    """`compare` is the reading that positional blast radius gets wrong.

    On overlay 58 two windows carried 81 POSITIONAL differing words and 22
    aligned rows, because each is bracketed by a one-word insertion and every
    word after it mismatches by position while aligning perfectly. A view that
    moves only when content moves is the one that can rank a window.
    """

    def map(self, rows, insertions=(), deletions=(), object="obj"):
        return {"symbol": "f", "file": "a.c", "object": object,
                "rows": list(rows), "pairs": [],
                "insertions": list(insertions), "deletions": list(deletions)}

    def test_a_window_whose_content_did_not_move_says_unmoved(self):
        rows = [(0x00, "naming"), (0x04, "naming")]
        text = rm.compare(self.map(rows), self.map(rows), 0x80, 0, 1 << 30)
        self.assertIn("unmoved", text)
        self.assertIn("+0", text)

    def test_naming_rows_removed_read_as_a_negative_delta(self):
        before = self.map([(0x00, "naming"), (0x04, "naming"), (0x08, "naming")])
        after = self.map([(0x00, "naming")])
        text = rm.compare(before, after, 0x80, 0, 1 << 30)
        self.assertIn("3->1", text)
        self.assertIn("-2", text)

    def test_the_buckets_are_reported_separately(self):
        before = self.map([(0x00, "naming"), (0x04, "structural")])
        after = self.map([(0x00, "immediate")])
        text = rm.compare(before, after, 0x80, 0, 1 << 30)
        self.assertIn("1->0", text)   # naming
        self.assertIn("0->1", text)   # immediate
        self.assertIn("1->0", text)   # structural

    def test_windows_are_split_at_the_requested_width(self):
        before = self.map([(0x00, "naming"), (0x80, "naming")])
        after = self.map([(0x00, "naming")])
        text = rm.compare(before, after, 0x80, 0, 1 << 30)
        self.assertIn("+0x00000", text)
        self.assertIn("+0x00080", text)

    def test_the_range_filter_excludes_windows_outside_it(self):
        before = self.map([(0x00, "naming"), (0x200, "naming")])
        after = self.map([(0x00, "naming")])
        text = rm.compare(before, after, 0x80, 0x100, 1 << 30)
        self.assertNotIn("+0x00000", text)
        self.assertIn("+0x00200", text)

    def test_the_total_is_the_sum_of_the_window_deltas(self):
        before = self.map([(0x00, "naming"), (0x80, "naming"), (0x100, "naming")])
        after = self.map([(0x00, "naming")])
        text = rm.compare(before, after, 0x80, 0, 1 << 30)
        self.assertIn("total", text)
        self.assertIn("-2", text.splitlines()[-1])

    def test_an_unchanged_insertion_is_reported_as_still_present(self):
        before = self.map([], insertions=[0xDDC])
        after = self.map([], insertions=[0xDDC])
        text = rm.compare(before, after, 0x80, 0, 1 << 30)
        self.assertIn("candidate-only words unchanged at +0xDDC", text)

    def test_an_insertion_that_moved_is_reported_as_gone_and_new(self):
        before = self.map([], insertions=[0xDDC])
        after = self.map([], insertions=[0xE00])
        text = rm.compare(before, after, 0x80, 0, 1 << 30)
        self.assertIn("gone [3548]", text)
        self.assertIn("new [3584]", text)

    def test_both_object_labels_appear_so_a_delta_is_attributable(self):
        text = rm.compare(self.map([], object="lattice/base/candidate.o"),
                          self.map([], object="lattice/cell-031/candidate.o"),
                          0x80, 0, 1 << 30)
        self.assertIn("lattice/base/candidate.o", text)
        self.assertIn("lattice/cell-031/candidate.o", text)


if __name__ == "__main__":
    unittest.main()
