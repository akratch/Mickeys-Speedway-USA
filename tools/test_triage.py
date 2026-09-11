#!/usr/bin/env python3
"""Unit tests for the wave-scoping arithmetic."""
import pathlib
import sys
import unittest

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


if __name__ == "__main__":
    unittest.main()
