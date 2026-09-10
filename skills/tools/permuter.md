# decomp-permuter in Mickey

Use the permuter as a bounded batch job on one already-plausible, typed C
candidate -- not as a source of decompiled logic, and not inside the main
matching loop. It only rearranges/rewords code that is already structurally
correct; it cannot invent missing control flow, and its winning mutations
(`do{}while(0)`, nested assignments, temp variables with no name) read as
noise if pasted back verbatim. Take the *idea* the diff shows, then rewrite
it idiomatically by hand.

```sh
./tools/permute.sh <function> [permuter.py args...]
PERMUTE_MINUTES=20 ./tools/permute.sh <function>   # wall-clock cap, default 20 min
```

Requires `tools/permuter` to exist -- a symlink to a local decomp-permuter
checkout (gitignored, machine-specific; the commit this project was proven
against is pinned in `requirements.txt`'s comment). `./tools/check_tools.sh`
reports whether it's present.

The script locates the function's C file and its target `.s`, imports both
into `build/permuter/<function>/scratch/` (gitignored), runs `permuter.py -j
<ncpu-2> --stop-on-zero`, and prints the base score, the best score found,
and the winning candidate's diff.

Two cases for the target, both handled automatically:

- Still `#pragma GLOBAL_ASM(...)`: `asm/nonmatchings/**/<function>.s` is used
  directly.
- Already implemented in C (matching or not): splat stops emitting that
  function's `.s` once a C definition exists, so the script temporarily
  swaps the function's body for a `GLOBAL_ASM` pragma, re-runs `gmake
  extract` to pull the real target straight from the baserom, captures it,
  and restores the original C -- before running the permuter at all. Nothing
  under `asm/` is ever committed (gitignored wholesale).

`tools/permuter_settings.toml` hardcodes `compiler_command`/`assembler_command`
for the libultra default flag group (`-O2 -mips1 -32`); its header comment
explains why (`import.py`'s Makefile-dry-run auto-detection breaks on this
project's backslash-continued C rule) and how to retarget it for `src/main`
or `src/overlays` functions (`-O2 -mips2 -32`) or the other libultra flag
groups.

**The importer reformats the source, so a physical-line lever does not
survive into the scratch.** `import.py` re-prints the translation unit from
its own parse -- braces moved, statements re-split one per line -- and
`as1`'s scheduler tie-break reads physical source line numbers
([L59](../../docs/ido-learnings.md)), so any statement fold that is load
bearing is undone before the first candidate is generated. Measured on
`func_overlay_071_F0000870_18CA390`, 2026-09-10: the tree's source scores 11
relocation-masked words and the permuter reported `base score = 420`, the
unfolded form's score, then spent fourteen minutes improving the wrong base
to 200. Check the scratch `base.c` against the real object before believing a
base score, and treat a base score that disagrees with
`tools/score_symbol.py` as a scratch-fidelity failure rather than a scoring
convention.

A zero score is not the goal here -- the goal is a shorter list of candidate
edits for a human (or the matching loop) to evaluate. Never commit anything
from `build/permuter/`.
