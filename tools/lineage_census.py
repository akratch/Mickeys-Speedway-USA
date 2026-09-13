#!/usr/bin/env python3
"""Read the allocator's SPLIT decisions and web creation order.

    tools/lineage_census.py <symbol> [--proc N] [--save out.json] [--top N]
    tools/lineage_census.py --report out.json
    tools/lineage_census.py --compare before.json after.json

WHY THIS EXISTS

`tools/web_footprint.py` made the register-COLOUR axis exhaustible, and on the
tree's largest function the campaign spent 1,875 probes, then another 1,914, on
it. That axis is 141 of the 395 decisions the allocator records for that
procedure. **The other 254 are `decision=split`, and no force addresses them:
`CDX_FORCE=p1:wN=cM` sets a colour and says nothing to a web the allocator
decided to split.**

The blocker on that function is stated entirely in split terms -- "removing a
capture range fixes the first split, but a later split then takes the restore"
-- and the split webs carry the pressure: median `numintf` 92 against 24 for
coloured webs. So the axis that matters most there has never been measured.

The instrument was already present and unused. The instrumented `uopt` accepts
`CDX_LINEAGE_TABLES` beside the five knobs the campaign uses, and emits:

    lineage_range   proc event table chain type dtype sym exprtable exprchain
    lineage_member  ... plus bb, line, flags

`event` is a monotonic sequence number, so these are the **creation order**.
L154 says a web's number follows its type first and its first use second, and
that numbering decides colouring order -- so creation order is the upstream
cause of the colour questions the landscapes measure downstream.

**The `line` field on a member is not a source line.** It reads 8 for all 1,514
members on the overlay 58 procedure -- a constant, carrying nothing. `webdetail`
is barely better: -1 for 330 of its 395 rows and 9 for the rest. **`bb` is the
positional handle here**, and it does vary (185, 114, 391, 612, ...). Anything
that attributes a lineage to a source line off these records is reading a
constant; use the basic block, or `draw_census` if you need lines.

HOW THE PIECES JOIN

A `lineage_range` is the pre-split entity; the webs that share its
`(table, chain)` are what it became. Measured on the overlay 58 procedure: 395
webs across 204 lineages, and every one of the 146 `(table, chain)` keys that
`webdetail` reports is present in the lineage table. Capture BOTH
`CDX_DETAIL_WEB=all` and `CDX_LINEAGE_TABLES=all`; with only the latter there is
no `webdetail` to join to and the census can say nothing about decisions.

WHAT TO DO WITH IT

Take a census, change one thing, and `--compare`. A source edit that leaves the
creation order and the split structure untouched did not reach this axis,
whatever it did to the score -- the same discipline `draw_census` applies to the
schedule.
"""
from __future__ import annotations

import argparse
import collections
import json
import os
import pathlib
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import force_lattice as fl  # noqa: E402

FIELD_RE = re.compile(r"(\w+)=(\S+)")
REGS = ["zero", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3",
        "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6",
        "s7", "s8", "t8", "t9"]


def parse(text: str, proc: int) -> dict:
    """Webs, their decisions, and the lineage ranges they came from."""
    detail: dict[int, dict] = {}
    decision: dict[int, dict] = {}
    colour: dict[int, dict] = {}
    ranges: list[dict] = []
    members: list[dict] = []
    for raw in text.splitlines():
        if not raw.startswith("[CDX] "):
            continue
        parts = raw.split(None, 2)
        if len(parts) < 3:
            continue
        event, fields = parts[1], dict(FIELD_RE.findall(parts[2]))
        if fields.get("proc") != str(proc):
            continue
        if event == "webdetail":
            detail[int(fields["web"])] = fields
        elif event == "p1dec":
            decision[int(fields["web"])] = fields
        elif event == "p1color":
            colour[int(fields["web"])] = fields
        elif event == "lineage_range":
            ranges.append(fields)
        elif event == "lineage_member":
            members.append(fields)
    return {"detail": detail, "decision": decision, "colour": colour,
            "ranges": ranges, "members": members}


def key_of(row: dict) -> tuple[str, str]:
    """The join between a web and the lineage it came from."""
    return (row.get("table", "?"), row.get("chain", "?"))


