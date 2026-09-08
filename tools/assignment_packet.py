#!/usr/bin/env python3
"""Compose one assignment packet for a NON_MATCHING target.

The coordinator hand-wrote these packets until now, which cost tokens, drifted
between waves, and mis-stated evidence at least once (a size delta read as
words instead of bytes). This tool derives every factual field from the tree:
the assignment-gate verdict, the retained configured measurement, the committed
plateau handoff, the reopen authorization, and the workbench verdict routing.

It prints counts, offsets and paths only. It never reads ROM text, never
inlines field-guide lever prose (which quotes mnemonics), and never edits
source. The packet tells the worker which workbench playbook its measured
mismatch class routes to and the exact command that prints the current levers,
so the field guide stays the single source of truth.
"""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RANKING = ROOT / "config" / "nonmatching-ranking.us.json"
REOPEN = ROOT / "config" / "lane-reopen-authorizations.us.json"
HANDOFFS = ROOT / "docs" / "matching-triage-handoffs"

# Ranking category -> (workbench verdict, playbook). "other" is deliberately
# unmapped: the ranking cannot distinguish allocation from constant from frame
# residuals, so the worker must take the verdict from `decomp-workbench
# diagnose` rather than from a coordinator's guess.
CATEGORY_ROUTE = {
    "register-only": ("register-permutation", "forced-color-oracle"),
    "schedule-only": ("schedule", "g0-schedule-probe"),
    "size-mismatch": ("structure", "structure-buckets"),
    "reloc-mismatch": ("relocation-layout-mismatch", "relocation-only"),
}
CANDIDATE_PLAYBOOKS = (
    "pool-position (allocation)", "constant-audit (constant/operand)",
    "stack-frame-recovery (frame-layout)", "temp-fifo-phase (phase-shift)",
    "ast-shape (commutative-order)", "structure-buckets (structure)",
)


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True,
                          text=True).stdout.strip()


def ranking_row(symbol: str) -> dict | None:
    data = json.loads(RANKING.read_text(encoding="utf-8"))
    for row in data["functions"]:
        if row["name"] == symbol:
            return row
    return None


