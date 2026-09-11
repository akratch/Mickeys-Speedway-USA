#!/usr/bin/env python3
"""Unit tests for the wave-scoping arithmetic."""
import pathlib
import sys
import unittest
import unittest.mock

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import triage  # noqa: E402


def fn(name, size, words, overlay=None):
    return {"name": name, "size_bytes": size, "overlay": overlay,
            "relocation_masked_differing_words": words}


class ClusterTests(unittest.TestCase):
    def test_identical_size_in_one_overlay_is_a_cluster(self) -> None:
        rows = [fn("a", 2100, 149, 101), fn("b", 2100, 149, 101),
                fn("c", 900, 12, 101)]
        out = triage.clusters(rows)
        self.assertEqual(len(out), 1)
        self.assertEqual(out[0]["count"], 2)
        self.assertEqual(out[0]["total_bytes"], 4200)

    def test_same_size_in_different_overlays_is_not_a_cluster(self) -> None:
        """Size alone is a coincidence; it is size within one overlay that has
        meant 'the same routine specialised' in this codebase."""
        rows = [fn("a", 2100, 149, 101), fn("b", 2100, 149, 58)]
        self.assertEqual(triage.clusters(rows), [])

    def test_lead_is_the_cheapest_member_and_spread_is_reported(self) -> None:
        rows = [fn("a", 800, 145, 101), fn("b", 800, 145, 101),
                fn("c", 800, 163, 101)]
        out = triage.clusters(rows)[0]
        self.assertEqual(out["lead_words"], 145)
        self.assertEqual(out["spread"], 18)
        self.assertEqual(out["words"], [145, 145, 163])

    def test_clusters_sort_by_total_bytes(self) -> None:
        rows = [fn("a", 400, 5, 1), fn("b", 400, 5, 1),
                fn("c", 2000, 5, 2), fn("d", 2000, 5, 2)]
        self.assertEqual([c["size_bytes"] for c in triage.clusters(rows)], [2000, 400])


class RouteTests(unittest.TestCase):
    def test_the_route_prefers_bytes_per_word_not_raw_word_count(self) -> None:
        """A big function with a moderate residual is a better buy than a small
        one with a small residual -- that is the whole point of the ordering."""
        big = fn("big", 2100, 149)      # 0.07 words per byte
        small = fn("small", 200, 40)    # 0.20 words per byte
        out = triage.cheapest_route([small, big], 2000)
        self.assertEqual(out["names"][0], "big")

    def test_the_route_stops_once_the_gap_is_covered(self) -> None:
        rows = [fn("a", 1000, 10), fn("b", 1000, 10), fn("c", 1000, 10)]
        out = triage.cheapest_route(rows, 1500)
        self.assertEqual(out["functions"], 2)
        self.assertEqual(out["bytes"], 2000)

    def test_a_zero_gap_still_returns_one_function_not_a_crash(self) -> None:
        out = triage.cheapest_route([fn("a", 100, 1)], 0)
        self.assertEqual(out["functions"], 1)

    def test_words_are_summed_over_the_picked_set(self) -> None:
        rows = [fn("a", 1000, 10), fn("b", 1000, 25)]
        out = triage.cheapest_route(rows, 2000)
        self.assertEqual(out["words"], 35)




class UnassignableTests(unittest.TestCase):
    """A symbol with a proof that it cannot match must never reach a lane.

    The bar is machine-enforced rather than remembered, because the symbols
    that earn it are large and nearly closed -- exactly what a ratio-sorted
    list puts on top -- so every wave surfaces them again.
    """

    def _fixture(self, ranking_rows, barred):
        import json
        import tempfile
        tmp = pathlib.Path(tempfile.mkdtemp())
        rank = tmp / "ranking.json"
        rank.write_text(json.dumps({"functions": ranking_rows}), encoding="utf-8")
        bar = tmp / "barred.json"
        bar.write_text(json.dumps({"schema_version": 1, "symbols": barred}),
                       encoding="utf-8")
        return rank, bar

    def test_a_barred_symbol_is_dropped_from_the_queue(self) -> None:
        rank, bar = self._fixture(
            [fn("keep", 100, 4, 1), fn("barred", 1416, 5, 57)],
            {"barred": {"reason": "proven unmatchable at a floor of 2"}})
        with unittest.mock.patch.object(triage, "RANKING", rank), \
                unittest.mock.patch.object(triage, "UNASSIGNABLE", bar):
            self.assertEqual([r["name"] for r in triage.load()], ["keep"])

    def test_a_barred_symbol_never_appears_in_the_cheapest_route(self) -> None:
        """It would sort first: 1416 bytes at 5 words is the best ratio here."""
        rank, bar = self._fixture(
            [fn("keep", 100, 4, 1), fn("barred", 1416, 5, 57)],
            {"barred": {"reason": "proven unmatchable at a floor of 2"}})
        with unittest.mock.patch.object(triage, "RANKING", rank), \
                unittest.mock.patch.object(triage, "UNASSIGNABLE", bar):
            route = triage.cheapest_route(triage.load(), 10_000)
            self.assertNotIn("barred", route["names"])

    def test_the_exclusion_is_reported_rather_than_silent(self) -> None:
        """A silently shorter list reads as the queue having shrunk. The
        report has to say a symbol was removed and why, or the next reader
        re-adds it."""
        rank, bar = self._fixture(
            [fn("keep", 100, 4, 1), fn("barred", 1416, 5, 57)],
            {"barred": {"reason": "proven unmatchable at a floor of 2"}})
        with unittest.mock.patch.object(triage, "RANKING", rank), \
                unittest.mock.patch.object(triage, "UNASSIGNABLE", bar):
            rendered = triage.render(triage.report(60.0, 5))
        self.assertIn("barred", rendered)
        self.assertIn("proven unmatchable at a floor of 2", rendered)
        self.assertIn("Never assign it", rendered)

    def test_a_barred_symbol_absent_from_the_ranking_is_not_reported(self) -> None:
        """Once a symbol leaves the queue the bar is still correct, but
        printing it every wave trains the reader to skip the line."""
        rank, bar = self._fixture(
            [fn("keep", 100, 4, 1)],
            {"gone": {"reason": "proven unmatchable"}})
        with unittest.mock.patch.object(triage, "RANKING", rank), \
                unittest.mock.patch.object(triage, "UNASSIGNABLE", bar):
            self.assertEqual(triage.report(60.0, 5)["excluded"], [])

    def test_a_missing_registry_bars_nothing(self) -> None:
        rank, bar = self._fixture([fn("keep", 100, 4, 1)], {})
        with unittest.mock.patch.object(triage, "RANKING", rank), \
                unittest.mock.patch.object(triage, "UNASSIGNABLE", bar / "absent"):
            self.assertEqual([r["name"] for r in triage.load()], ["keep"])


class ShippedRegistryTests(unittest.TestCase):
    def test_the_proven_unmatchable_overlay_57_symbol_is_barred(self) -> None:
        """A standing project constraint. It has been re-surfaced by a
        ratio-sorted list in more than one wave."""
        self.assertIn("overlay57UpdateModeState", triage.unassignable())

    def test_every_barred_symbol_carries_its_proof(self) -> None:
        """'Do not assign' without the evidence is indistinguishable from a
        plateau someone gave up on, and gets overturned."""
        for name, entry in triage.unassignable().items():
            with self.subTest(name):
                self.assertTrue(entry.get("reason", "").strip(), name)
                self.assertGreater(len(entry.get("evidence", "")), 80, name)

if __name__ == "__main__":
    unittest.main()
