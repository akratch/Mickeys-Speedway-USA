# Plateau remeasurement

The ready queue fails closed when a guarded source candidate has plateau prose
but no exact-symbol structured handoff. Those rows are maintenance work, not
safe matching assignments: the prose may describe old source, old compiler
output, or incomplete relocation evidence.

Use the bounded remeasurement driver to refresh the closest rows through the
same full-TU candidate, workbench, frame, boundary, and relocation pipeline as
promotion preflight:

```sh
.venv/bin/python tools/plateau_remeasure.py --top 10
```

The command emits scalar evidence only. It never writes source handoffs and
never promotes a function. A `complete` result means the current candidate's
geometry and relocation identities are fully measured; it does **not** mean
the function matches. A `partial` result remains useful for prioritization but
must stay fail-closed until the reported identity or boundary problem is
resolved.

Every selected symbol is attempted serially with the configured per-command
timeout. In a discovered or explicit multi-symbol batch, one failed preflight
does not suppress later symbols. The output retains selection order and has one
row per symbol. Failed rows use `status: failed`, one stable
`failure_category` (`command-failed`, `timeout`, `malformed-json`,
`invocation-failed`, or `invalid-evidence`), and a bounded sanitized reason.
They never contain preflight stdout/stderr, absolute paths, tracebacks,
instruction bytes, disassembly, or relocation details, and they are never
counted as measurements. JSON and table output both finish with aggregate
selected, measured, complete, partial, and failed counts. The process exits
nonzero if any selected row failed, even though it emits the complete batch.

Pass exact symbols to remeasure an already selected batch. `--no-build`
requires all canonical and candidate artifacts to be current. JSON output is
available for queue automation:

```sh
.venv/bin/python tools/plateau_remeasure.py --format json \
  func_overlay_026_F0000B18_187AF10 func_8001C114
```

The output belongs in ignored campaign state. Do not commit remeasurement
JSON, candidate objects, disassembly, or other ROM-derived evidence.

A one-symbol request follows the same contract: success emits one measurement
and exits zero; a command or evidence failure emits one sanitized failed row
and exits nonzero. Invalid CLI input, failed discovery, or an empty selection
still stops before measurement because there is no trustworthy symbol list for
a batch result.
