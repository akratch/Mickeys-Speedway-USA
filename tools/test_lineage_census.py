#!/usr/bin/env python3
"""Tests for the split-decision and creation-order census.

Nothing here compiles. The fixtures use the grammar the instrumented uopt
emits, and what is pinned is the reading -- a wrong census here would send a
lane at the wrong axis on the tree's most expensive function:

  1. a lineage joins to its webs by (table, chain), which is the only key that
     covers every web the detail table reports;
  2. another procedure's rows never leak in, since a TU compiles several and the
     ordinal is Ucode order, not source order;
  3. `bb` is the positional field, NOT `line` -- the member `line` reads a
     constant on the real procedure, so a census that attributed a lineage to it
     would be reporting nothing at all;
  4. an edit that changes neither creation order nor split structure is reported
     as reaching nothing, because that is the finding.
"""
import json
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import lineage_census as lc  # noqa: E402


def detail(web, table, chain, bb=5, typ=3, proc=0):
    return (f"[CDX] webdetail phase=p1 proc={proc} role=target web={web} "
            f"sym={web} type={typ} dtype=0 table={table} chain={chain} "
            f"exprtable=-1 exprchain=-1 bb={bb} line=-1")


def dec(web, decision="color", numintf=10, regsleft=12, proc=0,
        forbidden0=0):
    return (f"[CDX] p1dec phase=p1 proc={proc} web={web} sym={web} class=1 "
            f"save=1.0 nocs=1 totalsave=1.0 bestcost=0.0 bestcolor=14 "
            f"bestreg=s0 forbidden0={forbidden0:#010x} forbidden1=0x00000000 "
            f"regsleft={regsleft} numintf={numintf} available0=0x0003fe00 "
            f"available1=0x00000000 allcallersave=0 taken1=-1 taken2=-1 "
            f"decision={decision} forced=-2")


def colour(web, c=14, reg="s0", proc=0):
    return (f"[CDX] p1color phase=p1 proc={proc} web={web} sym={web} "
            f"color={c} reg={reg} forced=-2")


def rng(event, table, chain, typ=3, proc=0):
    return (f"[CDX] lineage_range proc={proc} event={event} table={table} "
            f"chain={chain} type={typ} dtype=0 sym={table} exprtable=-1 "
            f"exprchain=-1")


def mem(event, table, chain, bb, proc=0, line=8):
    return (f"[CDX] lineage_member proc={proc} event={event} table={table} "
            f"chain={chain} type=3 dtype=0 sym={table} bb={bb} line={line} "
            f"flags=0,0,0,0,0,0")


class ParsingTests(unittest.TestCase):
    def test_each_record_kind_lands_in_its_own_bucket(self):
        text = "\n".join([detail(7, 100, 0), dec(7), colour(7),
                          rng(0, 100, 0), mem(1, 100, 0, 5)])
        p = lc.parse(text, 0)
        self.assertEqual(list(p["detail"]), [7])
        self.assertEqual(list(p["decision"]), [7])
        self.assertEqual(list(p["colour"]), [7])
        self.assertEqual(len(p["ranges"]), 1)
        self.assertEqual(len(p["members"]), 1)

    def test_another_procedures_rows_are_not_mixed_in(self):
        text = "\n".join([detail(7, 100, 0, proc=0), detail(9, 200, 0, proc=1),
                          rng(0, 100, 0, proc=0), rng(2, 200, 0, proc=1)])
        self.assertEqual(list(lc.parse(text, 0)["detail"]), [7])
        self.assertEqual(len(lc.parse(text, 1)["ranges"]), 1)
        self.assertEqual(lc.parse(text, 2)["ranges"], [])

    def test_unrelated_rows_are_ignored(self):
        p = lc.parse("[CDX] p1cand phase=p1 proc=0 web=0\n[CDX] globalcolor proc=0", 0)
        self.assertEqual(p["ranges"], [])
        self.assertEqual(p["detail"], {})


