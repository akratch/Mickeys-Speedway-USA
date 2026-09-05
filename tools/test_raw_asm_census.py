#!/usr/bin/env python3
"""Synthetic ownership tests: never embeds game bytes or instructions."""
import unittest

from raw_asm_census import census


class CensusTests(unittest.TestCase):
    def config(self, name="overlay_001_tail", end=8, segment="overlay_001"):
        return {"segments": [{"name": segment, "type": "code", "start": 0,
                              "subsegments": [[0, "asm", name], [end, "bin", "data"]]}, [16]]}

    def test_explicit_zero_tail(self):
        result = census(self.config(), bytes(16), set())
        self.assertTrue(result["overlay_carve_complete"])
        self.assertEqual(result["bytes"], {"verified-zero-padding": 8})
        self.assertEqual(result["matching_credit"], 0)

    def test_small_nonzero_tail_is_not_padding(self):
        result = census(self.config(), b"a" * 16, set())
        self.assertFalse(result["overlay_carve_complete"])
        self.assertEqual(result["bytes"], {"overlay-raw": 8})

    def test_unnamed_zero_code_is_not_padding(self):
        result = census(self.config("compiler_block"), bytes(16), set())
        self.assertFalse(result["overlay_carve_complete"])

    def test_overlay_identity_must_agree(self):
        result = census(self.config("overlay_002_padding"), bytes(16), set())
        self.assertFalse(result["overlay_carve_complete"])

    def test_padding_before_code_is_not_excluded(self):
        config = self.config()
        config["segments"][0]["subsegments"][1][1] = "c"
        self.assertFalse(census(config, bytes(16), set())["overlay_carve_complete"])

    def test_handwritten_uses_evidence_not_extension(self):
        config = self.config("manual", segment="main")
        self.assertEqual(census(config, bytes(16), {"manual"})["counts"], {"verified-handwritten": 1})
        self.assertEqual(census(config, bytes(16), set())["counts"], {"resident-unclassified": 1})

    def test_invalid_extents_fail(self):
        for end in (0, 3, 20):
            with self.subTest(end=end), self.assertRaises(ValueError):
                census(self.config(end=end), bytes(16), set())

    def test_last_range_uses_next_segment_boundary(self):
        config = self.config("manual", segment="main")
        config["segments"][0]["subsegments"].pop()
        self.assertEqual(census(config, bytes(16), set())["ranges"][0]["size_bytes"], 16)

    def test_missing_boundary_fails(self):
        config = self.config()
        config["segments"].pop()
        config["segments"][0]["subsegments"].pop()
        with self.assertRaises(ValueError):
            census(config, bytes(16), set())

    def test_unnamed_raw_range_is_reported(self):
        config = self.config(segment="main")
        config["segments"][0]["subsegments"][0].pop()
        row = census(config, bytes(16), set())["ranges"][0]
        self.assertEqual(row["source"], "unnamed-rom-0")
        self.assertEqual(row["classification"], "resident-unclassified")

    def test_padding_at_empty_module_end(self):
        config = self.config()
        config["segments"][0]["subsegments"].pop()
        self.assertTrue(census(config, bytes(16), set())["overlay_carve_complete"])

    def test_padding_at_bare_text_endpoint(self):
        config = self.config()
        config["segments"][0]["subsegments"][1] = [8]
        self.assertTrue(census(config, bytes(16), set())["overlay_carve_complete"])


if __name__ == "__main__":
    unittest.main()
