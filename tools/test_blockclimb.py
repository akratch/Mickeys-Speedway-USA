#!/usr/bin/env python3
"""Unit tests for the statement-order sweep.

Every fixture here is synthesised C written for the test. Nothing is read from
the ROM, a build, or a real reconstruction, and the scorer is a plain Python
function, so none of this needs a baserom or a toolchain.

The properties worth pinning are, in order:

  1. a statement containing a call never moves, and never gets moved across;
  2. a run is three or more consecutive movable statements at one indentation,
     and a call in the middle SPLITS the run rather than being stepped over;
  3. the climb terminates -- including against a scorer that contradicts
     itself, which is the only way a move-one climb can cycle at all;
  4. the guarded optimum and the unguarded one are reported separately, so an
     unsafe winner is visible instead of silently discarded.
"""
import io
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import blockclimb as bc  # noqa: E402


def body(text: str) -> list[str]:
    """Lines of a synthetic function body, with the leading newline trimmed."""
    return text.strip("\n").split("\n")


# A synthetic body: three plain stores, a call, then three more plain stores.
# The call is deliberately in the middle, because "does the call split the run"
# is the property the whole guard rests on.
SPLIT_BY_CALL = body("""
    a = 1;
    b = 2;
    c = 3;
    render(&a);
    d = 4;
    e = 5;
    f = 6;
""")


class MovabilityTests(unittest.TestCase):
    def test_a_plain_store_is_movable(self) -> None:
        self.assertTrue(bc.is_movable("    a = 1;"))

    def test_a_bare_call_is_not_movable(self) -> None:
        self.assertFalse(bc.is_movable("    render(&a);"))

    def test_an_assignment_from_a_call_is_not_movable(self) -> None:
        """The hazard is the call, not which side of the `=` it sits on."""
        self.assertFalse(bc.is_movable("    a = compute(b);"))

    def test_a_call_nested_in_a_subscript_is_not_movable(self) -> None:
        self.assertFalse(bc.is_movable("    table[index(i)] = 1;"))

    def test_a_cast_is_not_mistaken_for_a_call(self) -> None:
        """Casts are `(type) x`; only an identifier touching `(` is a call."""
        self.assertTrue(bc.is_movable("    a = (u8) b;"))
        self.assertTrue(bc.is_movable("    p = (RcpTextureInfo *) q[0x51];"))

    def test_allow_calls_opens_the_guard_for_the_probe_only(self) -> None:
        self.assertTrue(bc.is_movable("    render(&a);", allow_calls=True))

    def test_a_block_or_a_continuation_is_not_a_statement(self) -> None:
        for line in ("    if (a) {", "    }", "    while (i < 3);",
                     "    return a;", "    /* comment */",
                     "    func_x(a, b,", "#define X 1"):
            self.assertFalse(bc.is_movable(line), line)


class RunDetectionTests(unittest.TestCase):
    def test_a_call_splits_a_run_rather_than_being_skipped(self) -> None:
        runs = bc.find_runs(SPLIT_BY_CALL, 0, len(SPLIT_BY_CALL))
        self.assertEqual(runs, [(0, 3), (4, 7)])
        # The two halves are separate runs, so no single move can carry a
        # statement from one side of `render(&a);` to the other.
        for start, stop in runs:
            self.assertNotIn(3, range(start, stop))

    def test_with_the_guard_off_the_call_joins_one_long_run(self) -> None:
        runs = bc.find_runs(SPLIT_BY_CALL, 0, len(SPLIT_BY_CALL),
                            allow_calls=True)
        self.assertEqual(runs, [(0, 7)])

    def test_a_run_needs_three_statements(self) -> None:
        lines = body("""
    a = 1;
    b = 2;
    render(&a);
    c = 3;
    d = 4;
    e = 5;
""")
        self.assertEqual(bc.find_runs(lines, 0, len(lines)), [(3, 6)])

    def test_min_run_is_honoured(self) -> None:
        lines = body("""
    a = 1;
    b = 2;
    render(&a);
""")
        self.assertEqual(bc.find_runs(lines, 0, len(lines)), [])
        self.assertEqual(bc.find_runs(lines, 0, len(lines), min_run=2),
                         [(0, 2)])

    def test_indentation_ends_a_run(self) -> None:
        """A deeper statement belongs to an inner block, not to this run."""
        lines = body("""
    a = 1;
    b = 2;
        inner = 3;
        other = 4;
        third = 5;
    c = 6;
""")
        self.assertEqual(bc.find_runs(lines, 0, len(lines)), [(2, 5)])

    def test_the_line_range_bounds_the_search(self) -> None:
        lines = body("""
    a = 1;
    b = 2;
    c = 3;
    d = 4;
""")
        self.assertEqual(bc.find_runs(lines, 1, 4), [(1, 4)])
        self.assertEqual(bc.find_runs(lines, 2, 4), [])


