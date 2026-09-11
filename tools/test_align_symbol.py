#!/usr/bin/env python3
"""Unit tests for the shape aligner's bucketing and insertion reporting.

Synthetic instruction words only: these are hand-built encodings that exercise
the three buckets, never anything read from the ROM.
"""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import align_symbol as al  # noqa: E402
import nm_ranking as nr  # noqa: E402


def streams(base, target, base_reloc=None, target_reloc=None):
    return nr.WordStreams(
        base_words=base, target_words=target,
        base_reloc=base_reloc or {}, target_reloc=target_reloc or {},
        base_size=len(base) * 4, target_size=len(target) * 4)


ADDU_T = (0x00 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | 0x21
ADDU_S = (0x00 << 26) | (17 << 21) | (18 << 16) | (16 << 11) | 0x21
SUBU_T = (0x00 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | 0x23
NOP = 0x00000000


class BucketTests(unittest.TestCase):
    def test_identical_streams_are_all_byte_exact(self) -> None:
        out = al.align(streams([ADDU_T, SUBU_T], [ADDU_T, SUBU_T]))
        self.assertEqual(out["aligned_exact"], 2)
        self.assertEqual(out["aligned_register_naming"], 0)
        self.assertEqual(out["aligned_really_different"], 0)

    def test_a_register_swap_is_naming_not_structure(self) -> None:
        out = al.align(streams([ADDU_S, SUBU_T], [ADDU_T, SUBU_T]))
        self.assertEqual(out["aligned_register_naming"], 1)
        self.assertEqual(out["aligned_really_different"], 0)
        self.assertEqual(out["first_naming_offset"], 0)

    def test_a_different_opcode_is_structure_not_naming(self) -> None:
        out = al.align(streams([SUBU_T, SUBU_T], [ADDU_T, SUBU_T]))
        self.assertEqual(out["aligned_register_naming"], 0)
        self.assertEqual(out["aligned_really_different"], 1)

    def test_an_extra_candidate_instruction_is_reported_with_its_offset(self) -> None:
        out = al.align(streams([ADDU_T, NOP, SUBU_T], [ADDU_T, SUBU_T]))
        self.assertEqual(out["insertions"], [{"candidate_offset": 4, "words": 1}])
        self.assertEqual(out["deletions"], [])
        # the surviving pair still aligns, which is the whole point
        self.assertEqual(out["aligned_exact"], 2)

    def test_a_missing_candidate_instruction_is_reported_against_the_target(self) -> None:
        out = al.align(streams([ADDU_T, SUBU_T], [ADDU_T, NOP, SUBU_T]))
        self.assertEqual(out["deletions"], [{"target_offset": 4, "words": 1}])
        self.assertEqual(out["insertions"], [])
        self.assertEqual(out["aligned_exact"], 2)

    def test_one_insertion_does_not_smear_the_rest_of_the_stream(self) -> None:
        """The reason this tool exists: a positional diff would call every
        word after the insertion different."""
        tail = [ADDU_T, SUBU_T, ADDU_S, SUBU_T, ADDU_T]
        out = al.align(streams([NOP] + tail, tail))
        self.assertEqual(out["insertions"], [{"candidate_offset": 0, "words": 1}])
        self.assertEqual(out["aligned_exact"], len(tail))
        self.assertEqual(out["aligned_register_naming"], 0)


class SubstitutionTests(unittest.TestCase):
    """The regression that made the first implementation unusable.

    difflib has no substitution operation, so one changed instruction inside an
    otherwise identical stream came back as an insert at one offset and a
    delete at a far-away one -- double-counted, and pointing at a location
    where nothing was missing. That report is what a lane acts on for a size
    mismatch, so it has to be right.
    """

    def test_one_changed_instruction_mid_stream_is_a_single_substitution(self) -> None:
        base = [ADDU_T] * 20
        target = list(base)
        target[10] = SUBU_T
        out = al.align(streams(base, target))
        self.assertEqual(out["aligned_really_different"], 1)
        self.assertEqual(out["insertions"], [])
        self.assertEqual(out["deletions"], [])
        self.assertEqual(out["aligned_exact"], 19)
        self.assertEqual(out["first_different_offset"], 40)

    def test_a_run_of_extra_instructions_is_reported_as_one_span(self) -> None:
        tail = [ADDU_T, SUBU_T] * 6
        out = al.align(streams([ADDU_T] + [NOP, NOP, NOP] + tail, [ADDU_T] + tail))
        self.assertEqual(out["insertions"], [{"candidate_offset": 4, "words": 3}])
        self.assertEqual(out["deletions"], [])

    def test_the_net_shift_equals_the_size_difference(self) -> None:
        base = [ADDU_T, NOP, SUBU_T, ADDU_S, SUBU_T]
        target = [ADDU_T, SUBU_T, ADDU_S, SUBU_T]
        out = al.align(streams(base, target))
        inserted = sum(s["words"] for s in out["insertions"])
        deleted = sum(s["words"] for s in out["deletions"])
        self.assertEqual(inserted - deleted, len(base) - len(target))


class RelocationMaskTests(unittest.TestCase):
    def test_linker_controlled_bits_are_masked_from_either_surface(self) -> None:
        """One side carrying a relocation the other lacks must still compare
        equal once the payload is masked -- splat writes a literal where it has
        no symbol."""
        lui_a = (0x0F << 26) | (2 << 16) | 0x1234
        lui_b = (0x0F << 26) | (2 << 16) | 0xABCD
        reloc = {0: ("R_MIPS_HI16", "some_global")}
        out = al.align(streams([lui_a], [lui_b], base_reloc=reloc))
        self.assertEqual(out["aligned_exact"], 1)
        self.assertEqual(out["aligned_register_naming"], 0)

    def test_an_unrelocated_immediate_difference_still_counts(self) -> None:
        addiu_a = (0x09 << 26) | (9 << 21) | (8 << 16) | 0x0010
        addiu_b = (0x09 << 26) | (9 << 21) | (8 << 16) | 0x0020
        out = al.align(streams([addiu_a], [addiu_b]))
        self.assertEqual(out["aligned_exact"], 0)
        self.assertEqual(out["aligned_really_different"], 1)


if __name__ == "__main__":
    unittest.main()
