#!/usr/bin/env python3
"""Unit tests for the stack-slot census decoder.

Synthetic instruction words only: hand-built encodings that exercise the
decoder, never anything read from the ROM.
"""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import frame_census as fc  # noqa: E402
import nm_ranking as nr  # noqa: E402

SP = 29


def mem(op, base, rt, offset):
    return (op << 26) | (base << 21) | (rt << 16) | (offset & 0xFFFF)


class DecodeTests(unittest.TestCase):
    def test_a_word_store_to_sp_is_a_slot(self) -> None:
        out = fc.census([mem(0x2B, SP, 4, 0x40)])
        self.assertEqual(list(out["slots"]), [0x40])
        self.assertEqual(out["slots"][0x40]["store"], 1)
        self.assertEqual(out["slots"][0x40]["widths"], [4])

    def test_accesses_through_another_base_are_ignored(self) -> None:
        """Only $sp-relative traffic is a frame slot; a struct field is not."""
        out = fc.census([mem(0x23, 16, 4, 0x40)])
        self.assertEqual(out["slots"], {})

    def test_width_comes_from_the_opcode(self) -> None:
        out = fc.census([mem(0x21, SP, 4, 0x10),   # lh
                         mem(0x20, SP, 5, 0x14),   # lb
                         mem(0x3D, SP, 6, 0x18)])  # sdc1
        self.assertEqual(out["slots"][0x10]["widths"], [2])
        self.assertEqual(out["slots"][0x14]["widths"], [1])
        self.assertEqual(out["slots"][0x18]["widths"], [8])

    def test_float_slots_are_counted_like_any_other(self) -> None:
        out = fc.census([mem(0x31, SP, 4, 0x20), mem(0x39, SP, 4, 0x20)])
        self.assertEqual(out["slots"][0x20]["load"], 1)
        self.assertEqual(out["slots"][0x20]["store"], 1)

    def test_taking_a_slot_address_counts_without_a_width(self) -> None:
        """addiu rt,$sp,imm is an escaping local -- traffic with no access."""
        out = fc.census([mem(fc.ADDIU, SP, 4, 0x30)])
        self.assertEqual(out["slots"][0x30]["address_taken"], 1)
        self.assertEqual(out["slots"][0x30]["widths"], [])
        self.assertEqual(out["slots"][0x30]["load"], 0)

    def test_the_prologue_adjustment_is_the_frame_not_a_slot(self) -> None:
        out = fc.census([mem(fc.ADDIU, SP, SP, -0x90),
                         mem(0x2B, SP, 31, 0x8C)])
        self.assertEqual(out["frame_size"], 0x90)
        self.assertEqual(list(out["slots"]), [0x8C])

    def test_repeated_access_accumulates(self) -> None:
        out = fc.census([mem(0x23, SP, 4, 0x40)] * 3 + [mem(0x2B, SP, 4, 0x40)])
        self.assertEqual(out["slots"][0x40]["load"], 3)
        self.assertEqual(out["slots"][0x40]["store"], 1)


class CompareTests(unittest.TestCase):
    def streams(self, base, target):
        return nr.WordStreams(base_words=base, target_words=target,
                              base_reloc={}, target_reloc={},
                              base_size=len(base) * 4,
                              target_size=len(target) * 4)

    def test_a_slot_only_one_side_uses_is_reported(self) -> None:
        """The finding that closed two functions: a home one side does not have."""
        ours = [mem(0x2B, SP, 4, 0x40), mem(0x2B, SP, 5, 0xF8)]
        theirs = [mem(0x2B, SP, 4, 0x40), mem(0x2B, SP, 5, 0x2C)]
        out = fc.compare(self.streams(ours, theirs))
        self.assertEqual([s["offset"] for s in out["only_candidate"]], [0xF8])
        self.assertEqual([s["offset"] for s in out["only_target"]], [0x2C])

    def test_identical_frames_report_no_difference(self) -> None:
        words = [mem(fc.ADDIU, SP, SP, -0x40), mem(0x2B, SP, 4, 0x20)]
        out = fc.compare(self.streams(words, list(words)))
        self.assertEqual(out["only_candidate"], [])
        self.assertEqual(out["only_target"], [])
        self.assertEqual(out["shared_differing"], [])
        self.assertEqual(out["candidate_frame"], out["target_frame"])

    def test_the_same_slot_with_different_traffic_is_reported(self) -> None:
        ours = [mem(0x23, SP, 4, 0x40), mem(0x23, SP, 4, 0x40)]
        theirs = [mem(0x23, SP, 4, 0x40)]
        out = fc.compare(self.streams(ours, theirs))
        self.assertEqual(len(out["shared_differing"]), 1)
        self.assertEqual(out["shared_differing"][0]["ours"]["load"], 2)
        self.assertEqual(out["shared_differing"][0]["theirs"]["load"], 1)

    def test_the_ladder_is_ordered_from_the_frame_top_down(self) -> None:
        """L99 reads homes as a ladder, so the order is the useful output."""
        words = [mem(0x2B, SP, 4, 0x10), mem(0x2B, SP, 5, 0x40),
                 mem(0x2B, SP, 6, 0x24)]
        out = fc.compare(self.streams(words, words))
        self.assertEqual(out["target_ladder"], [0x40, 0x24, 0x10])


if __name__ == "__main__":
    unittest.main()