def lineages(parsed: dict) -> list[dict]:
    """One record per lineage, in creation order, with the webs it became."""
    webs_by_key: dict[tuple, list[int]] = collections.defaultdict(list)
    for web, row in parsed["detail"].items():
        webs_by_key[key_of(row)].append(web)
    # bb, not line: the member `line` field is a constant (8 everywhere on the
    # overlay 58 procedure) and attributing a lineage to it reads nothing.
    blocks_by_key: dict[tuple, list[int]] = collections.defaultdict(list)
    for m in parsed["members"]:
        bb = int(m.get("bb", -1))
        if bb >= 0:
            blocks_by_key[key_of(m)].append(bb)
    member_count = collections.Counter(key_of(m) for m in parsed["members"])

    out = []
    for r in sorted(parsed["ranges"], key=lambda r: int(r.get("event", 0))):
        k = key_of(r)
        webs = sorted(webs_by_key.get(k, []))
        states = [parsed["decision"].get(w, {}).get("decision") for w in webs]
        out.append({
            "event": int(r.get("event", -1)),
            "table": r.get("table"), "chain": r.get("chain"),
            "sym": r.get("sym"), "type": r.get("type"), "dtype": r.get("dtype"),
            "members": member_count.get(k, 0),
            "blocks": sorted(set(blocks_by_key.get(k, []))),
            "webs": webs,
            "split": sum(1 for s in states if s == "split"),
            "coloured": sum(1 for s in states if s == "color"),
        })
    return out


def pressure(parsed: dict) -> list[dict]:
    """Every web with its interference count and how much room it had left.

    The admission bound the overlay 58 passes kept hitting -- "admits the row
    entry at 24 interference entries and rejects restoration at 25" -- is this
    `numintf`, recorded per web and never read.
    """
    out = []
    for web, d in parsed["decision"].items():
        det = parsed["detail"].get(web, {})
        col = parsed["colour"].get(web)
        out.append({
            "web": web,
            "decision": d.get("decision"),
            "numintf": int(d.get("numintf", 0)),
            "regsleft": int(d.get("regsleft", 0)),
            "colour": int(col["color"]) if col else None,
            "reg": col.get("reg") if col else None,
            "bb": int(det.get("bb", -1)) if det else -1,
            "type": det.get("type"),
        })
    return sorted(out, key=lambda r: (-r["numintf"], r["web"]))


def taken_colours(fields: dict) -> list[str]:
    """Decode a forbidden mask into register names, for the squeezed webs."""
    mask = (int(fields.get("forbidden0", "0"), 0)
            | int(fields.get("forbidden1", "0"), 0) << 32)
    return [REGS[c] if c < len(REGS) else f"c{c}"
            for c in range(64) if mask >> c & 1]


def census(symbol: str, proc: int, keep: pathlib.Path | None = None) -> dict:
    command = fl.replace_compiler(fl.compile_command(symbol),
                                  fl.INSTRUMENTED / "cc")
    work = keep or pathlib.Path(tempfile.mkdtemp())
    work.mkdir(parents=True, exist_ok=True)
    command[command.index("-o") + 1] = str(work / "candidate.o")
    trace = work / "lineage.log"
    env = {k: v for k, v in os.environ.items()
           if not k.startswith(("CDX_", "DKWB_"))}
    env.update(CDX_LOG="1", CDX_PROC=str(proc), CDX_OUT=str(trace),
               CDX_DETAIL_WEB="all", CDX_LINEAGE_TABLES="all")
    result = subprocess.run(command, env=env, capture_output=True, text=True,
                            cwd=fl.ROOT, timeout=1800)
    if result.returncode:
        raise SystemExit(f"lineage_census: compile failed "
                         f"(exit {result.returncode})")
    text = trace.read_text() if trace.exists() else ""
    parsed = parse(text, proc)
    if not parsed["ranges"]:
        raise SystemExit(
            f"lineage_census: no proc={proc} lineage rows. The ordinal is Ucode "
            "procedure order, not source order (web_footprint --list-procs "
            "prints the index), and the capture needs CDX_LINEAGE_TABLES=all.")
    if not parsed["detail"]:
        raise SystemExit(
            "lineage_census: lineage rows but no webdetail -- the capture must "
            "set CDX_DETAIL_WEB=all too, or nothing can be joined to a decision.")
    lin = lineages(parsed)
    press = pressure(parsed)
    return {
        "symbol": symbol, "proc": proc,
        "webs": len(parsed["detail"]),
        "decisions": len(parsed["decision"]),
        "split": sum(1 for p in press if p["decision"] == "split"),
        "coloured": sum(1 for p in press if p["decision"] == "color"),
        "lineages": lin,
        "pressure": press,
        "creation_order": [l["event"] for l in lin],
    }