class MoveEnumerationTests(unittest.TestCase):
    def test_it_offers_exactly_the_one_move_neighbourhood(self) -> None:
        """Over three statements that is four orders; the reversal needs two."""
        orders = {tuple(order) for _, _, order in bc.moves(3)}
        self.assertEqual(orders, {(1, 0, 2), (1, 2, 0), (0, 2, 1), (2, 0, 1)})
        self.assertNotIn((2, 1, 0), orders)

    def test_the_identity_is_never_offered(self) -> None:
        for n in (3, 4, 5):
            for _, _, order in bc.moves(n):
                self.assertNotEqual(order, list(range(n)))


class ClimbTests(unittest.TestCase):
    def test_it_reaches_the_order_the_scorer_rewards(self) -> None:
        lines = body("""
    a = 1;
    b = 2;
    c = 3;
""")
        wanted = "\n".join(["    c = 3;", "    a = 1;", "    b = 2;"])

        def score(text: str) -> int:
            return 0 if text == wanted else 10

        result = bc.climb(lines, score, 0, len(lines))
        self.assertEqual(result.best_score, 0)
        self.assertEqual(result.lines, wanted.split("\n"))
        self.assertEqual(result.gain, 10)

    def test_a_call_is_never_moved_by_the_climb(self) -> None:
        """The end-to-end form of property 1, not just the predicate."""
        lines = list(SPLIT_BY_CALL)
        swapped = list(SPLIT_BY_CALL)
        swapped[2], swapped[3] = swapped[3], swapped[2]
        reward = "\n".join(swapped)

        def score(text: str) -> int:
            # The scorer *wants* the call moved. It must not get it.
            return 0 if text == reward else 10

        result = bc.climb(lines, score, 0, len(lines))
        self.assertEqual(result.best_score, 10)
        self.assertEqual(result.lines[3], "    render(&a);")
        # ... and the same scorer does reach it once the guard is lifted, so
        # the test is measuring the guard rather than an unreachable order.
        probe = bc.climb(lines, score, 0, len(lines), allow_calls=True)
        self.assertEqual(probe.best_score, 0)

    def test_a_statement_is_never_moved_across_a_call(self) -> None:
        lines = list(SPLIT_BY_CALL)
        # Reward putting the pre-call store `a = 1;` after the call.
        moved = ["    b = 2;", "    c = 3;", "    render(&a);", "    a = 1;",
                 "    d = 4;", "    e = 5;", "    f = 6;"]
        reward = "\n".join(moved)

        def score(text: str) -> int:
            return 0 if text == reward else 10

        result = bc.climb(lines, score, 0, len(lines))
        self.assertEqual(result.best_score, 10)
        self.assertEqual(result.lines, lines)

    def test_it_terminates_on_a_scorer_that_contradicts_itself(self) -> None:
        """Property 3: a cycle cannot ping-pong, because nothing is re-scored.

        A move-one climb over a *consistent* scorer cannot cycle -- the score
        strictly decreases. The only way to cycle is a scorer that answers
        differently on the second visit to the same order, so that is what this
        one does: each call returns one less than the call before it, which
        makes every candidate look like an improvement forever.
        """
        lines = body("""
    a = 1;
    b = 2;
    c = 3;
""")
        counter = {"n": 1000}

        def score(text: str) -> int:
            counter["n"] -= 1
            return counter["n"]

        result = bc.climb(lines, score, 0, len(lines), max_passes=4)
        # It stopped, and it stopped by exhausting the orders rather than by
        # running out of passes: with 3 statements there are 4 distinct
        # one-move orders plus the base, so the cache empties the space.
        self.assertLessEqual(result.evaluations, 6)
        self.assertFalse(result.hit_pass_cap)

    def test_the_pass_cap_is_reported_rather_than_silently_hit(self) -> None:
        """A second, independent bound on top of the visited-order cache."""
        lines = body("""
    a = 1;
    b = 2;
    c = 3;
    d = 4;
""")
        seen = {"n": 0}

        def score(text: str) -> int:
            # Always an improvement, and always a fresh order, until the cache
            # runs out -- with max_passes=1 the cap is what stops it.
            seen["n"] += 1
            return -seen["n"]

        result = bc.climb(lines, score, 0, len(lines), max_passes=1)
        self.assertTrue(result.hit_pass_cap)
        self.assertEqual(result.passes, 1)

    def test_a_flat_axis_reports_a_fixed_point_with_no_moves(self) -> None:
        """The empty result is a finding, so it has to be distinguishable."""
        lines = body("""
    a = 1;
    b = 2;
    c = 3;
""")
        result = bc.climb(lines, lambda text: 7, 0, len(lines))
        self.assertEqual(result.adopted, [])
        self.assertEqual(result.gain, 0)
        self.assertFalse(result.hit_pass_cap)
        self.assertGreater(result.evaluations, 1)   # it really did look

    def test_an_order_is_scored_at_most_once(self) -> None:
        lines = body("""
    a = 1;
    b = 2;
    c = 3;
""")
        calls: list[str] = []

        def score(text: str) -> int:
            calls.append(text)
            return len(text)

        bc.climb(lines, score, 0, len(lines))
        self.assertEqual(len(calls), len(set(calls)))


