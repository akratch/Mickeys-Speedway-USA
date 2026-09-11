#!/usr/bin/env python3
"""Census both sides' stack slots, and diff them.

    tools/frame_census.py <symbol> [<symbol> ...] [--json]

Three of six closures re-tested on 2026-09-11 turned out to be **home-set**
problems that nobody had measured, and the lane that found them built this
census in scratch and threw it away -- the second instrument to be rebuilt that
way. So it lives here now.

What it answers: *which stack slots does each side use, how wide, how often,
and which does only one side have?* That question closed `overlay2ChooseBoundary`
(the target's ladder read straight off the object, then declared to) and
`func_80041530` (a slot that was a compiler temp on the target and a declared
home on the candidate, so ~190 declaration cells had been sweeping a space
disjoint from the answer -- L118).

**Why it reads the objects rather than debug info.** IDO's `.mdebug` names
declared locals, but only for something it compiled; the target is extracted
assembly and has no such section. Slot *traffic* is visible on both sides
identically, so the census is symmetric and the diff means something. What it
cannot tell you directly is whether a slot is a declared home or a compiler
temp -- for that, compare the shape: a slot the target touches that yours does
not, or a width that differs, is the tell, and `cc -g3` on your own side names
which of yours are declared.

Both streams come from `nm_ranking.word_streams`, the same path the ranking
scores by, so the symbol span and relocation handling match the rest of the
toolchain rather than being re-derived here.
"""
from __future__ import annotations

import argparse
import collections
import contextlib
import json
import pathlib
import shutil
import sys
import tempfile

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import nm_ranking as nr  # noqa: E402
import permute_batch as pb  # noqa: E402

SP = 29

# op -> (mnemonic class, access width in bytes)
MEMORY_OPS = {
    0x20: ("load", 1), 0x24: ("load", 1), 0x28: ("store", 1),
    0x21: ("load", 2), 0x25: ("load", 2), 0x29: ("store", 2),
    0x23: ("load", 4), 0x2B: ("store", 4),
    0x22: ("load", 4), 0x26: ("load", 4),   # lwl / lwr
    0x2A: ("store", 4), 0x2E: ("store", 4),  # swl / swr
    0x31: ("load", 4), 0x39: ("store", 4),   # lwc1 / swc1
    0x35: ("load", 8), 0x3D: ("store", 8),   # ldc1 / sdc1
}
ADDIU = 0x09


def signed16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def census(words: list[int]) -> dict:
    """Every $sp-relative access in one instruction stream, by slot."""
    slots: dict[int, dict] = {}
    frame = None
    for index, word in enumerate(words):
        op = (word >> 26) & 0x3F
        base = (word >> 21) & 0x1F
        offset = signed16(word & 0xFFFF)
        if op == ADDIU and base == SP:
            rt = (word >> 16) & 0x1F
            if rt == SP:
                # the prologue's own frame adjustment, not a slot
                if frame is None and offset < 0:
                    frame = -offset
                continue
            slot = slots.setdefault(offset, _blank(offset))
            slot["address_taken"] += 1
            continue
        if op in MEMORY_OPS and base == SP:
            kind, width = MEMORY_OPS[op]
            slot = slots.setdefault(offset, _blank(offset))
            slot[kind] += 1
            slot["widths"].add(width)
    for slot in slots.values():
        slot["widths"] = sorted(slot["widths"])
    return {"frame_size": frame, "slots": slots}


def _blank(offset: int) -> dict:
    return {"offset": offset, "load": 0, "store": 0,
            "address_taken": 0, "widths": set()}


