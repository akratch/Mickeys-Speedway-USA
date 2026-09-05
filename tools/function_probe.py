#!/usr/bin/env python3
"""Bounded, compact preflight/diagnosis/optional flag-lattice report.

Only scalar evidence is printed. Full tool reports and compiler logs remain
in a unique ignored build directory. Never edits source or runs a promotion.
"""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parent.parent
PYTHON = str(ROOT / ".venv/bin/python")
COMPARISON_FIELDS = (
    "exact", "words", "raw_word_mismatches", "target_instructions",
    "candidate_instructions", "instruction_delta", "true_instruction_delta",
    "normalized_distance", "opcode_distance", "opcode_mismatches",
    "geometry_front", "geometry_edit_distance",
    "target_frame_size", "candidate_frame_size", "first_divergent_row",
    "relocation_metadata_mismatches", "relocation_target_mismatches",
)


def run_phase(command, directory, label, deadline):
    output = directory / f"{label}.json"
    errors = directory / f"{label}.log"
    receipt = {"status": "failed", "output": str(output.relative_to(ROOT)),
               "log": str(errors.relative_to(ROOT))}
    remaining = deadline - time.monotonic()
    if remaining <= 0:
        return {**receipt, "status": "budget-exhausted"}, None
    started = time.monotonic()
    with output.open("wb") as stdout, errors.open("wb") as stderr:
        try:
            process = subprocess.Popen(command, cwd=ROOT, stdout=stdout, stderr=stderr,
                                       start_new_session=True)
        except OSError:
            return {**receipt, "status": "launch-failed"}, None
        try:
            code = process.wait(timeout=remaining)
        except (subprocess.TimeoutExpired, KeyboardInterrupt) as error:
            # These read/compile-only phases inherit our dedicated process
            # group. Do not leave their compilers running after a timeout.
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            process.wait()
            if isinstance(error, KeyboardInterrupt):
                raise
            return {**receipt, "status": "timeout"}, None
    receipt.update(returncode=code, seconds=round(time.monotonic() - started, 3))
    if code != 0:
        return receipt, None
    try:
        if output.stat().st_size > 16 * 1024 * 1024:
            raise ValueError("report too large")
        payload = json.loads(output.read_text())
        if not isinstance(payload, dict):
            raise ValueError("report is not an object")
    except (ValueError, OSError):
        return {**receipt, "status": "invalid-report"}, None
    return {**receipt, "status": "ok"}, payload