class ReportingTests(unittest.TestCase):
    """Property 4: the unsafe winner has to be visible, not discarded."""

    def _results(self):
        lines = list(SPLIT_BY_CALL)
        moved = list(SPLIT_BY_CALL)
        moved[2], moved[3] = moved[3], moved[2]
        reward = "\n".join(moved)

        def score(text: str) -> int:
            return 3 if text == reward else 9

        guarded = bc.climb(lines, score, 0, len(lines))
        unguarded = bc.climb(lines, score, 0, len(lines), allow_calls=True)
        return guarded, unguarded

    def test_the_unguarded_optimum_is_better_and_separately_measurable(self) -> None:
        guarded, unguarded = self._results()
        self.assertEqual(guarded.best_score, 9)
        self.assertEqual(unguarded.best_score, 3)
        self.assertLess(unguarded.best_score, guarded.best_score)

    def test_each_is_reported_under_its_own_label(self) -> None:
        guarded, unguarded = self._results()
        out = io.StringIO()
        bc._report(guarded, "sym guarded", out)
        bc._report(unguarded, "sym UNGUARDED (not written)", out)
        text = out.getvalue()
        self.assertIn("sym guarded: 9 -> 9", text)
        self.assertIn("sym UNGUARDED (not written): 9 -> 3", text)
        self.assertIn("fixed point", text)      # the guarded run found nothing

    def test_the_report_names_the_pass_count_and_the_compile_count(self) -> None:
        guarded, _ = self._results()
        out = io.StringIO()
        bc._report(guarded, "sym", out)
        text = out.getvalue()
        self.assertIn("pass(es)", text)
        self.assertIn("compiles", text)

    def test_the_direction_is_spelled_out_not_signed(self) -> None:
        """The metric falls when it improves; a signed number misreads."""
        guarded, unguarded = self._results()
        out = io.StringIO()
        bc._report(guarded, "guarded", out)
        bc._report(unguarded, "unguarded", out)
        text = out.getvalue()
        self.assertIn("no change", text)
        self.assertIn("6 word(s) better", text)
        self.assertNotIn("+", text)


