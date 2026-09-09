#!/usr/bin/env python3
"""Unit tests for pairing a friendly-named candidate with its target asm."""

import unittest

import resolve_target_asm as resolver


GUARD = '''#include "common.h"

#ifdef NON_MATCHING
void firstFunction(Actor *actor) {
    doThing(actor);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/o001/tail/func_overlay_001_AAA.s")
#endif

#ifdef NON_MATCHING
s32 secondFunction(Actor *actor, s32 mode) {
    return mode;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/o001/tail/func_overlay_001_BBB.s")
#endif
'''


class PairingTests(unittest.TestCase):
    def test_each_function_resolves_to_the_pragma_in_its_own_guard(self):
        # The regression: with twelve pragmas in one unit, the old sole-pragma
        # rule resolved none of them.
        self.assertEqual(
            resolver.paired_target(GUARD, "firstFunction"),
            "asm/nonmatchings/o001/tail/func_overlay_001_AAA.s",
        )
        self.assertEqual(
            resolver.paired_target(GUARD, "secondFunction"),
            "asm/nonmatchings/o001/tail/func_overlay_001_BBB.s",
        )

    def test_an_absent_symbol_resolves_to_nothing(self):
        self.assertIsNone(resolver.paired_target(GUARD, "thirdFunction"))

    def test_a_definition_outside_any_guard_resolves_to_nothing(self):
        text = 'void plain(void) {\n}\n#pragma GLOBAL_ASM("asm/x.s")\n'
        self.assertIsNone(resolver.paired_target(text, "plain"))

    def test_a_guard_with_no_pragma_resolves_to_nothing(self):
        text = "#ifdef NON_MATCHING\nvoid f(void) {\n}\n#endif\n"
        self.assertIsNone(resolver.paired_target(text, "f"))

    def test_a_nested_conditional_does_not_capture_the_definition(self):
        # A nested #if inside the body must not be read as the function's own
        # guard, or the pragma below it is missed.
        text = (
            "#ifdef NON_MATCHING\n"
            "void f(void) {\n"
            "#if VERSION_US\n"
            "    us();\n"
            "#endif\n"
            "}\n"
            "#else\n"
            '#pragma GLOBAL_ASM("asm/right.s")\n'
            "#endif\n"
        )
        self.assertEqual(resolver.paired_target(text, "f"), "asm/right.s")

    def test_a_call_site_is_not_mistaken_for_a_definition(self):
        # A definition starts in column zero; a call is indented.
        text = (
            "#ifdef NON_MATCHING\n"
            "void caller(void) {\n"
            "    target(1);\n"
            "}\n"
            "#else\n"
            '#pragma GLOBAL_ASM("asm/caller.s")\n'
            "#endif\n"
        )
        self.assertIsNone(resolver.paired_target(text, "target"))

    def test_a_similar_name_is_not_matched(self):
        self.assertIsNone(resolver.paired_target(GUARD, "first"))


class BlockTests(unittest.TestCase):
    def test_blocks_are_paired_innermost_first(self):
        lines = ["#ifdef A", "#if B", "#endif", "#endif"]
        self.assertEqual(
            sorted(resolver.conditional_blocks(lines)), [(0, 3), (1, 2)]
        )

    def test_an_unclosed_conditional_is_ignored_rather_than_crashing(self):
        self.assertEqual(resolver.conditional_blocks(["#ifdef A", "int x;"]), [])


if __name__ == "__main__":
    unittest.main()