def compare(streams: "nr.WordStreams") -> dict:
    ours = census(streams.base_words)
    theirs = census(streams.target_words)
    only_ours = sorted(set(ours["slots"]) - set(theirs["slots"]))
    only_theirs = sorted(set(theirs["slots"]) - set(ours["slots"]))
    shared_differing = []
    for offset in sorted(set(ours["slots"]) & set(theirs["slots"])):
        a, b = ours["slots"][offset], theirs["slots"][offset]
        if (a["widths"] != b["widths"] or a["load"] != b["load"]
                or a["store"] != b["store"]
                or a["address_taken"] != b["address_taken"]):
            shared_differing.append({"offset": offset, "ours": a, "theirs": b})
    return {
        "candidate_frame": ours["frame_size"],
        "target_frame": theirs["frame_size"],
        "candidate_slots": len(ours["slots"]),
        "target_slots": len(theirs["slots"]),
        "only_candidate": [ours["slots"][o] for o in only_ours],
        "only_target": [theirs["slots"][o] for o in only_theirs],
        "shared_differing": shared_differing,
        "target_ladder": sorted(theirs["slots"], reverse=True),
        "candidate_ladder": sorted(ours["slots"], reverse=True),
    }


@contextlib.contextmanager
def _isolated_workdir():
    previous = nr.WORK_DIR
    scratch = pathlib.Path(tempfile.mkdtemp(prefix="frame-census-"))
    nr.WORK_DIR = scratch
    try:
        yield
    finally:
        nr.WORK_DIR = previous
        shutil.rmtree(scratch, ignore_errors=True)


def measure(symbols: list[str]) -> tuple[list[dict], list[str]]:
    queue = {item.func: item for item in pb.discover_queue()}
    wanted, errors = [], []
    for symbol in symbols:
        item = queue.get(symbol)
        if item is None:
            errors.append(f"{symbol}: not in the NON_MATCHING queue")
        else:
            wanted.append(item)
    if not wanted:
        return [], errors
    rows = []
    with _isolated_workdir():
        commands = nr.configured_compile_commands(wanted)
        compiled = {source: nr.compile_configured_tu(source, commands[source])
                    for source in commands}
        for item in wanted:
            candidate, error = compiled[item.rel_c_file]
            if candidate is None:
                errors.append(f"{item.func}: {error}")
                continue
            streams, error = nr.word_streams(item, candidate)
            if streams is None:
                errors.append(f"{item.func}: {error}")
                continue
            row = {"symbol": item.func, "file": item.rel_c_file}
            row.update(compare(streams))
            rows.append(row)
    return rows, errors


def _slot(entry: dict) -> str:
    widths = "/".join(str(w) for w in entry["widths"]) or "-"
    parts = [f"+0x{entry['offset']:X}", f"w{widths}",
             f"{entry['load']}ld", f"{entry['store']}st"]
    if entry["address_taken"]:
        parts.append(f"{entry['address_taken']}&")
    return " ".join(parts)


def render(row: dict) -> str:
    out = [f"{row['symbol']}  ({row['file']})"]
    cf, tf = row["candidate_frame"], row["target_frame"]
    note = "" if cf == tf else "   <-- frames differ"
    out.append(f"  frame: candidate {cf if cf is None else hex(cf)}, "
               f"target {tf if tf is None else hex(tf)}{note}")
    out.append(f"  slots: candidate {row['candidate_slots']}, "
               f"target {row['target_slots']}")
    out.append("  target ladder:    "
               + " ".join(f"+0x{o:X}" for o in row["target_ladder"]))
    out.append("  candidate ladder: "
               + " ".join(f"+0x{o:X}" for o in row["candidate_ladder"]))
    if row["only_target"]:
        out.append("  the target uses slots you do not:")
        for entry in row["only_target"]:
            out.append(f"    {_slot(entry)}")
    if row["only_candidate"]:
        out.append("  you use slots the target does not:")
        for entry in row["only_candidate"]:
            out.append(f"    {_slot(entry)}")
    if row["shared_differing"]:
        out.append("  shared slots with different traffic:")
        for pair in row["shared_differing"]:
            out.append(f"    ours   {_slot(pair['ours'])}")
            out.append(f"    theirs {_slot(pair['theirs'])}")
    return "\n".join(out)


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Census and diff both sides' stack slots.")
    parser.add_argument("symbols", nargs="+")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args(argv)
    rows, errors = measure(args.symbols)
    if args.json:
        print(json.dumps({"functions": rows, "errors": errors}, indent=2,
                         default=list))
    else:
        for row in rows:
            print(render(row))
            print()
        for error in errors:
            print(f"error: {error}", file=sys.stderr)
    return 1 if errors and not rows else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
