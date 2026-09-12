#!/usr/bin/env python3
"""Tests for the force-combination search.

The compile-and-score path needs the instrumented toolchain and a built tree,
so it is exercised by a lane rather than here. What IS tested here is the part
that decides what gets measured and how a result is read -- which is where the
reasoning errors live.
"""
import unittest

import force_lattice as fl


class SubsetPlanTests(unittest.TestCase):
    def test_every_non_empty_subset_is_planned(self):
        plan = fl.subsets(["a", "b", "c"])
        self.assertEqual(len(plan), 7)          # 2^3 - 1
        self.assertIn(("a", "b", "c"), plan)

    def test_singles_come_first(self):
        """A single that fails to reproduce its inherited receipt invalidates
        every combination containing it. Finding that out first saves the sweep.
        """
        plan = fl.subsets(["a", "b", "c"])
        self.assertEqual([len(p) for p in plan[:3]], [1, 1, 1])
        self.assertEqual(len(plan[-1]), 3)

    def test_max_size_bounds_the_sweep(self):
        plan = fl.subsets(["a", "b", "c", "d"], max_size=2)
        self.assertTrue(all(len(p) <= 2 for p in plan))
        self.assertEqual(len(plan), 4 + 6)

    def test_the_empty_set_is_not_a_cell(self):
        """The base is measured separately; planning it here would double it."""
        self.assertNotIn((), fl.subsets(["a"]))


class ForceValidationTests(unittest.TestCase):
    def test_a_wellformed_force_passes(self):
        fl.validate_forces(["p1:w27=c4", "p2:w9=c1"])

    def test_a_malformed_force_is_refused_before_compiling(self):
        """A malformed spec is ignored by the compiler, which reads as a
        declined force -- the exact shape of Trap 20."""
        for bad in ("w27=c4", "p3:w27=c4", "p1:w27", "p1:wX=c4", ""):
            with self.subTest(bad=bad):
                with self.assertRaises(SystemExit):
                    fl.validate_forces([bad])

    def test_one_web_forced_twice_is_refused(self):
        """The compiler takes one and the result would be credited to both."""
        with self.assertRaises(SystemExit) as caught:
            fl.validate_forces(["p1:w27=c4", "p1:w27=c5"])
        self.assertIn("twice", str(caught.exception))

    def test_the_same_colour_on_two_webs_is_allowed(self):
        """Two webs may legitimately be asked for the same colour; only the
        allocator decides whether that is satisfiable."""
        fl.validate_forces(["p1:w27=c4", "p1:w75=c4"])


class InteractionTests(unittest.TestCase):
    """The verdict that matters is ANTAGONISTIC.

    Two colours that each help alone but hurt together are competing for one
    register. That says the target's assignment is not the union of the
    individual wins, and it names the contest -- which a sequence of single
    measurements cannot show at all.
    """

    def test_independent_effects_read_as_additive(self):
        # base 100, each saves 10, together saves 20.
        self.assertEqual(fl.interaction(80, 90, 90, 100), "additive")

    def test_beating_the_sum_is_synergistic(self):
        self.assertEqual(fl.interaction(70, 90, 90, 100), "SYNERGISTIC")

    def test_falling_short_of_the_sum_is_antagonistic(self):
        self.assertEqual(fl.interaction(95, 90, 90, 100), "ANTAGONISTIC")

    def test_a_pair_worse_than_either_single_is_antagonistic(self):
        """The L100 shape inverted: each helps, the pair hurts."""
        self.assertEqual(fl.interaction(105, 90, 90, 100), "ANTAGONISTIC")

    def test_a_pair_that_helps_where_both_singles_hurt_is_synergistic(self):
        """L100's actual receipt: a function went 21 -> 5 on a pair where each
        edit alone was a 31-word regression. Independence predicts far worse
        than the pair achieves, so this must not read as merely additive."""
        self.assertEqual(fl.interaction(5, 52, 52, 21), "SYNERGISTIC")


class CellTests(unittest.TestCase):
    def test_a_cell_labels_itself_by_its_forces(self):
        cell = fl.Cell(("p1:w27=c4", "p1:w75=c3"), 200, True)
        self.assertEqual(cell.label, "w27=c4+w75=c3")

    def test_the_base_cell_is_labelled(self):
        self.assertEqual(fl.Cell((), 227, True).label, "(base)")


class RenderTests(unittest.TestCase):
    def test_an_unapplied_force_is_not_scored_as_a_result(self):
        """Trap 20: a dropped force returns a byte-identical object. Scoring it
        would record the base as though the force had been tried and declined.
        """
        base = fl.Cell((), 227, True)
        cells = [fl.Cell(("p1:w27=c4",), None, False, "forced=-2")]
        text = fl.render(base, cells, ["p1:w27=c4"])
        self.assertIn("UNMEASURED", text)
        self.assertNotIn("BEST", text)

    def test_the_best_combination_is_named(self):
        base = fl.Cell((), 227, True)
        cells = [
            fl.Cell(("p1:w27=c4",), 224, True),
            fl.Cell(("p1:w75=c3",), 212, True),
            fl.Cell(("p1:w27=c4", "p1:w75=c3"), 200, True),
        ]
        text = fl.render(base, cells, ["p1:w27=c4", "p1:w75=c3"])
        self.assertIn("BEST: 200", text)
        # Positive means words SAVED, consistently with the singles column
        # (+3, +15). A mixed convention here would be read backwards exactly
        # once, by whoever is reading a regression at 3am.
        self.assertIn("+27", text)

    def test_saving_words_reads_as_a_positive_delta_everywhere(self):
        base = fl.Cell((), 227, True)
        cells = [fl.Cell(("p1:w75=c3",), 212, True)]
        text = fl.render(base, cells, ["p1:w75=c3"])
        self.assertIn("+15", text)          # single
        self.assertIn("+15 against the base", text)   # best