def render(data: dict, top: int = 20) -> str:
    lin, press = data["lineages"], data["pressure"]
    split = [p for p in press if p["decision"] == "split"]
    col = [p for p in press if p["decision"] == "color"]

    def med(v):
        return sorted(v)[len(v) // 2] if v else 0

    out = [f"{data['symbol']}  proc={data['proc']}",
           f"  {data['webs']} webs, {data['decisions']} decisions: "
           f"{data['coloured']} coloured, {data['split']} SPLIT",
           f"  {len(lin)} lineages",
           f"  numintf median: coloured {med([p['numintf'] for p in col])}, "
           f"split {med([p['numintf'] for p in split])}",
           ""]

    multi = [l for l in lin if len(l["webs"]) > 1]
    out.append(f"  lineages that became more than one web: {len(multi)}")
    out.append("  event  members  webs  split/col  basic blocks")
    for l in sorted(multi, key=lambda l: -len(l["webs"]))[:top]:
        blocks = ",".join(str(x) for x in l["blocks"][:6])
        span = f" (+{len(l['blocks']) - 6} more)" if len(l["blocks"]) > 6 else ""
        out.append(f"  {l['event']:>5} {l['members']:>8} {len(l['webs']):>5} "
                   f"{l['split']:>4}/{l['coloured']:<4} {blocks}{span}")

    out += ["", "  most contended webs (numintf, then room left):",
            "  web     intf  left  decision  colour    bb"]
    for p in press[:top]:
        c = f"c{p['colour']}/{p['reg']}" if p["colour"] is not None else "-"
        out.append(f"  {p['web']:<7} {p['numintf']:>4} {p['regsleft']:>5}  "
                   f"{p['decision']:<8}  {c:<8} {p['bb']}")

    out += ["", "  creation order, first twenty lineages "
                "(event, type, first basic block):"]
    for l in lin[:20]:
        first = l["blocks"][0] if l["blocks"] else "-"
        out.append(f"   {l['event']:>5}  type={l['type']:<3} bb={first}")
    return "\n".join(out)


def compare(before: dict, after: dict) -> str:
    """What a source edit did to creation order and split structure."""
    out = [f"{before['symbol']}: webs {before['webs']} -> {after['webs']}, "
           f"split {before['split']} -> {after['split']}, "
           f"coloured {before['coloured']} -> {after['coloured']}", ""]
    if before["creation_order"] == after["creation_order"]:
        out.append("  creation order unchanged")
    else:
        out.append(f"  creation order CHANGED "
                   f"({len(before['creation_order'])} -> "
                   f"{len(after['creation_order'])} lineages)")
    b = {(l["table"], l["chain"]): l for l in before["lineages"]}
    a = {(l["table"], l["chain"]): l for l in after["lineages"]}
    gone, new = sorted(set(b) - set(a)), sorted(set(a) - set(b))
    if gone:
        out.append(f"  lineages gone: {len(gone)}")
    if new:
        out.append(f"  lineages new:  {len(new)}")
    moved = [(k, b[k], a[k]) for k in sorted(set(b) & set(a))
             if len(b[k]["webs"]) != len(a[k]["webs"])
             or b[k]["split"] != a[k]["split"]]
    if moved:
        out.append("")
        out.append("  lineages whose split structure moved:")
        for k, x, y in moved[:24]:
            out.append(f"    table={k[0]} chain={k[1]}: webs "
                       f"{len(x['webs'])}->{len(y['webs'])}, split "
                       f"{x['split']}->{y['split']}, bb "
                       f"{x['blocks'][:3]}")
    if not (gone or new or moved) and before["creation_order"] == after["creation_order"]:
        out.append("")
        out.append("  nothing moved -- this edit did not reach the split axis")
    return "\n".join(out)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Read the allocator's split decisions and creation order.")
    parser.add_argument("symbol", nargs="?")
    parser.add_argument("--proc", type=int, default=0)
    parser.add_argument("--save", type=pathlib.Path, default=None)
    parser.add_argument("--keep", type=pathlib.Path, default=None)
    parser.add_argument("--report", type=pathlib.Path, default=None,
                        help="re-render a saved census, no compile")
    parser.add_argument("--compare", nargs=2, type=pathlib.Path, default=None,
                        metavar=("BEFORE", "AFTER"))
    parser.add_argument("--top", type=int, default=20)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args(argv)

    if args.compare:
        before, after = (json.loads(p.read_text()) for p in args.compare)
        if before["symbol"] != after["symbol"]:
            raise SystemExit("lineage_census: --compare needs two censuses of "
                             f"the same function ({before['symbol']} vs "
                             f"{after['symbol']})")
        print(compare(before, after))
        return 0
    if args.report:
        print(render(json.loads(args.report.read_text()), args.top))
        return 0
    if not args.symbol:
        parser.error("a symbol is required unless --report or --compare is used")

    data = census(args.symbol, args.proc, args.keep)
    if args.save:
        args.save.write_text(json.dumps(data, indent=2) + "\n")
    print(json.dumps(data, indent=2) if args.json else render(data, args.top))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
