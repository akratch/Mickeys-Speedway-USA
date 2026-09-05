#!/usr/bin/env python3
"""Inventory raw assembly ownership without counting scaffolds as matches.

Only explicitly named, zero-filled overlay padding/tails are excluded from
the overlay carve gate. Small executable tails are work, not padding. Resident
handwritten ranges use the scoreboard's existing evidence ledger; all other
resident raw ranges remain unclassified rather than guessed to be compiler C.
No assembly, decoded instructions, or ROM bytes leave this tool.
"""
from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

import yaml

ROOT = Path(__file__).resolve().parent.parent


def segment_fields(row):
    if isinstance(row, list) and row and isinstance(row[0], int):
        return row[0], row[1] if len(row) > 1 else None, row[2] if len(row) > 2 else None
    if isinstance(row, dict) and isinstance(row.get("start"), int):
        return row["start"], row.get("type"), row.get("name")
    raise ValueError("raw census requires explicit numeric ROM subsegment boundaries")


def census(config, rom, handwritten):
    rows = []
    segments = config["segments"]
    for index, segment in enumerate(segments):
        if not isinstance(segment, dict) or segment.get("type") != "code":
            continue
        overlay = re.fullmatch(r"overlay_(\d{3})", segment.get("name", ""))
        subs = segment.get("subsegments", [])
        for i, sub in enumerate(subs):
            kind = sub.get("type") if isinstance(sub, dict) else sub[1] if len(sub) > 1 else None
            if kind != "asm":
                continue
            start, kind, name = segment_fields(sub)
            if name is None:
                name = f"unnamed-rom-{start:X}"
            if not isinstance(name, str) or not name:
                raise ValueError("invalid raw assembly name")
            if i + 1 < len(subs):
                end, next_kind, _ = segment_fields(subs[i + 1])
            elif index + 1 < len(segments):
                end, next_kind, _ = segment_fields(segments[index + 1])
            else:
                raise ValueError(f"missing end boundary for {name}")
            if not 0 <= start < end <= len(rom) or (end - start) % 4:
                raise ValueError(f"invalid raw assembly extent for {name}")
            classification = "resident-unclassified"
            if overlay:
                classification = "overlay-raw"
                declared_padding = re.fullmatch(
                    rf"overlay_{overlay[1]}_(?:padding(?:_[a-z]+)?|tail)", name
                )
                # A bare endpoint marks text ending before C-owned data. An
                # empty-data module instead ends at the next segment start.
                text_end = i + 1 == len(subs) or next_kind in (None, "bin", "data", "rodata")
                if declared_padding and text_end and not any(rom[start:end]):
                    classification = "verified-zero-padding"
            elif name in handwritten:
                classification = "verified-handwritten"
            rows.append({"source": name, "overlay": int(overlay[1]) if overlay else None,
                         "section": ".text", "rom_start": start, "rom_end": end,
                         "size_bytes": end - start, "classification": classification})
    counts = Counter(row["classification"] for row in rows)
    sizes = Counter()
    for row in rows:
        sizes[row["classification"]] += row["size_bytes"]
    return {"schema": "mickey-raw-asm-census-v1", "ranges": rows,
            "counts": dict(counts), "bytes": dict(sizes),
            "overlay_carve_complete": counts["overlay-raw"] == 0,
            "matching_credit": 0}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--check-overlays", action="store_true",
                        help="fail if any overlay executable/unknown raw range remains")
    args = parser.parse_args(argv)
    try:
        config = yaml.safe_load((ROOT / "mickey.us.yaml").read_text())
        rom = (ROOT / "baseroms/mickey.us.z64").read_bytes()
        if hashlib.sha1(rom).hexdigest() != config["sha1"]:
            raise ValueError("baserom does not match configured SHA1")
        handwritten = {line.split("#", 1)[0].strip() for line in
                       (ROOT / "verified_asm.us.txt").read_text().splitlines()}
        report = census(config, rom, handwritten)
    except (OSError, ValueError, KeyError, TypeError, yaml.YAMLError) as error:
        parser.exit(2, f"raw-asm-census: {error}\n")
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        for kind in sorted(report["counts"]):
            print(f"{kind}: {report['counts'][kind]} ranges, {report['bytes'][kind]} bytes")
        print("overlay carve gate: " + ("PASS" if report["overlay_carve_complete"] else "FAIL"))
        print("Ownership inventory only; no matching credit. Resident unclassified ranges require review.")
    return int(args.check_overlays and not report["overlay_carve_complete"])


if __name__ == "__main__":
    raise SystemExit(main())
