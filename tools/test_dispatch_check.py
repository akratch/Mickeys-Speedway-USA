#!/usr/bin/env python3
"""Every case here replays waste that actually happened while planning."""
import unittest
from unittest import mock

import dispatch_check as dc

ROW = {"size_bytes": 400, "differing_words": 12,
       "relocation_masked_differing_words": 10, "size_delta": 0, "category": "other"}


def rows(*names):
    return {n: dict(ROW, name=n) for n in names}


class PlanParsing(unittest.TestCase):
    def test_lanes_and_symbol_lists_parse(self):
        plan = dc.parse_plan(["a=x,y", "b=z"])
        self.assertEqual(plan, {"a": ["x", "y"], "b": ["z"]})

    def test_a_repeated_lane_accumulates_rather_than_overwrites(self):
        self.assertEqual(dc.parse_plan(["a=x", "a=y"]), {"a": ["x", "y"]})

    def test_a_malformed_assignment_is_rejected(self):
        with self.assertRaises(ValueError):
            dc.parse_plan(["no-equals-sign"])


class Refusals(unittest.TestCase):
    def run_check(self, plan, queued):
        with mock.patch.object(dc, "queued_rows", return_value=queued), \
             mock.patch.object(dc, "closure_facts", return_value={}):
            return dc.check(plan)

    def test_a_symbol_in_two_lanes_is_refused(self):
        """Caught by hand twice while planning campaign 2."""
        problems, _ = self.run_check({"a": ["dup"], "b": ["dup"]}, rows("dup"))
        self.assertTrue(any("two lanes" in p or "2 lanes" in p for p in problems))

    def test_the_refusal_names_both_lanes(self):
        problems, _ = self.run_check({"alpha": ["dup"], "beta": ["dup"]}, rows("dup"))
        joined = " ".join(problems)
        self.assertIn("alpha", joined)
        self.assertIn("beta", joined)

    def test_an_unqueued_symbol_is_refused(self):
        """A matched function has nothing left to work."""
        problems, _ = self.run_check({"a": ["gone"]}, rows("other"))
        self.assertTrue(any("not in the NON_MATCHING queue" in p for p in problems))

    def test_a_sound_plan_raises_nothing(self):
        problems, _ = self.run_check({"a": ["x"], "b": ["y"]}, rows("x", "y"))
        self.assertEqual(problems, [])

    def test_one_symbol_in_one_lane_twice_is_not_an_overlap(self):
        """Sloppy input, not a planning fault; do not refuse on it."""
        problems, _ = self.run_check({"a": ["x", "x"]}, rows("x"))
        self.assertEqual(problems, [])


class EmittedFacts(unittest.TestCase):
    def run_check(self, plan, queued, closures=None):
        with mock.patch.object(dc, "queued_rows", return_value=queued), \
             mock.patch.object(dc, "closure_facts", return_value=closures or {}):
            return dc.check(plan)

    def test_the_artifact_count_is_stated_so_no_lane_re_derives_it(self):
        """Two lanes were told to spend a phase computing this number."""
        _, notes = self.run_check({"a": ["x"]}, rows("x"))
        self.assertIn("artifact=2", " ".join(notes))

    def test_masked_and_delta_and_class_are_all_carried(self):
        _, notes = self.run_check({"a": ["x"]}, rows("x"))
        text = " ".join(notes)
        for field in ("masked=10", "delta=0", "class=other"):
            self.assertIn(field, text)

    def test_a_pre_law_closure_is_surfaced_in_the_brief(self):
        closures = {"x": {"symbol": "x", "closed": "2026-09-01",
                          "laws": ["L93 loop shape"]}}
        _, notes = self.run_check({"a": ["x"]}, rows("x"), closures)
        text = " ".join(notes)
        self.assertIn("closure predates", text)
        self.assertIn("L93", text)


if __name__ == "__main__":
    unittest.main()