class LineageJoinTests(unittest.TestCase):
    def build(self, *rows):
        return lc.lineages(lc.parse("\n".join(rows), 0))

    def test_a_lineage_collects_the_webs_that_share_its_table_and_chain(self):
        lin = self.build(rng(0, 100, 0),
                         detail(7, 100, 0), detail(9, 100, 0), detail(11, 200, 0),
                         dec(7), dec(9), dec(11))
        self.assertEqual(len(lin), 1)
        self.assertEqual(lin[0]["webs"], [7, 9])

    def test_chain_is_part_of_the_key_not_just_table(self):
        lin = self.build(rng(0, 100, 0), rng(2, 100, 1),
                         detail(7, 100, 0), detail(9, 100, 1), dec(7), dec(9))
        by_chain = {l["chain"]: l["webs"] for l in lin}
        self.assertEqual(by_chain["0"], [7])
        self.assertEqual(by_chain["1"], [9])

    def test_split_and_coloured_members_are_counted_separately(self):
        lin = self.build(rng(0, 100, 0),
                         detail(7, 100, 0), detail(9, 100, 0), detail(11, 100, 0),
                         dec(7, "split"), dec(9, "split"), dec(11, "color"))
        self.assertEqual((lin[0]["split"], lin[0]["coloured"]), (2, 1))

    def test_lineages_come_back_in_creation_order(self):
        lin = self.build(rng(9, 300, 0), rng(0, 100, 0), rng(4, 200, 0))
        self.assertEqual([l["event"] for l in lin], [0, 4, 9])

    def test_blocks_come_from_members_and_are_deduplicated(self):
        lin = self.build(rng(0, 100, 0), mem(1, 100, 0, 15), mem(2, 100, 0, 15),
                         mem(3, 100, 0, 9))
        self.assertEqual(lin[0]["blocks"], [9, 15])
        self.assertEqual(lin[0]["members"], 3)

    def test_the_constant_line_field_is_not_used_as_a_position(self):
        # Every member on the real procedure reads line=8. A census that keyed
        # on it would report one position for the whole function.
        lin = self.build(rng(0, 100, 0), mem(1, 100, 0, 15, line=8),
                         mem(2, 100, 0, 91, line=8))
        self.assertEqual(lin[0]["blocks"], [15, 91])
        self.assertNotIn("lines", lin[0])


class PressureTests(unittest.TestCase):
    def test_webs_are_ordered_by_interference_descending(self):
        p = lc.parse("\n".join([dec(7, numintf=10), dec(9, numintf=140),
                                dec(11, numintf=24)]), 0)
        self.assertEqual([r["web"] for r in lc.pressure(p)], [9, 11, 7])

    def test_a_coloured_web_reports_its_colour_and_a_split_web_does_not(self):
        p = lc.parse("\n".join([dec(7, "color"), colour(7, 16, "s2"),
                                dec(9, "split")]), 0)
        by = {r["web"]: r for r in lc.pressure(p)}
        self.assertEqual((by[7]["colour"], by[7]["reg"]), (16, "s2"))
        self.assertIsNone(by[9]["colour"])
        self.assertEqual(by[9]["decision"], "split")

    def test_room_left_is_carried_through(self):
        p = lc.parse(dec(7, "split", numintf=140, regsleft=6), 0)
        row = lc.pressure(p)[0]
        self.assertEqual((row["numintf"], row["regsleft"]), (140, 6))


class ForbiddenDecodeTests(unittest.TestCase):
    def test_a_forbidden_mask_decodes_to_register_names(self):
        # bits 1 and 15 -> v0 and s0 in the colour order
        fields = {"forbidden0": hex((1 << 1) | (1 << 15)), "forbidden1": "0x0"}
        self.assertEqual(lc.taken_colours(fields), ["v0", "s0"])

    def test_the_high_half_of_the_mask_is_read(self):
        fields = {"forbidden0": "0x0", "forbidden1": "0x4"}
        self.assertEqual(lc.taken_colours(fields), ["c34"])

    def test_an_empty_mask_forbids_nothing(self):
        self.assertEqual(lc.taken_colours({"forbidden0": "0x0"}), [])