def gate_verdict(symbol: str) -> tuple[bool, str]:
    proc = subprocess.run(
        [str(ROOT / ".venv/bin/python"), str(ROOT / "tools/lane_status.py"),
         "--symbol", symbol],
        cwd=ROOT, capture_output=True, text=True)
    return proc.returncode == 0, (proc.stdout + proc.stderr).strip()


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("symbol")
    ap.add_argument("--minutes", type=int, default=90)
    ap.add_argument("--allow-unassignable", action="store_true",
                    help="compose the packet even if the gate is not base-only")
    args = ap.parse_args()

    row = ranking_row(args.symbol)
    if row is None:
        print(f"error: {args.symbol} is not in the retained ranking", file=sys.stderr)
        return 2
    ok, verdict = gate_verdict(args.symbol)
    if not ok and not args.allow_unassignable:
        print(f"error: assignment gate refused {args.symbol}\n{verdict}", file=sys.stderr)
        return 3

    base = git("rev-parse", "HEAD")
    words = row["size_bytes"] // 4
    masked = row["relocation_masked_differing_words"]
    raw = row["differing_words"]
    cat = row["category"]

    out: list[str] = []
    add = out.append
    add(f"# Assignment: {args.symbol}")
    add("")
    add(f"You own `{row['file']}` exclusively for this lane. Base commit:")
    add(f"`{base}`. The ADR 0011 assignment gate returned `base-only` for this")
    add("exact symbol/path identity at that commit.")
    add("")
    add("## Measured baseline (retained configured full-TU measurement)")
    add("")
    add(f"- {row['size_bytes']} bytes, {words} words, `size_delta = {row['size_delta']}`"
        + (" **bytes**, not words" if row["size_delta"] else ""))
    add(f"- differing words: {raw} raw, {masked} relocation-masked")
    add(f"- first mismatch: raw `+0x{row['first_mismatch_offset']:X}`, "
        f"masked `+0x{row['relocation_masked_first_mismatch_offset']:X}`")
    add(f"- ranking category: `{cat}`")
    add("")
    add("These are scheduling evidence, not proof. Re-derive your own configured")
    add("baseline in this lane before any source edit.")
    add("")
    add("## Workbench routing — do this before writing any candidate C")
    add("")
    add("Classify with the workbench rather than by eye:")
    add("")
    add("```sh")
    add(f"tools/wb_compare.sh {args.symbol}")
    add(f".venv/bin/decomp-workbench diagnose build/wb/{args.symbol}.target.o \\")
    add(f"    build/src/<tu>.c.o --function {args.symbol} \\")
    add("    --objdump tools/binutils/mips64-elf-objdump")
    add("```")
    add("")
    add("Its `next:` footer names the field-guide lever to apply. Then:")
    add("")
    route = CATEGORY_ROUTE.get(cat)
    if route:
        vd, pb = route
        add(f"```sh")
        add(f".venv/bin/decomp-workbench guide {pb}")
        add("```")
        add("")
        add(f"The retained `{cat}` measurement routes to the **`{pb}`** playbook")
        add(f"(workbench verdict `{vd}`). Read those levers before your first")
        add("attempt; they are measured IDO mechanism, not general advice.")
    else:
        add("The ranking category `other` does not identify the mechanism on its")
        add("own, so take the verdict from `diagnose` and run")
        add("`.venv/bin/decomp-workbench guide <verdict>` for the routed playbook.")
        add("Likely candidates for this class:")
        add("")
        for name in CANDIDATE_PLAYBOOKS:
            add(f"- {name}")
    add("")
    add("`decomp-workbench guide` alone lists every playbook, verdict and lever.")
    add("`decomp-workbench guide laws ido53` / `ido71` gives compiler mechanism.")
    add("A plateau note quotes the verdict and the lever, never the aligned rows.")
    add("")

    handoff = HANDOFFS / f"{args.symbol}.md"
    if handoff.is_file():
        add("## Committed prior plateau — do not repeat it")
        add("")
        add(f"`docs/matching-triage-handoffs/{args.symbol}.md` is committed evidence.")
        add("Read it in full. Anything it records as flat, regressing or exhausted")
        add("is off-limits without changed evidence about its inputs (ADR 0017).")
        add("")
    else:
        add("## Virgin target")
        add("")
        add("No committed plateau handoff exists. You have the full ADR 0009")
        add("attempt budget, and no prior mechanism is excluded.")
        add("")

    if REOPEN.is_file():
        auth = json.loads(REOPEN.read_text(encoding="utf-8"))["authorizations"].get(args.symbol)
        if auth:
            add("## Authorized mechanism — this is your entire permission")
            add("")
            add("Reopen authorization, verbatim:")
            add("")
            add(f"> {auth['reason']}")
            add("")
            add("Stay inside it. If it does not close the function, record the")
            add("plateau; do not substitute a different mechanism.")
            add("")

    add("## Budget and definition of done")
    add("")
    add(f"Soft deadline {args.minutes} minutes; reserve the tail for proof or handoff.")
    add("")
    add("There is no attempt-count cap (ADR 0018). Stop on absence of new")
    add("information: keep going while an attempt still buys a better residual, a")
    add("newly proved identity, or an eliminated hypothesis, and stop only after")
    add("five consecutive attempts that buy none of those. Record that stall as")
    add("the stopping evidence. Lean toward continuing: while the series is still")
    add("moving at all, the wall clock is your limit, not the stall count, and a")
    add("high attempt number is not by itself a reason to stop or to hand off.")
    add("Conversely, stop early — with zero")
    add("attempts if warranted — when this target's committed evidence already")
    add("rules out the mechanism available to you; verifying exhaustion is a")
    add("result, re-deriving a known-flat one is waste. Baseline measurements,")
    add("tool failures and reruns of an unchanged configuration are not attempts.")
    add("")
    add("Done means either an exact match committed with its symbol-table line")
    add("and atlas row, proved by extracting the owned range from the linked")
    add("output and byte-comparing it against the same offsets in the baserom")
    add("plus a passing `gmake verify`; or a plateau recorded with")
    add("`tools/finalize_plateau.py` carrying measured score, frame, relocation")
    add("count, first mismatch and a concrete next lever.")
    print("\n".join(out))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
