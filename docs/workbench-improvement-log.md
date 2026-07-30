# Workbench improvement log

Friction encountered while using the decompilation tooling, one dated bullet
per idea, concrete enough that someone can act on it later without having been
there. This is a backlog, not a changelog: entries stay until they are fixed or
consciously dropped.

## 2026-07-31 — Task A (first C translation unit)

- **asm-differ was broken on Python 3.12+ at the pinned commit.** `./diff.sh
  <symbol>` died with "Failed to parse --base-shift (-S) argument as an
  integer" for *every* invocation, because `eval_` tested `isinstance(node,
  ast.Num)` and `ast.Num` no longer exists. Upstream `main` fixes it; the
  submodule was bumped from `0a001ce` to `6299ebf`. Suggest: the moment any
  vendored tool is pinned, record the Python version it was validated against
  next to the pin, so the next "it just fails at startup" is diagnosed in
  seconds instead of by reading `eval_expr`.
- **asm-differ overruns the end of a function in ROM mode.** `./diff.sh strchr`
  shows the 16 real instructions and then keeps going through the tail padding
  and the whole of the next two objects, because it derives the end address
  from the *next symbol in the map file* and splat's tail padding pushes that
  a long way out. Zero-diff is still unambiguous, but the output is noisy and
  gets much worse for a short function followed by a long one. Suggest: add a
  `gmake expected` target (copy `build/` to `expected/` from a known-good
  build) so `-o`/`-3` object mode is usable, since object mode is bounded by
  the object's own symbol sizes.
- **asm-processor and splat disagree about the nonmatchings prelude.** Two
  separate papercuts, both hit on the very first C file: splat's IDO preset
  emits a `nonmatching <name>, <size>` marker line before the first `glabel`,
  which asm-processor rejects outright ("`.text` block without an initial
  glabel"); and splat's nonmatchings `.s` files use `endlabel`, which
  asm-processor's `prelude.inc` doesn't define. Worked around with
  `asm_nonmatching_label_macro: ""` and by passing
  `include/asm_processor_prelude.inc` to `as` as an extra input file. Suggest:
  `build.py` should take the prelude path from an environment variable or
  argument so a project can point it at its own `macro.inc` instead of
  maintaining a parallel one — worth an upstream PR.
- **`hasm` subsegments go stale silently — RESOLVED, kept for the reasoning.**
  Turning off `asm_nonmatching_label_macro` made splat regenerate
  `include/macro.inc` without the `nonmatching` macro, but splat deliberately
  never rewrites `hasm` files once they exist, so `asm/entrypoint.s` kept using
  a macro that no longer existed. The build stayed green until the next
  `gmake clean`, because the stale `.s` had already been assembled. My first
  fix was to delete and re-extract the one file; that was treating the symptom.
  The real problem is that `hasm` and a generated, gitignored, `distclean`-wiped
  `asm/` tree are contradictory: `hasm` buys you "splat won't clobber my hand
  edits", but hand edits here are unrecoverable anyway. The entrypoint
  subsegment is now plain `asm` and splat owns it. Suggest as a standing rule:
  a subsegment may only be `hasm` if its output is tracked in git; if `asm/`
  ever gains a genuinely hand-maintained file, move that file out of the
  generated tree first.
- **m2c has no pin and a broken transitive dependency.** `tools/m2c/m2c.py`
  fails to import under a fresh venv (no `pycparser`), and then fails again
  with `pycparser` 2.23+ (`pycparser.plyparser` was removed). Now pinned as
  `pycparser<2.23` in `requirements.txt`. Suggest: `gmake setup` should
  smoke-test each tool it installs (`m2c.py --help`, `diff.py --help`,
  `build.py` with a trivial file) and fail loudly, rather than leaving the
  breakage to be discovered mid-task.
- **No workbench/`compare.py`-style tooling was needed for this task.** The TU
  was located by a throwaway script that extracted `.text` from the DKR
  decomp's built libultra objects and searched the ROM for those bytes; it
  found 30-odd byte-identical libultra functions in about a minute. Suggest:
  promote that to a real tool (`tools/find_known_objects.py`) — it is the
  cheapest possible source of both file boundaries and symbol names for the
  whole libultra corridor, and right now it lives only in a scratchpad. First
  use of a real matching workbench is still expected in Task C, on a function
  that does not match on the first try.
