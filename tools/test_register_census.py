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
A2, A3 = 6, 7


def addu(rd, rs, rt):
    return (0x00 << 26) | (rs << 21) | (rt << 16) | (rd << 11) | 0x21


def lwc1(ft, base, off):
    return (0x31 << 26) | (base << 21) | (ft << 16) | (off & 0xFFFF)


def add_s(fd, fs, ft):
    return (0x11 << 26) | (0x10 << 21) | (ft << 16) | (fs << 11) | (fd << 6)


def mtc1(rt, fs):
    return (0x11 << 26) | (0x04 << 21) | (rt << 16) | (fs << 11)


def bc1t(off):
    return (0x11 << 26) | (0x08 << 21) | (1 << 16) | (off & 0xFFFF)


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


class FloatBankTests(unittest.TestCase):
    """The bank tag, pinned. Without it `lwc1 $f2` against `lwc1 $f6` was read
    as the GPR substitution `v0 -> a2`, and COP1 register format contributed
    nothing at all -- so a wholly float residual printed as an integer ring
    cycle with L127 beside it. Measured on `func_8003F154`: nine of thirteen
    sites dropped, the other four mislabelled."""

    def test_a_float_load_datum_is_a_float_register(self) -> None:
        out = rc.census(streams([lwc1(2, A3, 60)], [lwc1(6, A3, 60)]))
        self.assertEqual(out["sites"], 1)
        self.assertEqual(out["pairs"], collections.Counter())
        self.assertEqual(out["fpairs"][("f2", "f6")], 1)

    def test_a_float_load_base_is_still_an_integer_register(self) -> None:
        out = rc.census(streams([lwc1(2, A2, 60)], [lwc1(2, A3, 60)]))
        self.assertEqual(out["pairs"][("a2", "a3")], 1)
        self.assertEqual(out["fpairs"], collections.Counter())

    def test_cop1_register_format_is_counted(self) -> None:
        out = rc.census(streams([add_s(2, 18, 10)], [add_s(4, 18, 10)]))
        self.assertEqual(out["sites"], 1)
        self.assertEqual(out["fpairs"][("f2", "f4")], 1)

    def test_mtc1_splits_the_two_banks(self) -> None:
        out = rc.census(streams([mtc1(0, 6)], [mtc1(0, 2)]))
        self.assertEqual(out["fpairs"][("f6", "f2")], 1)
        self.assertEqual(out["pairs"], collections.Counter())

    def test_bc1_names_no_register(self) -> None:
        self.assertEqual(rc.fields(bc1t(4)), ())


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

class ColouredCycleCitationTests(unittest.TestCase):
    """L127 is a ugen scratch-ring fact. A cycle over registers globalcolor
    assigns is a p1 save-ratio residual instead, and citing L127 at it sends
    the reader to the wrong allocator.

    Reported from overlay 92, where the tool printed a clean coherent
    s1 -> s2 cycle with "see L127" beside it. The real lever was the save
    ratio, and a force on the ranking took that object 26 masked words to 11.
    """

    def _row(self, cycle, share=0.95):
        return {
            "pairs": [{"ours": cycle[0], "theirs": cycle[1], "count": 9}],
            "coherence": {"share_following_dominant": share, "sources": 2,
                          "least_coherent_source": None,
                          "least_coherent_share": 0.0},
            "windows": {"global_share": 0.95, "windows": 1, "boundaries": []},
            "cycles": [list(cycle)],
        }

    def test_a_cycle_over_saved_registers_does_not_cite_l127(self) -> None:
        out = "\n".join(rc.render_bank(self._row(("s1", "s2")), "", "int", "ring"))
        self.assertNotIn("see L127", out)
        self.assertIn("does NOT", out)
        self.assertIn("save-ratio", out)

    def test_that_note_names_the_offending_registers(self) -> None:
        """A bare 'L127 may not apply' makes the reader re-derive which
        registers triggered it."""
        out = "\n".join(rc.render_bank(self._row(("s1", "s2")), "", "int", "ring"))
        self.assertIn("s1", out)
        self.assertIn("s2", out)

    def test_it_routes_to_the_colour_table_and_l100(self) -> None:
        out = "\n".join(rc.render_bank(self._row(("s1", "s2")), "", "int", "ring"))
        self.assertIn("p1cost", out)
        self.assertIn("L100", out)

    def test_a_cycle_over_ring_temporaries_still_cites_l127(self) -> None:
        """The guard must not suppress the citation where it is correct."""
        out = "\n".join(rc.render_bank(self._row(("t7", "t8")), "", "int", "ring"))
        self.assertIn("see L127", out)
        self.assertNotIn("does NOT", out)

    def test_one_saved_register_in_a_mixed_cycle_is_enough(self) -> None:
        """A cycle that touches a colour at all cannot be a pure ring phase."""
        row = self._row(("t7", "s0"))
        row["cycles"] = [["t7", "s0", "t8"]]
        out = "\n".join(rc.render_bank(row, "", "int", "ring"))
        self.assertNotIn("see L127", out)
        self.assertIn("s0", out)

    def test_callee_saved_float_registers_are_treated_the_same(self) -> None:
        """L133: f20-f30 are the callee-saved half of the float colour table."""
        out = "\n".join(rc.render_bank(self._row(("f20", "f22")), "", "fpr", "pool"))
        self.assertNotIn("see L127", out)
        self.assertIn("f20", out)

    def test_low_float_registers_are_not_treated_as_colours(self) -> None:
        """L133 measured f0-f6 outside the table entirely -- pure ring
        temporaries, which is exactly where L127 does apply."""
        out = "\n".join(rc.render_bank(self._row(("f4", "f6")), "", "fpr", "pool"))
        self.assertIn("see L127", out)

    def test_an_incoherent_coloured_cycle_keeps_the_incoherence_warning(self) -> None:
        """The pre-existing warning is about a different failure and must
        still fire ahead of the colour note."""
        out = "\n".join(rc.render_bank(self._row(("s1", "s2"), share=0.51),
                                        "", "int", "ring"))
        self.assertIn("incoherent", out)
        self.assertNotIn("see L127", out)

if __name__ == "__main__":
    unittest.main()