def blocks(web, bbs, aux, lr="0x10001000", proc=0):
    return (f"[CDX] webblocks phase=p1 proc={proc} role=target web={web} "
            f"sym={web} lr={lr} bbs={bbs} aux={aux}")


def grow(lr, bb, new, before, after, numintf, accepted, proc=0):
    return (f"[CDX] grow proc={proc} lr={lr} bb={bb} new={new} "
            f"left_before={before} left_after={after} numintf={numintf} "
            f"strict=1\n"
            f"[CDX] growv proc={proc} lr={lr} bb={bb} accepted={accepted}")


class BlockSetTests(unittest.TestCase):
    """The per-web block sets and the split growth, read off the new rows.

    A web number is shared by the parent range and the piece carved from it,
    so the reader must take the LAST webblocks row as the decided piece and
    join the growth rows through that row's live-range pointer -- reading the
    first row attributes the piece's decision to the parent's span.
    """

    def build(self, *rows):
        return lc.parse("\n".join(rows), 0)

    def test_refs_are_the_span_minus_the_pass_through_blocks(self):
        p = self.build(blocks(9, "183,184,190,191,202", "184,191"))
        b = lc.blocks_of(p, 9)
        self.assertEqual(b["refs"], [183, 190, 202])
        self.assertEqual(b["livein"], [184, 191])
        self.assertEqual(b["span"], [183, 184, 190, 191, 202])

    def test_an_empty_set_is_spelled_with_a_dash(self):
        p = self.build(blocks(9, "192", "-"))
        self.assertEqual(lc.blocks_of(p, 9), {
            "web": 9, "lr": "0x10001000", "span": [192], "livein": [],
            "refs": [192], "decisions": 1})

    def test_the_last_row_is_the_decided_piece_not_the_parent(self):
        p = self.build(blocks(9, "91,92,93,183,184", "92", lr="0x10001000"),
                       blocks(9, "183,184", "184", lr="0x10002000"))
        b = lc.blocks_of(p, 9)
        self.assertEqual(b["refs"], [183])
        self.assertEqual(b["lr"], "0x10002000")
        self.assertEqual(b["decisions"], 2)

    def test_growth_is_joined_through_the_pieces_pointer(self):
        p = self.build(blocks(9, "91,183", "-", lr="0x10001000"),
                       "[CDX] seed proc=0 lr=0x10002000 bb=183",
                       grow("0x10002000", 184, 1, 15, 14, 20, 1),
                       grow("0x10002000", 185, 0, 14, 11, 23, 0),
                       grow("0x10001000", 5, 1, 9, 9, 3, 1),
                       blocks(9, "183,184", "184", lr="0x10002000"))
        rows = lc.growth_of(p, 9)
        self.assertEqual([r["event"] for r in rows],
                         ["seed", "grow", "growv", "grow", "growv"])
        self.assertEqual([r["bb"] for r in rows if r["event"] == "grow"],
                         ["184", "185"])

    def test_a_web_without_the_profile_reads_as_missing_not_empty(self):
        p = self.build(detail(9, 1, 0))
        self.assertIsNone(lc.blocks_of(p, 9))
        self.assertEqual(lc.growth_of(p, 9), [])
        self.assertIn("no webblocks row", lc.render_growth(p, 9))

    def test_the_rendering_states_the_test_and_its_margin(self):
        p = self.build(dec(9, numintf=25, regsleft=13), colour(9, 5, "a2"),
                       "[CDX] seed proc=0 lr=0x10002000 bb=183",
                       grow("0x10002000", 185, 0, 14, 11, 23, 0),
                       blocks(9, "183,184", "184", lr="0x10002000"))
        text = lc.render_growth(p, 9)
        self.assertIn("2*left_after >= numintf + new", text)
        self.assertIn("bb= 185 new=0 left 14->11 numintf=23 margin=-1  reject", text)
        self.assertIn("colour=a2", text)

    def test_seedcand_rows_are_kept_out_of_the_growth_listing(self):
        p = self.build("[CDX] seedcand proc=0 lr=0x10002000 pass=1 bb=95 "
                       "f16=0 f18=0 f19=0 f20=0 maskdiff=1",
                       "[CDX] seed proc=0 lr=0x10002000 bb=183",
                       blocks(9, "183", "-", lr="0x10002000"))
        self.assertEqual([r["event"] for r in lc.growth_of(p, 9)], ["seed"])

    def test_pressure_rows_carry_the_block_sets_when_captured(self):
        p = self.build(detail(9, 1, 0), dec(9), colour(9),
                       blocks(9, "183,184", "184"))
        row = lc.pressure(p)[0]
        self.assertEqual(row["refs"], [183])
        self.assertEqual(row["livein"], [184])

    def test_another_procedures_block_rows_are_not_mixed_in(self):
        p = self.build(blocks(9, "1,2", "-", proc=1))
        self.assertIsNone(lc.blocks_of(p, 9))


