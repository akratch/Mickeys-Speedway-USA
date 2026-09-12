#!/usr/bin/env python3
"""Unit tests for the reopen-authorization writer."""

import json
import unittest
from unittest import mock

import authorize_reopen as writer


class ReasonValidationTests(unittest.TestCase):
    """The schema lane_status enforces, checked before writing not after."""

    def test_a_reason_at_the_limit_is_accepted(self):
        writer.validate("x" * writer.REASON_LIMIT, "func_A")

    def test_an_over_long_reason_is_refused(self):
        with self.assertRaises(RuntimeError) as caught:
            writer.validate("x" * (writer.REASON_LIMIT + 1), "func_A")
        self.assertIn("241 chars", str(caught.exception))

    def test_a_newline_is_refused(self):
        with self.assertRaises(RuntimeError):
            writer.validate("two\nlines", "func_A")

    def test_a_pipe_is_refused(self):
        # The ledger's own column separator; lane_status rejects it.
        with self.assertRaises(RuntimeError):
            writer.validate("a | b", "func_A")

    def test_an_empty_reason_is_refused(self):
        with self.assertRaises(RuntimeError):
            writer.validate("   ", "func_A")


class ClassRoutingTests(unittest.TestCase):
    """--reason-from-class must route by the target's measured class."""

    ROWS = {
        "func_reg": {
            "category": "register-only",
            "relocation_masked_differing_words": 9,
            "size_delta": 0,
        },
        "func_size": {
            "category": "size-mismatch",
            "relocation_masked_differing_words": 300,
            "size_delta": -32,
        },
        "func_other": {
            "category": "other",
            "relocation_masked_differing_words": 26,
            "size_delta": 0,
        },
    }

    def test_register_only_routes_to_the_oracle_and_permuter(self):
        reason = writer.reason_for("func_reg", self.ROWS)
        self.assertIn("register-only", reason)
        self.assertIn("9 masked words", reason)

    def test_size_mismatch_routes_to_reconstruction_not_permutation(self):
        # The distinction that matters: no permutation closes a size deficit,
        # so a size-mismatch target must never be told to permute.
        reason = writer.reason_for("func_size", self.ROWS)
        self.assertIn("reconstruction", reason)
        self.assertIn("-32", reason)
        self.assertNotIn("permuter", reason)

    def test_other_routes_to_the_stall_rule(self):
        self.assertIn("0018", writer.reason_for("func_other", self.ROWS))

    def test_every_generated_reason_satisfies_the_schema(self):
        for symbol in self.ROWS:
            writer.validate(writer.reason_for(symbol, self.ROWS), symbol)

    def test_a_symbol_with_no_ranking_row_is_refused_not_guessed(self):
        with self.assertRaises(RuntimeError) as caught:
            writer.reason_for("func_absent", self.ROWS)
        self.assertIn("no ranking row", str(caught.exception))


class ReopenableTests(unittest.TestCase):
    def test_only_an_exhausted_verdict_may_be_authorized(self):
        # An active target is owned by a live lane and authorizing it would
        # race that lane; an assignable one needs no authorization at all.
        self.assertIn("already-integrated/exhausted", writer.REOPENABLE)
        self.assertNotIn("active", writer.REOPENABLE)
        self.assertNotIn("base-only", writer.REOPENABLE)


class ClassifyBatchTests(unittest.TestCase):
    def test_classify_sends_one_batch_call_and_indexes_by_symbol(self):
        payload = json.dumps({"assignments": [
            {"symbol": "func_A", "state": "base-only"},
            {"symbol": "func_B", "state": "active"},
        ]})
        with mock.patch.object(writer.subprocess, "run") as run:
            run.return_value = mock.Mock(stdout=payload, stderr="")
            out = writer.classify(["func_A", "func_B"])
        self.assertEqual(run.call_count, 1, "must be one batch call, not one per symbol")
        self.assertIn("--symbols", run.call_args[0][0])
        self.assertEqual(out["func_B"]["state"], "active")

    def test_a_symbol_the_classifier_skipped_is_an_error_not_a_silent_pass(self):
        payload = json.dumps({"assignments": [{"symbol": "func_A", "state": "base-only"}]})
        with mock.patch.object(writer.subprocess, "run") as run:
            run.return_value = mock.Mock(stdout=payload, stderr="")
            with self.assertRaises(RuntimeError) as caught:
                writer.classify(["func_A", "func_B"])
        self.assertIn("func_B", str(caught.exception))


