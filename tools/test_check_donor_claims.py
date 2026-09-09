#!/usr/bin/env python3
"""Unit tests for the donor-claim reopen guard."""

import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

import check_donor_claims as guard


class ClaimDetectionTests(unittest.TestCase):
    """Which reasons send a lane to a donor, and which only mention one."""

    AFFIRMATIVE = (
        "New donor evidence: Jet Force Gemini upstream efd5abb advanced this "
        "shared-engine TU past the reference this plateau was filed against. "
        "Re-derive with a PROVENANCE note; Mickey ROM decides disagreements."
    )
    ALSO_AFFIRMATIVE = (
        "New donor evidence: Jet Force Gemini upstream efd5abb newly matches "
        "this shared-engine TU. Re-derive from that donor with a PROVENANCE "
        "note; Mickey ROM decides every disagreement."
    )
    DISCLAIMER = (
        "No donor here: JFG src/fx.c is all GLOBAL_ASM. True mechanism: "
        "decompiler-assisted reconstruction. At delta -8 bytes the candidate "
        "is not the same function; no permutation closes it."
    )

    def test_affirmative_reasons_are_claims(self):
        self.assertTrue(guard.claims_donor(self.AFFIRMATIVE))
        self.assertTrue(guard.claims_donor(self.ALSO_AFFIRMATIVE))

    def test_disclaimer_naming_the_donor_is_not_a_claim(self):
        # The regression that motivated this test: an earlier detector keyed
        # on a bare "JFG" and so read every restatement it had just provoked
        # as a fresh claim, making the guard impossible to satisfy.
        self.assertFalse(guard.claims_donor(self.DISCLAIMER))

    def test_empty_lever_phrasing_is_not_a_claim(self):
        self.assertFalse(
            guard.claims_donor(
                "The JFG donor lever is empty here; routing to the permuter."
            )
        )

    def test_unrelated_mechanisms_are_not_claims(self):
        self.assertFalse(
            guard.claims_donor(
                "ADR 0018 stall rule: this plateau closed under the retired "
                "ten-attempt cap while still improving."
            )
        )


class DonorShapeTests(unittest.TestCase):
    """Counting function bodies in a donor translation unit."""

    def shape(self, text: str):
        with TemporaryDirectory() as tmp:
            path = Path(tmp) / "donor.c"
            path.write_text(text)
            return guard.donor_shape(path)

    def test_pragma_only_file_has_no_bodies(self):
        self.assertEqual(
            self.shape('#pragma GLOBAL_ASM("asm/a.s")\n'
                       '#pragma GLOBAL_ASM("asm/b.s")\n'),
            (0, 2),
        )

    def test_top_level_brace_counts_as_a_body(self):
        self.assertEqual(self.shape("void f(void) {\n    g();\n}\n"), (1, 0))

    def test_aggregate_initializer_is_not_a_body(self):
        # An initializer closes with `};`, a function with `}`. Conflating
        # them would report a data-only donor as having C to adapt.
        self.assertEqual(self.shape("int t[] = {\n    1, 2,\n};\n"), (0, 0))

    def test_mixed_file_reports_both_counts(self):
        self.assertEqual(
            self.shape(
                'void f(void) {\n}\n#pragma GLOBAL_ASM("asm/b.s")\n'
                "int g(void) {\n    return 1;\n}\n"
            ),
            (2, 1),
        )

    def test_missing_file_is_distinguishable_from_an_empty_one(self):
        with TemporaryDirectory() as tmp:
            self.assertIsNone(guard.donor_shape(Path(tmp) / "absent.c"))


class CounterpartTests(unittest.TestCase):
    def test_our_main_tu_maps_to_the_donor_top_level_source(self):
        self.assertEqual(
            guard.counterpart(Path("src/main/fx.c")).name, "fx.c"
        )
        self.assertEqual(
            guard.counterpart(Path("src/main/fx.c")).parent.name, "src"
        )

class SplitTranslationUnitTests(unittest.TestCase):
    """A unit this project split still has a donor counterpart."""

    def test_a_split_unit_maps_to_the_donor_file_that_holds_it(self):
        # JFG keeps the whole front end in one src/menu.c; Mickey splits it
        # across menu.c, menu_3B1A0.c and two frontend_*.c files. Refusing a
        # true claim is as bad as accepting a false one.
        for ours in ("src/main/frontend_37D50.c", "src/main/frontend_37680.c",
                     "src/main/menu_3B1A0.c"):
            self.assertEqual(
                guard.counterpart(guard.REPO / ours).name, "menu.c",
                f"{ours} should map to the donor's menu.c",
            )

    def test_an_unlisted_unit_still_uses_the_basename_rule(self):
        self.assertEqual(
            guard.counterpart(guard.REPO / "src/main/camera.c").name, "camera.c"
        )

    def test_the_override_is_keyed_repo_relative_not_absolute(self):
        # The first draft keyed on str(path) and silently never matched,
        # because callers pass an absolute path.
        self.assertTrue(all(not k.startswith("/") for k in guard.TU_COUNTERPARTS))


if __name__ == "__main__":
    unittest.main()
