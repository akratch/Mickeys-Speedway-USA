#!/usr/bin/env python3
"""Score one or more queued functions the way the ranking does.

    tools/score_symbol.py <symbol> [<symbol> ...] [--json]

Exists because lanes hand-roll raw-word comparisons in their fast direct-`cc`
loops and then rediscover, every few days, that the numbers are inflated.

The inflation is real and its cause is well understood: splat emits an address
as a `%hi`/`%lo` pair of a named symbol only when it has a symbol and the
halves sit together, and otherwise writes the literal form, which carries no
relocation. A raw comparison then sees a relocated word against a literal one
and reports a difference that does not survive linking. The project's own
comparator already handles this -- it masks the linker-controlled bits at the
positional UNION of both objects' relocation surfaces, so an unrelocated
literal `lui` and a relocated `%hi` `lui` compare equal while a genuine
register difference still shows -- but that comparator lives inside a
whole-queue tool, so a lane iterating spellings never reaches it.

This is that comparator with a symbol-sized entry point. It calls the same
`process_item` the ranking calls, so its numbers agree with
`docs/nm-ranking.md` by construction rather than by review.

Two traps it exists to keep lanes out of, both of which have cost real work:
`objdump -r` prints relocation offsets in SECTION coordinates while a
hand-rolled scorer indexes instructions from the function's base, and not
rebasing them silently unmasks relocated words; and the same `>> 16` syntax
carrying a full 32-bit constant is a float immediate, which must not be masked
at all.
"""
from __future__ import annotations

import argparse
import json
import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import nm_ranking as nr  # noqa: E402
import permute_batch as pb  # noqa: E402


def score(symbols: list[str]) -> tuple[list[dict], list[str]]:
    """Measure each named symbol, compiling each owning TU exactly once."""
    queue = {item.func: item for item in pb.discover_queue()}
    wanted, errors = [], []
    for symbol in symbols:
        item = queue.get(symbol)
        if item is None:
            errors.append(f"{symbol}: not in the NON_MATCHING queue "
                          f"(already matched, or never queued)")
        else:
            wanted.append(item)
    if not wanted:
        return [], errors

    commands = nr.configured_compile_commands(wanted)
    compiled = {source: nr.compile_configured_tu(source, commands[source])
                for source in commands}

    rows = []
    for item in wanted:
        candidate, error = compiled[item.rel_c_file]
        if candidate is None:
            errors.append(f"{item.func}: {error}")
            continue
        result, error = nr.process_item(item, candidate)
        if result is None:
            errors.append(f"{item.func}: {error}")
            continue
        raw = result.differing_words
        masked = result.relocation_masked_differing_words
        rows.append({
            "symbol": result.name,
            "file": result.file,
            "size_bytes": result.size_bytes,
            "size_delta": result.size_delta,
            "differing_words": raw,
            "relocation_masked_differing_words": masked,
            "relocation_artifact_words": (raw - masked)
            if (raw is not None and masked is not None) else None,
            "first_mismatch_offset": result.first_mismatch_offset,
            "relocation_masked_first_mismatch_offset":
                result.relocation_masked_first_mismatch_offset,
            "category": result.category,
        })
    return rows, errors


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("symbols", nargs="+")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args(argv)

    rows, errors = score(args.symbols)
    if args.json:
        print(json.dumps({"functions": rows, "errors": errors}, indent=2, sort_keys=True))
    else:
        if rows:
            print("%-44s %7s %7s %7s %8s %7s  %s" % (
                "symbol", "bytes", "raw", "masked", "artifact", "delta", "category"))
            for row in rows:
                print("%-44s %7d %7s %7s %8s %7s  %s" % (
                    row["symbol"][:44], row["size_bytes"],
                    row["differing_words"], row["relocation_masked_differing_words"],
                    row["relocation_artifact_words"], row["size_delta"],
                    row["category"]))
            print("\n'masked' is the number to work against: 'raw' counts words whose "
                  "only difference is\nhow an address is spelled, which does not survive "
                  "linking.")
        for error in errors:
            print(error, file=sys.stderr)
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