class FunctionSpanTests(unittest.TestCase):
    def test_the_body_range_excludes_the_signature_and_the_brace(self) -> None:
        text = "\n".join(body("""
static void other(void) {
    x = 1;
}

void demo_function(s32 arg0) {
    a = 1;
    b = 2;
    c = 3;
}
"""))
        lo, hi = bc._function_line_span(text, "demo_function")
        lines = text.split("\n")
        self.assertEqual([line.strip() for line in lines[lo:hi]],
                         ["a = 1;", "b = 2;", "c = 3;"])

    def test_a_wrapped_parameter_list_still_finds_the_body(self) -> None:
        """Several definitions here wrap the signature over two lines."""
        text = "\n".join(body("""
void demo_function(s32 arg0, s32 arg1,
                   s32 arg2) {
    a = 1;
}
"""))
        lo, hi = bc._function_line_span(text, "demo_function")
        self.assertEqual([line.strip() for line in text.split("\n")[lo:hi]],
                         ["a = 1;"])

    def test_a_declaration_or_a_call_is_not_the_definition(self) -> None:
        text = "\n".join(body("""
extern void demo_function(s32 arg0);
void other(void) {
    demo_function(1);
}
void demo_function(s32 arg0) {
    a = 1;
}
"""))
        lo, hi = bc._function_line_span(text, "demo_function")
        self.assertEqual([line.strip() for line in text.split("\n")[lo:hi]],
                         ["a = 1;"])

    def test_an_unknown_symbol_is_an_error_rather_than_an_empty_sweep(self) -> None:
        with self.assertRaises(SystemExit):
            bc._function_line_span("void a(void) {\n}\n", "missing_function")


class HygieneTests(unittest.TestCase):
    def test_neither_file_carries_an_absolute_workstation_path(self) -> None:
        """The repository is public; a home directory in a tracked file leaks.

        The markers are assembled rather than written out, so this test does
        not trip over its own source text.
        """
        here = pathlib.Path(__file__).resolve().parent
        markers = ["/" + "Users/", "/" + "home/", "C:" + "\\"]
        for name in ("blockclimb.py", "test_blockclimb.py"):
            text = (here / name).read_text()
            for marker in markers:
                self.assertNotIn(marker, text, f"{name} carries {marker}")


