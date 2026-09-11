#!/usr/bin/env python3
"""Unit tests for the register-substitution census.

Synthetic encodings only. The value of this tool is telling a ring-phase cycle
apart from scattered colour problems, so that is what the tests pin.
"""
import collections
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import nm_ranking as nr  # noqa: E402
import register_census as rc  # noqa: E402

T6, T7, T8, T9 = 14, 15, 24, 25
S0, S1 = 16, 17


def addu(rd, rs, rt):
    return (0x00 << 26) | (rs << 21) | (rt << 16) | (rd << 11) | 0x21


def streams(base, target):
    return nr.WordStreams(base_words=base, target_words=target,
                          base_reloc={}, target_reloc={},
                          base_size=len(base) * 4, target_size=len(target) * 4)


class CensusTests(unittest.TestCase):
    def test_a_register_swap_is_counted_once_per_field(self) -> None:
        out = rc.census(streams([addu(T6, S0, S1)], [addu(T7, S0, S1)]))
        self.assertEqual(out["sites"], 1)
        self.assertEqual(out["pairs"][("t6", "t7")], 1)

    def test_identical_words_contribute_nothing(self) -> None:
        out = rc.census(streams([addu(T6, S0, S1)], [addu(T6, S0, S1)]))
        self.assertEqual(out["sites"], 0)

    def test_a_different_opcode_is_not_a_substitution(self) -> None:
        subu = (0x00 << 26) | (S0 << 21) | (S1 << 16) | (T6 << 11) | 0x23
        out = rc.census(streams([addu(T6, S0, S1)], [subu]))
        self.assertEqual(out["sites"], 0)

    def test_a_shifted_stream_still_pairs_correctly(self) -> None:
        """The reason this reads the alignment rather than matching indices: a
        size delta shifts the streams, and a positional read would pair
        unrelated instructions and invent substitutions."""
        body = [addu(T6, S0, S1), addu(T7, S0, S1), addu(T8, S0, S1)]
        shifted = [addu(S0, S0, S1)] + body      # one extra instruction in front
        out = rc.census(streams(shifted, body))
        self.assertEqual(out["sites"], 0, "aligned bodies are identical")


class CycleTests(unittest.TestCase):
    def test_a_closed_cycle_is_found(self) -> None:
        pairs = collections.Counter({("t6", "t7"): 9, ("t7", "t8"): 8,
                                     ("t8", "t6"): 7})
        self.assertEqual(rc.cycles(pairs), [["t6", "t7", "t8"]])

    def test_scattered_substitutions_are_not_a_cycle(self) -> None:
        """These are per-web colour questions and must not read as ring phase."""
        pairs = collections.Counter({("t6", "s0"): 4, ("t7", "a1"): 3})
        self.assertEqual(rc.cycles(pairs), [])

    def test_the_dominant_mapping_wins_a_contested_source(self) -> None:
        pairs = collections.Counter({("t6", "t7"): 40, ("t6", "s0"): 2,
                                     ("t7", "t6"): 30})
        self.assertEqual(rc.cycles(pairs), [["t6", "t7"]])

    def test_a_self_mapping_is_not_reported_as_a_cycle(self) -> None:
        self.assertEqual(rc.cycles(collections.Counter({("t6", "t6"): 5})), [])


class WindowTests(unittest.TestCase):
    """Coherence counts fan-out but not where it happens. A census printed a
    clean four-cycle whose dominant mapping hid that one source went three ways;
    offset-resolved, 60% of its pairs fit a single global mapping and six
    windows were needed. Two functions away the same instrument saw 83% and one
    clean shift, so it discriminates -- but only in address order."""

    def test_one_mapping_function_wide_is_a_single_window(self) -> None:
        sites = [(0, "t6", "t7"), (4, "t7", "t6"), (8, "t6", "t7")]
        w = rc.windows(sites)
        self.assertEqual(w["windows"], 1)
        self.assertEqual(w["global_share"], 1.0)

    def test_a_mapping_that_changes_midway_opens_a_window(self) -> None:
        sites = [(0, "t6", "t7"), (4, "t6", "t7"), (0x40, "t6", "t8")]
        w = rc.windows(sites)
        self.assertEqual(w["windows"], 2)
        self.assertEqual(w["boundaries"], [0x40])

    def test_per_iteration_consumption_needs_many_windows(self) -> None:
        sites = []
        for row in range(4):
            base = row * 0x40
            sites += [(base, "t6", "t7"), (base + 4, "t6", "t8")]
        self.assertGreaterEqual(rc.windows(sites)["windows"], 4)

    def test_global_share_counts_sites_not_distinct_pairs(self) -> None:
        sites = [(0, "t6", "t7")] * 9 + [(0x40, "t6", "t9")]
        self.assertAlmostEqual(rc.windows(sites)["global_share"], 0.9)

    def test_no_substitutions_is_one_trivial_window(self) -> None:
        w = rc.windows([])
        self.assertEqual(w["windows"], 1)
        self.assertEqual(w["boundaries"], [])


class CoherenceTests(unittest.TestCase):
    """Cycle presence alone is not the tell. A transfer was applied on it and
    refused: the lead's mapping was functional and covered most of its residual,
    while the sibling mapped one source register three ways. These pin the
    measured separation -- 88% on the lead, 51% on the sibling."""

    def test_a_clean_permutation_is_fully_coherent(self) -> None:
        pairs = collections.Counter({("t6", "t7"): 50, ("t7", "t6"): 40})
        self.assertEqual(rc.coherence(pairs)["share_following_dominant"], 1.0)

    def test_a_source_splitting_three_ways_lowers_coherence(self) -> None:
        pairs = collections.Counter({("t6", "t7"): 10, ("t6", "t8"): 9,
                                     ("t6", "t9"): 8})
        c = rc.coherence(pairs)
        self.assertLess(c["share_following_dominant"], 0.5)
        self.assertEqual(c["least_coherent_source"], "t6")

    def test_the_least_coherent_source_is_named(self) -> None:
        pairs = collections.Counter({("t6", "t7"): 100,          # clean
                                     ("v1", "a0"): 5, ("v1", "a1"): 5})
        self.assertEqual(rc.coherence(pairs)["least_coherent_source"], "v1")

    def test_an_empty_mapping_does_not_divide_by_zero(self) -> None:
        c = rc.coherence(collections.Counter())
        self.assertEqual(c["sources"], 0)
        self.assertEqual(c["share_following_dominant"], 0.0)


if __name__ == "__main__":
    unittest.main()