class InstrumentedSwapTests(unittest.TestCase):
    """The compiler is NOT argv[0], and assuming it is poisons every cell.

    This project's configured command runs the compile through asm-processor,
    so argv[0] is the wrapper. Swapping argv[0] replaces the wrapper and
    silently compiles with the STOCK compiler -- every force then reads as
    declined, which is indistinguishable from a real negative result.
    """

    def test_the_driver_is_found_where_it_actually_sits(self):
        command = ["python3", "asm_processor.py", "tools/ido/cc", "-c", "x.c"]
        out = fl._instrumented(command)
        self.assertTrue(out[2].endswith("ido-instrumented/cc"))
        self.assertEqual(out[0], "python3")          # wrapper untouched
        self.assertEqual(out[1], "asm_processor.py")

    def test_only_the_first_driver_is_swapped(self):
        """The command names the driver twice (asm-processor passes it the
        compiler as well); swapping both would corrupt the wrapper's argument."""
        command = ["tools/ido/cc", "--", "tools/ido/cc", "-c"]
        out = fl._instrumented(command)
        self.assertTrue(out[0].endswith("ido-instrumented/cc"))
        self.assertEqual(out[2], "tools/ido/cc")

    def test_a_command_with_no_driver_refuses_rather_than_guessing(self):
        """Guessing compiles with the stock compiler and reports declines."""
        with self.assertRaises(SystemExit) as caught:
            fl._instrumented(["gcc", "-c", "x.c"])
        self.assertIn("refusing to guess", str(caught.exception))

    def test_every_other_argument_is_preserved_exactly(self):
        """A dropped per-file flag makes the forced and configured builds
        differ for a reason that has nothing to do with the force."""
        command = ["py", "ap.py", "tools/ido/cc", "-Wab,-r4300_mul", "-O2", "-o", "a.o"]
        out = fl._instrumented(command)
        self.assertEqual(out[3:], ["-Wab,-r4300_mul", "-O2", "-o", "a.o"])

class BlastRadiusTests(unittest.TestCase):
    """Why an interaction happens, not merely that it did.

    The whale's handoff records that intervention prices "are global effects".
    That is the observation; the radius is the measurement. Two forces whose
    radii are disjoint should compose. Two that move the same window are
    contending for something in it, and will disappoint however good each
    looks alone.
    """

    BASE = None

    def setUp(self):
        self.BASE = fl.Cell((), 227, True,
                            windows={0x0: 23, 0x200: 13, 0x600: 9})

    def test_a_local_force_moves_one_window(self):
        cell = fl.Cell(("a",), 224, True, windows={0x0: 20, 0x200: 13, 0x600: 9})
        self.assertEqual(fl.blast_radius(self.BASE, cell), {0x0: -3})

    def test_improvement_reads_negative_and_regression_positive(self):
        better = fl.Cell(("a",), 224, True, windows={0x0: 20, 0x200: 13, 0x600: 9})
        worse = fl.Cell(("b",), 230, True, windows={0x0: 26, 0x200: 13, 0x600: 9})
        self.assertEqual(fl.blast_radius(self.BASE, better)[0x0], -3)
        self.assertEqual(fl.blast_radius(self.BASE, worse)[0x0], +3)

    def test_a_window_that_did_not_move_is_omitted(self):
        """Reporting unmoved windows would bury the signal in 30 zero rows."""
        cell = fl.Cell(("a",), 224, True, windows={0x0: 20, 0x200: 13, 0x600: 9})
        self.assertNotIn(0x200, fl.blast_radius(self.BASE, cell))

    def test_a_window_appearing_only_after_the_force_still_counts(self):
        """A force can make a previously exact window differ; that is exactly
        the global effect worth seeing."""
        cell = fl.Cell(("a",), 229, True,
                       windows={0x0: 23, 0x200: 13, 0x600: 9, 0xC00: 2})
        self.assertEqual(fl.blast_radius(self.BASE, cell)[0xC00], +2)

    def test_disjoint_radii_do_not_collide(self):
        a = fl.Cell(("a",), 224, True, windows={0x0: 20, 0x200: 13, 0x600: 9})
        b = fl.Cell(("b",), 212, True, windows={0x0: 23, 0x200: 4, 0x600: 9})
        self.assertEqual(fl.collides(self.BASE, a, b), set())

    def test_a_shared_window_is_reported_as_a_collision(self):
        a = fl.Cell(("a",), 224, True, windows={0x0: 20, 0x200: 13, 0x600: 9})
        b = fl.Cell(("b",), 220, True, windows={0x0: 19, 0x200: 13, 0x600: 9})
        self.assertEqual(fl.collides(self.BASE, a, b), {0x0})

    def test_a_missing_window_census_yields_no_radius(self):
        """An unmeasured cell must not read as 'moved nothing'."""
        self.assertEqual(fl.blast_radius(self.BASE, fl.Cell(("a",), 224, True)), {})


if __name__ == "__main__":
    unittest.main()