class RefreshStaleTests(unittest.TestCase):
    """Renewing a drifted pin is not the same act as granting a new one.

    A pin arms only while its pinned commits match what the classifier derives
    now, so every lane that edits a handoff and gets merged invalidates that
    symbol's own pin. The authorization decays as a consequence of integration,
    silently. It cost two dispatched lanes outright on one function: each
    arrived, found the gate closed, and returned having done nothing.

    Renewing therefore restores a decision already made, and must PRESERVE the
    reason that was accepted. Overwriting many distinct justifications with one
    generic line would destroy the record of why each was granted.
    """

    STALE = "reopen authorization is stale because the pins no longer match"

    def _doc(self, authorizations):
        return json.dumps({"schema_version": 1,
                           "authorizations": authorizations})

    def test_discover_returns_a_stale_symbol_that_has_a_reason(self):
        doc = self._doc({"func_A": {"reason": "ADR 0018 stall rule",
                                    "source_commit": "a" * 40,
                                    "ledger_commit": "a" * 40}})
        with mock.patch.object(type(writer.AUTHORIZATIONS), "read_text",
                               return_value=doc), \
             mock.patch.object(writer, "ranking", return_value={"func_A": {}}), \
             mock.patch.object(writer, "classify", return_value={
                 "func_A": {"state": "already-integrated/exhausted",
                            "reason": self.STALE}}):
            self.assertEqual(writer.discover_stale(), ["func_A"])

    def test_discover_skips_a_stale_symbol_with_no_recorded_reason(self):
        """Granting a NEW authorization is a human judgement and must not
        happen as a side effect of a refresh."""
        doc = self._doc({})
        with mock.patch.object(type(writer.AUTHORIZATIONS), "read_text",
                               return_value=doc), \
             mock.patch.object(writer, "ranking", return_value={"func_A": {}}), \
             mock.patch.object(writer, "classify", return_value={
                 "func_A": {"state": "already-integrated/exhausted",
                            "reason": self.STALE}}):
            self.assertEqual(writer.discover_stale(), [])

    def test_discover_skips_a_symbol_that_is_not_stale(self):
        doc = self._doc({"func_A": {"reason": "still good"}})
        with mock.patch.object(type(writer.AUTHORIZATIONS), "read_text",
                               return_value=doc), \
             mock.patch.object(writer, "ranking", return_value={"func_A": {}}), \
             mock.patch.object(writer, "classify", return_value={
                 "func_A": {"state": "base-only", "reason": "assignable"}}):
            self.assertEqual(writer.discover_stale(), [])

    def test_discover_is_sorted_so_a_run_is_reproducible(self):
        doc = self._doc({"func_B": {"reason": "r"}, "func_A": {"reason": "r"}})
        with mock.patch.object(type(writer.AUTHORIZATIONS), "read_text",
                               return_value=doc), \
             mock.patch.object(writer, "ranking",
                               return_value={"func_B": {}, "func_A": {}}), \
             mock.patch.object(writer, "classify", return_value={
                 "func_B": {"state": "already-integrated/exhausted",
                            "reason": self.STALE},
                 "func_A": {"state": "already-integrated/exhausted",
                            "reason": self.STALE}}):
            self.assertEqual(writer.discover_stale(), ["func_A", "func_B"])

    def test_refresh_and_an_explicit_reason_are_mutually_exclusive(self):
        """--refresh-stale preserves reasons; passing one would overwrite the
        very thing the mode exists to keep."""
        with mock.patch.object(
                writer.sys, "argv",
                ["authorize_reopen.py", "--refresh-stale", "--reason", "x"]):
            with self.assertRaises(SystemExit):
                writer.main()

    def test_symbols_is_not_required_with_refresh(self):
        """The mode discovers its own work; requiring --symbols would put the
        coordinator back in the business of remembering which pins died."""
        with mock.patch.object(writer.sys, "argv",
                               ["authorize_reopen.py", "--refresh-stale"]), \
             mock.patch.object(writer, "discover_stale", return_value=[]):
            self.assertEqual(writer.main(), 0)

if __name__ == "__main__":
    unittest.main()
