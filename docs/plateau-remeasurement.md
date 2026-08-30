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

Pass exact symbols to remeasure an already selected batch. `--no-build`
requires all canonical and candidate artifacts to be current. JSON output is
available for queue automation:

```sh
.venv/bin/python tools/plateau_remeasure.py --format json \
  func_overlay_026_F0000B18_187AF10 func_8001C114
```

The output belongs in ignored campaign state. Do not commit remeasurement
JSON, candidate objects, disassembly, or other ROM-derived evidence.
