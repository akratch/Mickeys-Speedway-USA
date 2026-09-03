#!/usr/bin/env python3
"""Route every NON_MATCHING candidate to the cheapest tool that has closed
its class, from the two measurements the campaign already keeps.

Inputs:
  build/promotion-trial.json   (tools/promotion_trial.py; overlay candidates,
                                the linked-ROM oracle: class, in-range words,
                                out-of-range bytes, module delta)
  config/nonmatching-ranking.us.json (tools/nm_ranking.py; every candidate,
                                relocation-masked differing words, size delta,
                                category)

Routes (docs/epoch15-plan.md, docs/adr/0016):
  sweep        permuter sweep: overlay text-differs with out=0 and at most
               `--sweep-words` in-range words; resident rows within the same
               masked-word bound and |size delta| <= 64
  ownership    overlay rom-size rows (module delta != 0): data-ownership carve
  lever        rows within `--lever-words` words that the sweep has already
               left flat: Opus/Fable with a named lever (a sweep-flat row
               beyond that bound is structural work, not a second sweep)
  reshape      text-differs beyond the sweep bound, and build-error rows whose
               cause is schedule-divergence-at-site (structural; Codex shape
               lanes, frame-exact candidate mandate)
  compile-fix  build-error rows whose cause is compile-error
  structural   resident rows beyond the bound (candidate is far off)

Output is a Markdown table per route (default) or JSON (`--json`), plus the
exclude lists the sweep runner takes (`--write-excludes DIR`). It never reads
ROM bytes and prints no instruction text.
"""
import argparse
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load(path):
    with open(path, encoding="utf-8") as fh:
        return json.load(fh)


def live_candidates():
    """Names still guarded by `#ifdef NON_MATCHING` in src/ (the ranking and
    the trial both outlive a promotion)."""
    try:
        sys.path.insert(0, os.path.join(ROOT, "tools"))
        import permute_batch  # noqa: WPS433
        return {item.func for item in permute_batch.discover_queue_from_source_scan()}
    except Exception:  # pragma: no cover - the scan is a convenience filter
        return None


def route(trial, ranking, sweep_words, lever_words, sweep_done, live=None):
    rows = trial.get("results", trial) if isinstance(trial, dict) else trial
    if live is not None:
        rows = [r for r in rows if r["func"] in live]
        ranking = {"functions": [r for r in ranking["functions"] if r["name"] in live]}
    ranking_rows = {r["name"]: r for r in ranking["functions"]}
    routes = {k: [] for k in ("sweep", "ownership", "lever", "reshape",
                              "compile-fix", "structural")}
    seen = set()
    for r in rows:
        name = r["func"]
        seen.add(name)
        klass = r.get("klass")
        words = r.get("in_range_words")
        out = r.get("out_of_range_bytes") or 0
        entry = {"function": name, "file": r.get("c_file"), "overlay": r.get("overlay"),
                 "in_range_words": words, "out_of_range_bytes": out,
                 "cause": r.get("cause")}
        if klass == "exact":
            entry["note"] = "promote"
            routes["lever"].insert(0, entry)
        elif klass == "rom-size":
            routes["ownership"].append(entry)
        elif klass == "build-error":
            (routes["compile-fix"] if r.get("cause") == "compile-error"
             else routes["reshape"]).append(entry)
        elif klass in ("text-differs", "text-size-differs"):
            if out != 0:
                routes["ownership"].append(entry)
            elif words is not None and words <= sweep_words:
                if name not in sweep_done:
                    routes["sweep"].append(entry)
                elif words <= lever_words:
                    routes["lever"].append(entry)
                else:
                    routes["reshape"].append(entry)
            else:
                routes["reshape"].append(entry)
    for name, r in ranking_rows.items():
        if r.get("overlay") is not None or name in seen:
            continue
        words = r.get("relocation_masked_differing_words")
        if words is None:
            words = r.get("differing_words")
        entry = {"function": name, "file": r.get("file"), "overlay": None,
                 "masked_words": words, "size_delta": r.get("size_delta"),
                 "category": r.get("category")}
        if words is not None and words <= sweep_words and abs(r.get("size_delta") or 0) <= 64:
            if name not in sweep_done:
                routes["sweep"].append(entry)
            elif words <= lever_words:
                routes["lever"].append(entry)
            else:
                routes["structural"].append(entry)
        else:
            routes["structural"].append(entry)
    for key, lst in routes.items():
        lst.sort(key=lambda e: (e.get("in_range_words") if e.get("in_range_words") is not None
                                else e.get("masked_words") if e.get("masked_words") is not None
                                else 10 ** 6))
    return routes


def sweep_done_names(summary_paths):
    done = set()
    for path in summary_paths:
        try:
            data = load(path)
        except (OSError, ValueError):
            continue
        rows = data.get("results", data) if isinstance(data, dict) else data
        for r in rows:
            if r.get("func") and not r.get("promoted") and not r.get("zero_found"):
                done.add(r["func"])
    return done


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--trial", default=os.path.join(ROOT, "build", "promotion-trial.json"))
    ap.add_argument("--ranking", default=os.path.join(ROOT, "config", "nonmatching-ranking.us.json"))
    ap.add_argument("--sweep-summary", action="append", default=[],
                    help="permuter summary.json of a finished sweep (repeatable); its "
                         "flat rows within the lever bound route to lever instead")
    ap.add_argument("--sweep-words", type=int, default=40)
    ap.add_argument("--lever-words", type=int, default=8)
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--no-live-filter", action="store_true",
                    help="keep rows whose function is no longer a guarded candidate")
    ap.add_argument("--write-excludes", metavar="DIR",
                    help="write sweep-exclude-resident.txt and sweep-exclude-overlay.txt")
    args = ap.parse_args()
    trial = load(args.trial) if os.path.exists(args.trial) else {"results": []}
    ranking = load(args.ranking)
    routes = route(trial, ranking, args.sweep_words, args.lever_words,
                   sweep_done_names(args.sweep_summary),
                   live=None if args.no_live_filter else live_candidates())
    if args.write_excludes:
        os.makedirs(args.write_excludes, exist_ok=True)
        overlay_ex = [e["function"] for k in ("ownership", "reshape", "compile-fix")
                      for e in routes[k] if e.get("overlay") is not None]
        resident_ex = [e["function"] for e in routes["structural"]]
        with open(os.path.join(args.write_excludes, "sweep-exclude-overlay.txt"), "w") as fh:
            fh.write("\n".join(overlay_ex) + "\n")
        with open(os.path.join(args.write_excludes, "sweep-exclude-resident.txt"), "w") as fh:
            fh.write("\n".join(resident_ex) + "\n")
    if args.json:
        json.dump({"schema_version": 1, "sweep_words": args.sweep_words,
                   "lever_words": args.lever_words, "routes": routes}, sys.stdout, indent=2)
        print()
        return 0
    for key, lst in routes.items():
        print(f"## {key} ({len(lst)})")
        for e in lst:
            words = e.get("in_range_words") if e.get("in_range_words") is not None else e.get("masked_words")
            extra = e.get("cause") or e.get("category") or ""
            print(f"- {e['function']} `{e.get('file')}` {words}w {extra}".rstrip())
        print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
