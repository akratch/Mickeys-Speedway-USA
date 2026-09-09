import importlib.util
import unittest
from pathlib import Path

MODULE = Path(__file__).with_name("route_candidates.py")
spec = importlib.util.spec_from_file_location("route_candidates", MODULE)
rc = importlib.util.module_from_spec(spec)
spec.loader.exec_module(rc)


class RouteTests(unittest.TestCase):
    def test_routes_by_class_and_bounds(self):
        trial = {"results": [
            {"func": "a", "c_file": "src/overlays/o1/a.c", "overlay": 1, "klass": "text-differs", "in_range_words": 3, "out_of_range_bytes": 0},
            {"func": "b", "c_file": "src/overlays/o1/b.c", "overlay": 1, "klass": "text-differs", "in_range_words": 90, "out_of_range_bytes": 0},
            {"func": "c", "c_file": "src/overlays/o2/c.c", "overlay": 2, "klass": "rom-size", "in_range_words": 5, "out_of_range_bytes": 0, "cause": "module -24 bytes"},
            {"func": "d", "c_file": "src/overlays/o2/d.c", "overlay": 2, "klass": "build-error", "in_range_words": 0, "cause": "compile-error"},
            {"func": "e", "c_file": "src/overlays/o2/e.c", "overlay": 2, "klass": "build-error", "in_range_words": 0, "cause": "schedule-divergence-at-site (x)"},
            {"func": "f", "c_file": "src/overlays/o3/f.c", "overlay": 3, "klass": "exact", "in_range_words": 0, "out_of_range_bytes": 0},
        ]}
        ranking = {"functions": [
            {"name": "r1", "file": "src/main/x.c", "overlay": None, "relocation_masked_differing_words": 2, "differing_words": 2, "size_delta": 0, "category": "register-only"},
            {"name": "r2", "file": "src/main/y.c", "overlay": None, "relocation_masked_differing_words": 300, "differing_words": 300, "size_delta": -400, "category": "size-mismatch"},
            {"name": "a", "file": "src/overlays/o1/a.c", "overlay": 1, "relocation_masked_differing_words": 3, "differing_words": 3, "size_delta": 0, "category": "other"},
        ]}
        routes = rc.route(trial, ranking, 40, 8, sweep_done={"r1"})
        names = {k: [e["function"] for e in v] for k, v in routes.items()}
        self.assertEqual(names["sweep"], ["a"])
        self.assertEqual(names["ownership"], ["c"])
        self.assertEqual(names["lever"], ["f", "r1"])
        self.assertEqual(names["reshape"], ["e", "b"])
        self.assertEqual(names["compile-fix"], ["d"])
        self.assertEqual(names["structural"], ["r2"])

    def test_sweep_done_ignores_promotions(self):
        import json, tempfile, os
        d = tempfile.mkdtemp()
        p = os.path.join(d, "summary.json")
        with open(p, "w") as fh:
            json.dump([{"func": "x", "promoted": True}, {"func": "y", "best_score": 5}], fh)
        self.assertEqual(rc.sweep_done_names([p]), {"y"})


if __name__ == "__main__":
    unittest.main()
