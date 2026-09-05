#!/usr/bin/env python3
"""Synthetic-only prepared-context comparison regressions."""
import unittest
from unittest import mock

import candidate_context as cc

BASE = b'''typedef int count_t;
struct Packet { int count; float weight; };
extern void callee(struct Packet *p, float rate);
static int setting = 4;
static inline int helper(int x) { return x + 1; }
int target(struct Packet *p) { callee(p, 1.0f); return p->count; }
'''


class ContextTests(unittest.TestCase):
    def compare(self, winner, baseline=BASE):
        return cc.compare_context(baseline, winner, "target")

    def test_body_only_is_unchanged(self):
        report = self.compare(BASE.replace(b"return p->count;", b"int x = p->count; return x + 1;"))
        self.assertEqual(report["status"], "unchanged")
        self.assertNotEqual(report["baseline_sha256"], report["winner_sha256"])
        self.assertEqual(report["baseline_context_sha256"], report["winner_context_sha256"])

    def test_comments_whitespace_and_line_markers_are_ignored(self):
        winner = b'# 20 "other.c"\n/* note */\n' + BASE.replace(b"extern void", b"extern /* signature */ void")
        self.assertEqual(self.compare(winner)["status"], "unchanged")

    def test_return_abi_change_is_named(self):
        report = self.compare(BASE.replace(b"extern void callee", b"extern int callee"))
        self.assertEqual(report["status"], "changed")
        self.assertEqual(len(report["changes"]), 1)
        self.assertEqual(report["changes"][0]["before"]["name"], "callee")
        self.assertIn("extern int", report["changes"][0]["after"]["text"])

    def test_all_non_body_surfaces_are_checked(self):
        variants = [
            (b"float rate", b"int rate"),
            (b"typedef int", b"typedef unsigned int"),
            (b"int count; float weight;", b"float weight; int count;"),
            (b"static int setting = 4", b"static int setting = 5"),
            (b"static int setting", b"extern int setting"),
            (b"return x + 1", b"return x + 2"),
            (b"int target", b"void target"),
            (b"target(struct Packet *p)", b"target(const struct Packet *p)"),
            (b"float rate);", b"float rate, ...);"),
        ]
        for old, new in variants:
            with self.subTest(new=new):
                self.assertEqual(self.compare(BASE.replace(old, new))["status"], "changed")

    def test_old_style_parameter_declarations_are_context(self):
        baseline = b"int target(x) int x; { return x; }"
        self.assertEqual(self.compare(baseline.replace(b"int x;", b"float x;"), baseline)["status"], "changed")

    def test_deletion_insertion_and_order_are_checked(self):
        first = b"typedef int count_t;\n"
        for winner in (BASE.replace(first, b""), first + BASE, BASE.replace(first, b"") + first):
            with self.subTest(winner=winner):
                self.assertEqual(self.compare(winner)["status"], "changed")

    def test_pragma_is_not_discarded(self):
        self.assertEqual(self.compare(b"#pragma pack(1)\n" + BASE)["status"], "changed")

    def test_target_body_pragma_cannot_escape_context_comparison(self):
        baseline = b"int target(void) { return 0; }\nstruct Later { char a; int b; };"
        winner = baseline.replace(b"return 0;", b"\n#pragma pack(1)\nreturn 0;")
        self.assertEqual(self.compare(winner, baseline)["status"], "unverifiable")
        self.assertEqual(self.compare(winner, winner)["status"], "unverifiable")

    def test_phase_two_splicing_precedes_line_comment_handling(self):
        baseline = b"extern int shared;\nint target(void) { return 0; }"
        for ending in (b"\n", b"\r\n"):
            winner = b"// hidden \\" + ending + baseline
            self.assertEqual(self.compare(winner, baseline)["status"], "changed")
        self.assertEqual(self.compare(BASE.replace(b"extern void", b"extern \\\nvoid"))["status"], "unchanged")

    def test_unexpanded_location_macros_fail_closed(self):
        for name in cc.LOCATION_MACROS:
            baseline = f"#line 1\nstatic int value = {name};\nint target(void) {{ return 0; }}".encode()
            winner = baseline.replace(b"#line 1", b"#line 9")
            self.assertEqual(self.compare(winner, baseline)["status"], "unverifiable")
        literal = b'char *note = "__LINE__"; int target(void) { return 0; }'
        self.assertEqual(self.compare(literal, literal)["status"], "unchanged")

    def test_trigraph_splicing_fails_closed(self):
        baseline = b"extern int shared;\nint target(void) { return 0; }"
        self.assertEqual(self.compare(b"// hidden ??/\n" + baseline, baseline)["status"], "unverifiable")

    def test_pragma_operators_are_not_hidden_as_body_calls(self):
        for operator in cc.PRAGMA_OPERATORS:
            baseline = b"int target(void) { return 0; }\nstruct Later { char a; int b; };"
            winner = baseline.replace(b"return 0;", f'{operator}("pack(1)"); return 0;'.encode())
            self.assertEqual(self.compare(winner, baseline)["status"], "unverifiable")

    def test_lone_cr_ends_comment_before_context_declaration(self):
        baseline = b"int target(void) { return 0; }\n"
        winner = b"// hidden\rextern int added;\n" + baseline
        self.assertEqual(self.compare(winner, baseline)["status"], "changed")

    def test_string_literals_are_not_stripped_as_comments(self):
        baseline = b'char *note = "/* old */"; int target(void) { return 0; }'
        self.assertEqual(self.compare(baseline.replace(b"old", b"new"), baseline)["status"], "changed")

    def test_missing_duplicate_wrong_target_fail_closed(self):
        for winner in (BASE.replace(b"int target", b"int elsewhere"), BASE + b"int target(void) { return 1; }", b""):
            with self.subTest(winner=winner):
                self.assertEqual(self.compare(winner)["status"], "unverifiable")

    def test_invalid_or_unprepared_c_fails_closed(self):
        for winner in (b"#include <missing.h>\n" + BASE, b"#define rate 2\n" + BASE,
                       b"#if 1\n" + BASE, BASE + b"bad syntax!!", b"\xff", BASE + b"/* unterminated"):
            with self.subTest(winner=winner):
                self.assertEqual(self.compare(winner)["status"], "unverifiable")

    def test_no_typedef_state_leaks_between_inputs_or_calls(self):
        self.assertEqual(self.compare(BASE.replace(b"typedef int count_t;", b"count_t x;"))["status"], "unverifiable")
        self.assertEqual(self.compare(BASE)["status"], "unchanged")

    def test_limits_fail_closed(self):
        with mock.patch.object(cc, "MAX_SOURCE_BYTES", 5):
            self.assertEqual(self.compare(BASE)["status"], "unverifiable")
        with mock.patch.object(cc, "MAX_NODES", 2):
            self.assertEqual(self.compare(BASE)["status"], "unverifiable")
        with mock.patch.object(cc, "MAX_DEPTH", 1):
            self.assertEqual(self.compare(BASE)["status"], "unverifiable")

    def test_report_truncation_never_turns_changes_into_success(self):
        baseline = b"int target(void) { return 0; }" + b"\n".join(f"int field{i} = 1;".encode() for i in range(40))
        report = self.compare(baseline.replace(b" = 1", b" = 2"), baseline)
        self.assertEqual(report["status"], "changed")
        self.assertTrue(report["truncated"])
        self.assertEqual(len(report["changes"]), cc.MAX_CHANGES)

    def test_identity_binds_parser_and_implementation(self):
        value = cc.identity()
        self.assertEqual(value["parser"], "pycparser")
        self.assertEqual(len(value["parser_sha256"]), 64)
        self.assertEqual(len(value["comparator_sha256"]), 64)


if __name__ == "__main__":
    unittest.main()