def compact(preflight, diagnosis):
    if preflight is not None and preflight.get("schema") != "mickey-function-evidence-preflight-v1":
        raise ValueError("unsupported preflight schema")
    if diagnosis is not None and diagnosis.get("schema") != "decomp-workbench-diagnosis-v3":
        raise ValueError("unsupported diagnosis schema")
    comparison = (diagnosis or {}).get("comparison", {})
    evidence = (preflight or {}).get("preflight", {})
    counts = evidence.get("counts", {})
    lever = (diagnosis or {}).get("lever") or {}
    # Allowlist scalars, never propagate aligned rows, disassembly or traces.
    scores = {key: comparison[key] for key in COMPARISON_FIELDS if key in comparison
              and (comparison[key] is None or isinstance(comparison[key], (bool, int)))}
    geometry = comparison.get("geometry") or {}
    complete = evidence.get("status") == "complete"
    target = scores.get("target_instructions")
    candidate = scores.get("candidate_instructions")
    eligible = (complete and (preflight or {}).get("resolution_mode") == "fallback"
                and scores.get("exact") is True and type(target) is int
                and target > 0 and candidate == target and scores.get("words") == 0)
    return {
        "preflight_status": evidence.get("status", "unavailable"),
        "owned_bytes": (preflight or {}).get("owned_size"),
        "comparison": scores,
        "geometry": {key: geometry[key] for key in
                     ("absolute_extent_delta", "edit_distance", "opcode_distance")
                     if type(geometry.get(key)) is int},
        "relocations": {key: value for key, value in counts.items() if type(value) is int},
        "mechanism": {"routing": (diagnosis or {}).get("routing"),
                      "lever_class": lever.get("lever_class"),
                      "edit_family": lever.get("edit_family")},
        "candidate_for_linked_trial": eligible,
        "promotion_proof_included": False,
        "next_action": ("run_separate_linked_promotion_proof" if eligible else
                        evidence.get("action", "inspect_phase_logs_and_restore_preflight_evidence")),
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("symbol")
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--no-build", action="store_true")
    parser.add_argument("--flags", action="store_true", help="also run the diagnostic flag lattice")
    parser.add_argument("--seconds", type=float, default=300, help="total subprocess wall budget")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 1)
    args = parser.parse_args(argv)
    if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", args.symbol) or not 0 < args.seconds <= 3600 or args.jobs < 1:
        parser.error("provide a C symbol, positive jobs, and a wall budget in (0, 3600]")
    if args.flags and args.no_build:
        parser.error("--flags compiles; it cannot be combined with --no-build")
    started = time.monotonic()
    parent = ROOT / "build/function-probe"
    parent.mkdir(parents=True, exist_ok=True)
    directory = Path(tempfile.mkdtemp(prefix=args.symbol + "-", dir=parent))
    deadline = started + args.seconds
    pre_command = [PYTHON, "tools/function_preflight.py", args.symbol, "--analysis-only", "--json"]
    if args.no_build:
        pre_command.append("--no-build")
    phases = {}
    phases["preflight"], preflight = run_phase(pre_command, directory, "preflight", deadline)
    # The preflight can fail on oversized TU ownership but still leave a
    # fresh, useful diagnostic object. --no-build must authenticate it first.
    command = ["tools/wb_compare.sh", "--diagnose", "--no-build", args.symbol, "--json", "--color", "never"]
    if preflight and preflight.get("resolution_mode") == "post_promotion":
        command.insert(1, "--rom")
    phases["diagnosis"], diagnosis = run_phase(command, directory, "diagnosis", deadline)
    try:
        report = compact(preflight, diagnosis)
    except (ValueError, TypeError, AttributeError):
        parser.exit(2, f"function-probe: invalid evidence schema; inspect {directory.relative_to(ROOT)}\n")
    flags = None
    if args.flags:
        if preflight is None:
            phases["flags"] = {"status": "skipped-missing-owned-context"}
        else:
            command = [PYTHON, "tools/flag_sweep.py", "src/" + preflight["translation_unit"] + ".c",
                       "--function", preflight["candidate_symbol"], "--target-symbol", preflight["target_symbol"],
                       "--jobs", str(args.jobs), "--json"]
            phases["flags"], flags = run_phase(command, directory, "flags", deadline)
            if flags and flags.get("schema") != "mickey-flag-sweep-summary-v1":
                phases["flags"]["status"] = "invalid-report"
                flags = None
    if flags:
        report["flags"] = {key: flags[key] for key in
                           ("complete", "total_combinations", "scored_combinations", "failed_combinations", "cache_key")}
        report["flags"]["best"] = flags["ranked"][:1]
    report.update(schema="mickey-function-probe-v1", symbol=args.symbol, phases=phases,
                  artifacts=str(directory.relative_to(ROOT)), seconds=round(time.monotonic() - started, 3))
    if preflight:
        report["source"] = preflight["source"]
        report["identity"] = {key: preflight[key] for key in ("target_symbol", "candidate_symbol", "linked_section")}
    (directory / "summary.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        print(f"{args.symbol}: preflight={report['preflight_status']}; linked trial candidate={report['candidate_for_linked_trial']}")
        print("comparison: " + json.dumps(report["comparison"], sort_keys=True))
        print("relocations: " + json.dumps(report["relocations"], sort_keys=True))
        print("mechanism: " + json.dumps(report["mechanism"], sort_keys=True))
        print("next: " + report["next_action"])
        print(f"artifacts: {report['artifacts']}/summary.json (no promotion performed)")
    return int(report["preflight_status"] != "complete" or any(p["status"] != "ok" for p in phases.values())
               or (args.flags and not (flags or {}).get("complete")))


if __name__ == "__main__":
    raise SystemExit(main())