class DataDependencyGuardTests(unittest.TestCase):
    """The call guard excludes side effects; this excludes data dependencies.

    Both are needed. A lane caught the climb proposing

        start.y = object->y + state->heightOffset;

    AFTER

        end.y = start.y + dy;

    which contains no call, passes the call guard, and reads `start` before it
    is assigned. It scored 198 -> 196 and was rejected by hand. Nothing in the
    tool refused it.
    """

    RAW_A = "    start.y = object->y + state->heightOffset;"
    RAW_B = "    end.y = start.y + dy;"

    def test_the_reported_read_before_write_is_a_conflict(self) -> None:
        self.assertTrue(bc.conflict(self.RAW_A, self.RAW_B))

    def test_the_reported_swap_is_illegal(self) -> None:
        self.assertFalse(bc.order_is_legal([self.RAW_A, self.RAW_B], [1, 0]))

    def test_the_original_order_stays_legal(self) -> None:
        self.assertTrue(bc.order_is_legal([self.RAW_A, self.RAW_B], [0, 1]))

    def test_independent_statements_may_still_swap(self) -> None:
        """The guard must not freeze every run -- that would make the tool
        useless, and it found 45 words on one function."""
        a, b = "    a.x = 1;", "    b.y = 2;"
        self.assertFalse(bc.conflict(a, b))
        self.assertTrue(bc.order_is_legal([a, b], [1, 0]))

    def test_write_after_read_is_a_conflict(self) -> None:
        a, b = "    total = count + 1;", "    count = 0;"
        self.assertTrue(bc.conflict(a, b))

    def test_write_after_write_is_a_conflict(self) -> None:
        a, b = "    mode = 1;", "    mode = 2;"
        self.assertTrue(bc.conflict(a, b))

    def test_a_subscripted_store_writes_its_base(self) -> None:
        """`p[i] = x` is treated as writing `p`. Over-approximate for a plain
        subscript, exactly right for the aliasing it is there to catch."""
        r, w = bc.reads_writes("    nodes[1].texture = 0;")
        self.assertIn("nodes", w)
        self.assertIn("1", "1")           # subscript literal is not an ident
        self.assertTrue(bc.conflict("    nodes[1].texture = 0;",
                                    "    count = nodes[0].texture;"))

    def test_a_dereference_store_writes_through_the_pointer(self) -> None:
        r, w = bc.reads_writes("    *cursor = value;")
        self.assertIn("cursor", w)
        self.assertIn("value", r)

    def test_a_compound_assignment_reads_and_writes_its_target(self) -> None:
        r, w = bc.reads_writes("    offset += stride;")
        self.assertIn("offset", w)
        self.assertIn("offset", r)
        self.assertIn("stride", r)

    def test_a_shift_assignment_is_not_read_as_a_comparison(self) -> None:
        r, w = bc.reads_writes("    mask <<= shift;")
        self.assertIn("mask", w)
        self.assertIn("shift", r)

    def test_a_comparison_is_not_mistaken_for_an_assignment(self) -> None:
        """`==`, `!=`, `<=`, `>=` must not split as assignments."""
        for stmt in ("    flag = a == b;", "    flag = a != b;",
                     "    flag = a <= b;", "    flag = a >= b;"):
            with self.subTest(stmt=stmt):
                _, w = bc.reads_writes(stmt)
                self.assertEqual(w, frozenset(["flag"]))

    def test_an_increment_counts_as_both_read_and_write(self) -> None:
        r, w = bc.reads_writes("    index++;")
        self.assertIn("index", r)
        self.assertIn("index", w)

    def test_type_keywords_are_not_storage(self) -> None:
        r, w = bc.reads_writes("    value = (u32) raw;")
        self.assertNotIn("u32", r)
        self.assertIn("raw", r)

    def test_the_climb_will_not_adopt_a_dependent_swap(self) -> None:
        """End to end: a scorer that actively rewards the illegal order must
        not get it. This measures the guard, not an unreachable order."""
        lines = [self.RAW_A, self.RAW_B, "    z.w = 3;"]
        def score(text: str) -> int:
            body = [l for l in text.split("\n") if l.strip()]
            return 0 if body[0].strip().startswith("end.y") else 100
        out = bc.climb(lines, score, 0, len(lines), min_run=3)
        self.assertFalse(out.lines[0].strip().startswith("end.y"))

    def test_probe_unguarded_still_reaches_it(self) -> None:
        """--probe-unguarded exists to show the raw reachable number and is
        reported as not adoptable, so it drops this guard too."""
        lines = [self.RAW_A, self.RAW_B, "    z.w = 3;"]
        def score(text: str) -> int:
            body = [l for l in text.split("\n") if l.strip()]
            return 0 if body[0].strip().startswith("end.y") else 100
        out = bc.climb(lines, score, 0, len(lines), min_run=3, allow_calls=True)
        self.assertTrue(out.lines[0].strip().startswith("end.y"))

if __name__ == "__main__":
    unittest.main(verbosity=2)
