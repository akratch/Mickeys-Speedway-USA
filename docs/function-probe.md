# Compact function probe

`tools/function_probe.py` composes existing proof preflight, workbench
diagnosis, and optional flag-lattice search. It does not replace their
evidence rules or execute promotion trials.

```sh
.venv/bin/python tools/function_probe.py SYMBOL --seconds 300
.venv/bin/python tools/function_probe.py SYMBOL --no-build --json
.venv/bin/python tools/function_probe.py SYMBOL --flags --seconds 300
```

Run compile-enabled probes in an isolated lane. The first phase selects the
configured full-TU candidate and refreshes its evidence. Diagnosis then uses
`--no-build`, so failed ownership validation can still leave a useful fresh
object without silently accepting stale evidence. A failed preflight remains
unavailable in the compact report; diagnosis alone cannot repair it.

Output separates owned bytes, positional and geometry scores, static/runtime
relocation counts, the workbench's routing/lever class, and whether the
evidence supports a **separate linked promotion trial**. Even that flag is
only a recommendation, never matching credit. Partial relocation identities,
missing evidence, non-exact comparison, or unequal extents cannot receive it.
The workbench's first divergent row includes its comparison evidence; do not
mistake it for the flag sweep's relocation-masked first byte mismatch.

Each invocation retains complete phase JSON, stderr logs, and a scalar
`summary.json` under a unique ignored `build/function-probe/` directory.
Instruction-bearing diagnosis rows never reach compact output. They remain
local artifacts and must never be tracked or published. A total subprocess
deadline covers the phases; timeout terminates the launched process group and
records a non-success verdict. No generated game binary or ROM is executed.

`--flags` explicitly runs the compiler lattice, using the resolved owning TU,
candidate and target names. `tools/flag_sweep.py --json` now emits scalar
coverage and ranking separately from its human logs. It labels a zero score
`masked_exact`, never a promotion proof, and lists failed combinations rather
than declaring them explored successfully. No scored combination is an error;
a partly failed lattice remains partial coverage. The probe exits nonzero for
partial preflight, failed phases, or incomplete requested flag coverage.
`--no-build` and `--flags` cannot be combined.

Known limitation at this checkpoint: the standalone flag sweep still uses
its existing generic define/include baseline, not the full per-TU compiler
argument recipe. Treat that optional phase as exploratory until recipe
fidelity is implemented and tested; it cannot establish configured-TU
equivalence even when the masked score is zero.

The probe is diagnostic only: it never changes source, compiler policy,
symbol names, or promotion guards. A proposed flag change still requires
target evidence and review of every consumer of its flag group. Full linked
range, relocation identity, and ROM verification remain mandatory.
