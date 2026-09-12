#!/usr/bin/env python3
"""Tests for the force-combination search.

The real compile-and-score path is calibrated by lanes. Fake compiler runs
here exercise its command, artifact, acceptance and failure contracts without
requiring a ROM or instrumented compiler.
"""
import pathlib
import subprocess
import tempfile
import time
from types import SimpleNamespace
import unittest
from unittest import mock

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

    def test_zero_score_still_has_an_interaction(self):
        forces = ["p1:w1=c1", "p1:w2=c2"]
        cells = [fl.Cell((forces[0],), 0, True), fl.Cell((forces[1],), 2, True),
                 fl.Cell(tuple(forces), 0, True)]
        self.assertIn("ANTAGONISTIC", fl.render(fl.Cell((), 3, True), cells, forces))

    def test_changed_size_does_not_win_or_claim_an_interaction(self):
        forces = ["p1:w1=c1", "p1:w2=c2"]
        cells = [fl.Cell((f,), 5, True, size_delta=0) for f in forces]
        cells.append(fl.Cell(tuple(forces), 0, True, size_delta=-4))
        report = fl.render(fl.Cell((), 10, True, size_delta=0), cells, forces)
        self.assertIn("BEST: 5", report)
        self.assertIn("SIZE-CHANGED", report)


def accepted_trace(web=75, colour=16, proc=0, forced=None):
    forced = colour if forced is None else forced
    # The proposed bestcolor is deliberately different from the final colour.
    return (f"[CDX] p1dec phase=p1 proc={proc} web={web} bestcolor=14 "
            f"decision=color forced={forced}\n"
            f"[CDX] p1color phase=p1 proc={proc} web={web} color={colour} "
            f"reg=s2 forced={forced}\n")


class AcceptanceTests(unittest.TestCase):
    def test_final_assignment_overrides_proposal_and_ignores_unrequested_declines(self):
        trace = accepted_trace() + accepted_trace(web=80, forced=-2)
        self.assertIsNone(fl.force_acceptance(trace, 0, ("p1:w75=c16",)))

    def test_legacy_accepted_marker_requires_the_final_colour(self):
        self.assertIsNone(fl.force_acceptance(accepted_trace(forced=-1), 0, ("p1:w75=c16",)))
        self.assertIsNotNone(fl.force_acceptance(accepted_trace(colour=15, forced=-1), 0,
                                               ("p1:w75=c16",)))

    def test_missing_wrong_procedure_declined_and_duplicate_records_fail_closed(self):
        for trace in ("", accepted_trace(proc=1), accepted_trace(web=76),
                      accepted_trace(forced=-2), accepted_trace() * 2,
                      accepted_trace().splitlines()[0]):
            with self.subTest(trace=trace):
                self.assertIsNotNone(fl.force_acceptance(trace, 0, ("p1:w75=c16",)))

    def test_every_requested_force_must_apply(self):
        self.assertIsNotNone(fl.force_acceptance(accepted_trace(), 0,
                                               ("p1:w75=c16", "p1:w27=c17")))


class ExecutionTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.directory = pathlib.Path(self.temp.name) / "cell"
        self.command = ["python", "tools/asm-processor/build.py", "tools/ido/cc",
                        "--", "as", "--", "-O2", "-Wab,-r4300_mul", "-o", "old.o", "src/a.c"]

    def run_cell(self, forces=(), **kwargs):
        return fl.run_cell("symbol", 0, forces, command=self.command,
                           directory=self.directory, deadline=time.time() + 600,
                           timeout=120, **kwargs)

    def test_wrapper_and_flags_survive_and_the_exact_new_object_is_scored(self):
        def compile(actual, **kwargs):
            self.assertEqual(actual[:2], self.command[:2])
            self.assertEqual(actual[2], str(fl.INSTRUMENTED / "cc"))
            self.assertEqual(actual[3:9], self.command[3:9])
            self.assertEqual(kwargs["env"]["CDX_PROC"], "0")
            self.assertEqual(kwargs["env"]["CDX_FORCE"], "p1:w75=c16")
            self.assertEqual(kwargs["timeout"], 120)
            pathlib.Path(actual[actual.index("-o") + 1]).write_bytes(b"fake object")
            pathlib.Path(kwargs["env"]["CDX_OUT"]).write_text(accepted_trace())
            return subprocess.CompletedProcess(actual, 0, "", "")
        result = SimpleNamespace(relocation_masked_differing_words=212, size_delta=0,
                                 differing_words=381, relocation_masked_first_mismatch_offset=80)
        with mock.patch.object(fl.subprocess, "run", side_effect=compile), \
                mock.patch.object(fl, "_score", return_value=result) as score:
            cell = self.run_cell(("p1:w75=c16",))
        self.assertEqual(cell.score, 212)
        score.assert_called_once_with("symbol", self.directory / "candidate.o")
        self.assertTrue((self.directory / "result.json").is_file())

    def test_missing_trace_does_not_reach_the_scorer(self):
        def compile(actual, **kwargs):
            (self.directory / "candidate.o").write_bytes(b"fake object")
            return subprocess.CompletedProcess(actual, 0, "", "")
        with mock.patch.object(fl.subprocess, "run", side_effect=compile), \
                mock.patch.object(fl, "_score") as score:
            cell = self.run_cell(("p1:w75=c16",))
        self.assertFalse(cell.accepted)
        score.assert_not_called()

    def test_base_clears_inherited_force_and_trace_environment(self):
        def compile(actual, **kwargs):
            self.assertNotIn("CDX_FORCE", kwargs["env"])
            self.assertNotIn("DKWB_UGEN_TRACE", kwargs["env"])
            return subprocess.CompletedProcess(actual, 1, "", "failure")
        with mock.patch.dict(fl.os.environ, CDX_FORCE="p1:w75=c16", DKWB_UGEN_TRACE="1"), \
                mock.patch.object(fl.subprocess, "run", side_effect=compile):
            self.assertFalse(self.run_cell().accepted)

    def test_deadline_refuses_before_starting_a_compile(self):
        with mock.patch.object(fl.subprocess, "run") as compile:
            cell = fl.run_cell("symbol", 0, (), command=self.command, directory=self.directory,
                               deadline=time.time() + 1, timeout=120)
        self.assertFalse(cell.accepted)
        compile.assert_not_called()

    def test_timeout_is_not_a_success(self):
        with mock.patch.object(fl.subprocess, "run", side_effect=subprocess.TimeoutExpired("cc", 120)):
            self.assertFalse(self.run_cell().accepted)


if __name__ == "__main__":
    unittest.main()