class ComparisonTests(unittest.TestCase):
    def census(self, webs=4, split=2, order=(0, 2), lineages=None):
        lin = lineages if lineages is not None else [
            {"event": e, "table": str(100 + e), "chain": "0", "sym": "s",
             "type": "3", "dtype": "0", "members": 2, "blocks": [5],
             "webs": [e], "split": 1, "coloured": 0} for e in order]
        return {"symbol": "f", "proc": 0, "webs": webs, "decisions": webs,
                "split": split, "coloured": webs - split, "lineages": lin,
                "pressure": [], "creation_order": list(order)}

    def test_an_edit_that_changes_nothing_says_it_reached_nothing(self):
        c = self.census()
        text = lc.compare(c, c)
        self.assertIn("did not reach the split axis", text)
        self.assertIn("creation order unchanged", text)

    def test_a_changed_creation_order_is_reported(self):
        text = lc.compare(self.census(order=(0, 2)), self.census(order=(0, 2, 4)))
        self.assertIn("creation order CHANGED", text)

    def test_a_lineage_whose_split_structure_moved_is_named(self):
        before = self.census()
        after = self.census()
        after["lineages"][0] = dict(after["lineages"][0], webs=[0, 1], split=2)
        text = lc.compare(before, after)
        self.assertIn("split structure moved", text)
        self.assertNotIn("did not reach the split axis", text)

    def test_comparing_two_different_functions_is_refused(self):
        a, b = self.census(), self.census()
        b["symbol"] = "g"
        d = pathlib.Path(tempfile.mkdtemp())
        (d / "a.json").write_text(json.dumps(a))
        (d / "b.json").write_text(json.dumps(b))
        with self.assertRaises(SystemExit):
            lc.main(["--compare", str(d / "a.json"), str(d / "b.json")])


class RenderTests(unittest.TestCase):
    def data(self):
        return {"symbol": "f", "proc": 0, "webs": 395, "decisions": 395,
                "split": 254, "coloured": 141,
                "lineages": [{"event": 37, "table": "1", "chain": "0",
                              "sym": "1", "type": "3", "dtype": "0",
                              "members": 70, "blocks": [15, 19],
                              "webs": list(range(29)), "split": 29,
                              "coloured": 0}],
                "pressure": [{"web": 1098, "decision": "split", "numintf": 140,
                              "regsleft": 7, "colour": None, "reg": None,
                              "bb": -1, "type": "3"}],
                "creation_order": [37]}

    def test_the_split_count_is_stated_up_front(self):
        text = lc.render(self.data())
        self.assertIn("141 coloured, 254 SPLIT", text)

    def test_a_lineage_that_became_many_webs_is_listed(self):
        text = lc.render(self.data())
        self.assertIn("lineages that became more than one web: 1", text)
        self.assertIn("29", text)

    def test_the_contended_webs_table_shows_interference_and_room(self):
        text = lc.render(self.data())
        self.assertIn("1098", text)
        self.assertIn("140", text)


if __name__ == "__main__":
    unittest.main()
