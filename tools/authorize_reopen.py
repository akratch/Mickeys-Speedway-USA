#!/usr/bin/env python3
"""Write and prove reopen authorizations for exhausted plateaus.

An authorization in `config/lane-reopen-authorizations.us.json` reopens one
exhausted plateau for one pass. It arms only when its `source_commit` and
`ledger_commit` pins equal the ones `lane_status.classify` derives for that
symbol *right now*; any other pair reads as
`reopen-authorization-stale` and the target stays exhausted.

That is the trap this tool exists to close. Restating twenty reasons by hand
on 2026-09-08 left every pin at its old value, so twenty authorizations were
rewritten, committed, gated, and armed nothing at all. The failure is silent:
the file looks right, `--check-reopen-schema` passes, and the queue simply
does not grow.

So this tool never computes a pin. It asks the classifier what the pins are
and writes those back, and then it re-asks and refuses to claim success for
any symbol that did not actually become assignable.

    # 1. write the pins, from the classifier's own answer
    python3 tools/authorize_reopen.py --symbols func_A,func_B \
        --reason "ADR 0018 stall rule: closed under the retired attempt cap."

    # 2. commit config/lane-reopen-authorizations.us.json, then:
    python3 tools/authorize_reopen.py --verify --symbols func_A,func_B

Step 2 is separate because the classifier reads the authorization file out of
the *committed* base (`git show <base>:<path>`), not the worktree. Committing
the file does not touch any source path or handoff shard, so the pins written
in step 1 are still the right ones when step 2 checks them.

`--reason-from-class` derives the reason from the target's measured class in
config/nonmatching-ranking.us.json instead of taking one string for all of
them, so a size-mismatch target is not told to go and permute.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path
from tempfile import NamedTemporaryFile

REPO = Path(__file__).resolve().parent.parent
AUTHORIZATIONS = REPO / "config" / "lane-reopen-authorizations.us.json"
RANKING = REPO / "config" / "nonmatching-ranking.us.json"
LANE_STATUS = REPO / "tools" / "lane_status.py"

REASON_LIMIT = 240

# A verdict an authorization can actually change. Anything else is either
# already assignable (nothing to authorize), owned by a live lane (authorizing
# it would race that lane), or broken in a way a reopen does not address.
REOPENABLE = {"already-integrated/exhausted"}

# Mechanism per measured class, for --reason-from-class. Each says what is
# newly true and what the lane should therefore do; a reason that names no
# mechanism is a reopen resting on nothing (ADR 0011).
CLASS_REASON = {
    "register-only": (
        "Class is register-only at {words} masked words, delta 0: routes to "
        "the forced-colour oracle and the permuter, mechanisms the recorded "
        "attempts did not use."
    ),
    "schedule-only": (
        "Class is schedule-only at {words} masked words, delta 0: routes to "
        "the g0 schedule probe, a mechanism the recorded attempts did not use."
    ),
    "size-mismatch": (
        "ADR 0018 stall rule, and at delta {delta} bytes the candidate is not "
        "the same function: decompiler-assisted reconstruction applies where "
        "no permutation can."
    ),
    "other": (
        "ADR 0018 stall rule: this plateau closed under the retired ten-"
        "attempt cap. Delta 0 at {words} masked words is a schedule or "
        "structure defect the permuter can still move."
    ),
}


def classify(symbols: list[str]) -> dict[str, dict]:
    """Ask lane_status for each symbol's verdict and its current pins.

    One batch call, not one per symbol: `--symbols` shares a single evidence
    scan across the whole list, and `--json` reports the same assignment
    record that `--symbol` does, pins included. Forty-eight symbols one at a
    time took twelve minutes; the batch is a single process.

    Exit status is deliberately ignored -- the batch returns 1 when any
    symbol is not `base-only`, which for this tool is the normal case and
    not an error.
    """
    result = subprocess.run(
        [sys.executable, str(LANE_STATUS), "--symbols", ",".join(symbols), "--json"],
        cwd=REPO, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    if not result.stdout.strip():
        raise RuntimeError(
            f"lane_status produced nothing for {len(symbols)} symbol(s) "
            f"({result.stderr.strip() or 'no error text'})"
        )
    assignments = json.loads(result.stdout)["assignments"]
    found = {row["symbol"]: row for row in assignments}
    missing = [s for s in symbols if s not in found]
    if missing:
        raise RuntimeError(f"lane_status returned no verdict for: {missing}")
    return found


def ranking() -> dict[str, dict]:
    return {row["name"]: row for row in json.loads(RANKING.read_text())["functions"]}


def reason_for(symbol: str, rows: dict[str, dict]) -> str:
    row = rows.get(symbol)
    if row is None:
        raise RuntimeError(
            f"{symbol}: no ranking row, so no measured class to derive a "
            f"reason from; pass an explicit --reason"
        )
    template = CLASS_REASON.get(row["category"], CLASS_REASON["other"])
    return template.format(
        words=row.get("relocation_masked_differing_words"),
        delta=row.get("size_delta"),
    )


def validate(reason: str, symbol: str) -> None:
    """The schema lane_status enforces, checked before writing, not after."""
    if not reason.strip():
        raise RuntimeError(f"{symbol}: reason is empty")
    if len(reason) > REASON_LIMIT:
        raise RuntimeError(
            f"{symbol}: reason is {len(reason)} chars, limit is {REASON_LIMIT}"
        )
    if "\n" in reason or "|" in reason:
        raise RuntimeError(f"{symbol}: reason may not contain a newline or '|'")


def entry_is_valid(symbol: str, source_commit: str, ledger_commit: str | None) -> str | None:
    """Return why lane_status would reject this pin pair, or None if it holds.

    The authorization file is validated as a whole: `reopen_authorizations`
    raises on the FIRST bad entry and every other authorization in the file
    goes inert with it. One entry whose ledger commit carries no handoff for
    its source path silently disarmed 48 armed authorizations on 2026-09-09.

    These are the same checks `reopen_authorizations` makes, called through
    lane_status's own functions rather than reimplemented, so the two cannot
    drift apart.
    """
    sys.path.insert(0, str(REPO / "tools"))
    import lane_status

    source_path, identity_error = lane_status.source_identity(source_commit, symbol)
    if identity_error or source_path is None:
        return identity_error or "source commit identifies no single definition"
    if ledger_commit is None:
        return None
    shard_text = lane_status.show_file(ledger_commit, lane_status.shard_path(symbol))
    try:
        shard_source = lane_status.validated_shard_source(shard_text, symbol)
    except RuntimeError as error:
        return f"ledger commit has invalid handoff evidence: {error}"
    if shard_source != source_path:
        return "ledger commit does not identify the authorized source path"
    return None


def write(symbols: list[str], reason: str | None, dry_run: bool) -> int:
    document = json.loads(AUTHORIZATIONS.read_text())
    authorizations = document["authorizations"]
    verdicts = classify(symbols)
    rows = ranking() if reason is None else {}

    planned: list[tuple[str, str, str | None, str]] = []
    refused: list[tuple[str, str]] = []
    for symbol in symbols:
        assignment = verdicts[symbol]
        state = assignment["state"]
        if state not in REOPENABLE:
            refused.append((symbol, f"verdict is {state}, not exhausted"))
            continue
        source_commit = assignment.get("source_commit")
        if source_commit is None:
            refused.append((symbol, "classifier reports no source commit to pin"))
            continue
        ledger_commit = assignment.get("ledger_commit")
        invalid = entry_is_valid(symbol, source_commit, ledger_commit)
        if invalid is not None:
            # Write it with no ledger pin rather than poisoning the file; if
            # that will not validate either, refuse the symbol outright.
            if entry_is_valid(symbol, source_commit, None) is None:
                ledger_commit = None
            else:
                refused.append((symbol, invalid))
                continue
        text = reason if reason is not None else reason_for(symbol, rows)
        validate(text, symbol)
        planned.append((symbol, source_commit, ledger_commit, text))

    for symbol, why in refused:
        print(f"refused  {symbol}: {why}", file=sys.stderr)

    for symbol, source_commit, ledger_commit, text in planned:
        print(f"authorize {symbol}\n    source {source_commit}\n"
              f"    ledger {ledger_commit}\n    {text}")
        if dry_run:
            continue
        authorizations[symbol] = {
            "ledger_commit": ledger_commit,
            "reason": text,
            "source_commit": source_commit,
        }

    if dry_run:
        print(f"\ndry run: {len(planned)} would be armed, {len(refused)} refused")
        return 0 if planned else 1

    if planned:
        AUTHORIZATIONS.write_text(
            json.dumps(document, indent=2, sort_keys=True) + "\n"
        )
    print(
        f"\nwrote {len(planned)} authorization(s), refused {len(refused)}.\n"
        f"Commit {AUTHORIZATIONS.relative_to(REPO)}, then re-run with "
        f"--verify to prove they armed."
    )
    return 0 if planned else 1


def verify(symbols: list[str]) -> int:
    """Prove each symbol is now assignable. Silence here is the bug."""
    verdicts = classify(symbols)
    armed, inert = [], []
    for symbol in symbols:
        assignment = verdicts[symbol]
        if assignment["state"] == "base-only":
            armed.append(symbol)
        else:
            inert.append((symbol, assignment["state"], assignment["reason"]))

    for symbol in armed:
        print(f"armed    {symbol}")
    for symbol, state, why in inert:
        print(f"INERT    {symbol}: {state} -- {why}", file=sys.stderr)

    if inert:
        print(
            f"\n{len(inert)} of {len(symbols)} authorization(s) armed nothing. "
            f"An authorization arms only when its pins equal the ones the "
            f"classifier derives now; if the tree moved after they were "
            f"written, re-run without --verify to repin.",
            file=sys.stderr,
        )
        return 1
    print(f"\nall {len(armed)} authorization(s) armed")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--symbols", required=True, help="comma-separated")
    parser.add_argument("--reason", help="one reason for every symbol")
    parser.add_argument(
        "--reason-from-class", action="store_true",
        help="derive each reason from the target's measured class",
    )
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument(
        "--verify", action="store_true",
        help="re-classify and prove the committed authorizations armed",
    )
    args = parser.parse_args()

    symbols = [s for s in (t.strip() for t in args.symbols.split(",")) if s]
    if not symbols:
        parser.error("--symbols is empty")
    if args.verify:
        return verify(symbols)
    if bool(args.reason) == bool(args.reason_from_class):
        parser.error("pass exactly one of --reason or --reason-from-class")
    return write(symbols, args.reason, args.dry_run)


if __name__ == "__main__":
    sys.exit(main())
