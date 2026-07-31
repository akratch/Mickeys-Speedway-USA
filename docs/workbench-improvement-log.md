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
- **No workbench/`compare.py`-style tooling was needed for this task — DONE in
  Task B, kept for the reasoning.** The TU
  was located by a throwaway script that extracted `.text` from the DKR
  decomp's built libultra objects and searched the ROM for those bytes; it
  found 30-odd byte-identical libultra functions in about a minute. Suggest:
  promote that to a real tool (`tools/find_known_objects.py`) — it is the
  cheapest possible source of both file boundaries and symbol names for the
  whole libultra corridor, and right now it lives only in a scratchpad. First
  use of a real matching workbench is still expected in Task C, on a function
  that does not match on the first try.

## 2026-07-31 — Task B (libultra sweep)

- **`tools/find_known_objects.py` now exists** (the "promote the throwaway
  script" entry above). Two things made it worth writing properly rather than
  pasting the scratchpad version: it is *relocation-aware* (mask the words a
  relocation record says the linker patches, compare the rest verbatim), which
  took the corridor from 30 reloc-free functions to 107 named ones; and
  `--sections` reports whole-object `.text` matches, which is what actually
  pins translation-unit boundaries. Suggest next: teach it to emit
  `symbol_addrs` lines directly (`--emit-symbols`) so the transcription step
  disappears, and to take a list of already-known addresses so re-runs show
  only what is new.
- **Two libultra objects can be byte-identical to each other.** `piacs.c` and
  `siacs.c` compile to the same instructions; only the relocated globals
  differ, which is exactly what the tool masks. So do
  `vigetcurrframebuf.c`/`vigetnextframebuf.c`, and `__ll_rem`/`__ull_rem`
  inside `ll.c`. The tool cannot tell them apart and should not pretend to —
  disambiguation came from the call graph (who `jal`s which one) and from
  which global a neighbouring writer touches. Suggest: when a blob matches
  more than one reference symbol, print all of them on one row rather than one
  row each, so the ambiguity is visible instead of looking like two findings.
- **asm-differ still can't bound a function in ROM mode.** Unchanged from Task
  A and hit again here constantly while checking individual libultra
  functions; the `gmake expected` idea remains the fix and remains undone.
  Working around it by grepping the dump for the `|` mismatch column
  (`./diff.sh --format=plain <sym> | grep -c '|'`) is reliable but is folklore
  that should be a flag.
- **`ASM_DIRS` was a hardcoded two-entry list.** Naming a subsegment
  `libultra/foo` makes splat write `asm/libultra/foo.s`, which the build then
  silently ignored — no error, just a link that is missing an object, caught
  only by the SHA1 check. Now discovered with `find`, excluding
  `asm/nonmatchings` (those are `#include`d into C objects by asm-processor,
  and assembling them separately would double every symbol). Suggest: the
  build should assert that every `.s` splat wrote is either assembled or
  deliberately excluded, instead of relying on the ROM hash to notice.

## 2026-07-31 — Task B review round 1

- **`occ` is window-scoped, and it reads like it is ROM-scoped.** The finder
  reports how many times a blob matched *inside `--start`/`--end`*, so
  `__osDisableInt` prints `occ=1` when scanned over `0x1000..0x6F420` and
  `occ=2` when scanned over the whole static segment — the second copy at
  `0x72810` is simply outside the first window. Every "unique match" claim made
  from a windowed run is therefore weaker than it looks, and this bit the Task
  B report in exactly that way (two piacs symbols asserted as uniquely matched
  when each occurs twice ROM-wide). Suggest: always compute occurrences over
  the whole ROM and print two columns — `occ` (in window) and `romocc` (total)
  — so a windowed scan can never be mistaken for a uniqueness proof. Cheap: the
  masked comparison already runs over arbitrary ranges.
- **Nothing stops a thin match from being reported as a finding.** The tool
  prints the masked-word count and leaves judging to the reader, which is the
  right default but is not a guard: a 0x10-byte function with 2 of its 4 words
  masked is two instructions of evidence and still lands in the table next to a
  0xAC0-byte whole-object match. The Task B report's stated mitigation ("prefer
  whole-.text matches, watch the masked count") is a human procedure, i.e. the
  kind that stops being followed. Suggest: `--min-unmasked-words N` (and
  perhaps `--max-masked-fraction`) so the mitigation is enforced by the tool
  and the threshold used is visible in the command line that produced a table.

## 2026-07-31 — Task C (first matched GAME functions, decomp-workbench in the loop)

Workbench installed from `~/Desktop/dev/n64-decomp-workbench` (not the DKR
checkout's copy), `pip install -e`, version 0.3.1 / commit `d6bedd4`.

- **There is no documented way to build the *target* object in a splat
  project, and the obvious one stops working the moment you succeed.**
  `compare` wants `target.o candidate.o`, and the README's framing ("compare
  your normal full-TU build against the expected object") assumes the expected
  object already exists. In a splat project it does not: the nearest thing is
  `asm/nonmatchings/<tu>/<sym>.s`, which has to be assembled by hand — and
  splat *deletes* that file as soon as the C implements the function, so the
  target object can only be built while the function is still unmatched.
  Confirmed here: after `ResolveRelocAddress` matched, `gmake extract` removed
  `asm/nonmatchings/main/runlink/func_800317F0.s` and wrote no replacement.
  Suggest: ship a `decomp-workbench object from-rom --rom X.z64 --vram A
  --size N` (or a splat-aware `--project` flag) so the target side is a
  first-class input rather than something every project reinvents. This
  project's reinvention is `tools/wb_compare.sh`.
- **`compare-dumps` rejects an empty dump with a message about *format*, not
  about *emptiness*.** Feeding it two files that objdump had produced no
  instructions into gave "no GNU-style objdump instruction lines ... expected
  lines like `1c: 8f998010 lw t9,-32752(gp)`", which reads as "your objdump
  output is the wrong flavour". The actual cause was that `objdump -b binary
  --adjust-vma=V --start-address=S` reads `S` in the *adjusted* space, so
  passing ROM offsets alongside a VRAM adjustment selects an empty range.
  Suggest: distinguish the two cases — if the file has a `file format` header
  and zero disassembly lines, say "dump contains no instructions (empty address
  range?)" and name the file. The current message sent ~15 minutes to the wrong
  hypothesis.
- **`--rom`-style fully-relocated comparison has no verdict of its own.**
  Comparing baserom bytes against built-ROM bytes can only ever return
  `instruction-words-identical` or a real difference, because both sides are
  linked — relocation reasoning is structurally unavailable. The tool happily
  reports `instruction-words-identical`, which is true but flatters the
  evidence: it is a *link* oracle, not a *codegen* oracle, and someone reading
  the verdict later cannot tell which they got. Suggest: a `--relocations
  none|expected` note in the summary line, or refuse `--fail-on-mismatch`
  without relocation records unless `--cross-rom`-style opt-in is passed.
- **The metric line does not say which side is which.** `sha1=121f13dcbd30
  build/wb/X.candidate.objdump` names one file at the end of a line whose other
  numbers are all comparisons; on a first read it is genuinely unclear whether
  `insns=98` is the target's or the candidate's. Suggest: print both paths on a
  header line above the metrics.

### Later in Task C — campaigns, and one limit worth naming

- **`--rom`-style comparison uses one address for both sides, so an oversized
  earlier function silently misaligns a later one.** After `SetLinkSlot` came
  out two instructions short, the very next function's comparison dumped the
  *target* starting eight bytes before its real prologue and reported a
  structural difference that did not exist — the first two rows of the diff
  were the tail of the previous function. Nothing in the output said the two
  sides were being read at different logical positions. Suggest: when a
  target-side symbol table is available, resolve the target address
  independently of the candidate's, and warn loudly when the two differ. Cheap
  and it turns a misleading answer into a correct one.
- **`campaign` is excellent and its ranking is the right shape, but a `--diff
  best` would close the loop.** The eight-variant ProcessRelocationEntry run
  ranked cleanly and reported "object basins: 5 across 8 successful variants",
  which is exactly the number that matters — three of the variants were
  byte-identical objects, i.e. spellings the front end canonicalizes away, and
  knowing that instantly is worth more than the ordering. What it does not do
  is show the residual for the winner: the next command is always another
  `diagnose-dumps` against a path you have to reconstruct. Suggest: print the
  winning object's path on the summary line, or take `--diagnose-best`.
- **`object basins: N across M variants` deserves to be documented as the
  headline metric.** It is the one number that says "you have M-N variants of
  wasted effort here", and it is currently the last line before the ledger
  paths. It answered a real question twice in this task and is not in
  docs/campaigns.md's description of the output.
- **Nothing in the tool models "the compiler is wrong".** Both float functions
  in main/matrix.c are unmatched for a reason no lever can fix: the ROM uses
  odd single-precision FP registers and this project's IDO 5.3 never emits one
  at any of the nine ISA/-O combinations tried. `diagnose` correctly reported
  `fp=46` and pointed at the allocator playbooks, all of which are the wrong
  advice here. Suggest: a toolchain-capability check — if the target uses odd
  single FP registers and the candidate's compiler has never emitted one, say
  so in the verdict, because that is a compiler-identity finding and not an
  allocator one. This connects to the existing `toolchain-calibration`
  document, which is the natural home for a capability probe.

## 2026-07-31 — Task D (ontology seed)

Not workbench findings this time: all four are about
`tools/find_known_objects.py`, which is this project's own tool and is now
carrying more evidential weight than anything else in the tree.

- **`--romocc` should be a column, not a second invocation.** Task B logged
  that `occ` is window-scoped and reads as ROM-scoped; this task had to work
  around it by running every scan twice — once over the window of interest and
  once over `0x1000..0x2000000` — and joining the two outputs in a throwaway
  script, for all four scans. That join is the thing every future naming pass
  will need, and it is exactly the sort of step that gets skipped once. Suggest
  computing occurrences ROM-wide always and printing both (`occ` in-window,
  `romocc` total), as already suggested in the Task B block. Reiterated here
  because it cost real time a second time.
- **The tool should refuse to report a match it cannot distinguish.** Its worst
  output is a row where several *different* reference symbols match the same
  address, because it looks exactly like a strong identification until you
  notice three names on consecutive lines. This task hit it eleven times
  (ROM `0x17F8`, `0x1B74`, `0x27A0`, `0x27C4`, `0x545C`, `0x8D18`, `0x1BE90`,
  `0x29B98`, `0x29BA8`, `0x29C90`, `0x2CB44`). Suggest a `candidates` column
  giving the number of distinct reference symbols matching those bytes, and a
  `--unambiguous` flag that drops rows where it is greater than 1.
- **`--min-unmasked-words` still does not exist and is still the right fix.**
  Task B logged it; this task had to enforce the threshold (6 unmasked words)
  by hand in a script, and the threshold is now written into
  `docs/modules.md` §1.2 as project policy. A policy enforced by prose rather
  than by the tool that produces the evidence is a policy that will drift.
- **Nothing warns when a "unique" match is semantically vacuous.** `return 0`
  and `return x->field_2c` matched ROM-wide uniquely and would have been
  adopted by any rule based on uniqueness alone; they were rejected only
  because a human read the four instructions. Suggest an entropy-style guard —
  flag any match whose unmasked words are drawn from a very small set of common
  encodings (`jr ra`, `move`, a single `lw`/`addiu`) regardless of how unique it
  is. Cheap, and it defends the one failure mode uniqueness cannot.

## 2026-07-31 — Task E (progress metric)

- **The stub's `tools/progress.py` was silently wrong two ways at once, and
  neither was the crash.** It imported `colour` unconditionally (not in
  `requirements.txt`, so `python3 tools/progress.py` failed before printing
  anything) — but fixing the import would have shipped a worse bug: it scored
  "matched" by checking whether a `.s` file *named after the function* exists
  anywhere under `asm/`. That assumption broke on this project's actual
  layout. `asm/main/gzip_asm.s` and every one of the 68 `asm/libultra/*.s`
  files are still whole-TU dumps holding several functions each (e.g.
  `gzip_asm.s` has five `gzip_inflate_*` glabels, none of them named
  `gzip_asm`), so a function with no same-named `.s` file was silently counted
  as "matched" even when it was still 100% hand-disassembled — the old tool
  would have reported all five `gzip_inflate_*` functions as decompiled C, and
  the 107 not-yet-organized `asm/<ADDR>.s` files would have made this worse,
  not better, as the project grows. Fixed by searching for the function's
  *name* as a `glabel`/`alabel` anywhere in the whole `asm/` tree instead of
  matching by filename. Suggest: any future "is X still assembly" check in
  this project's tooling should default to a name search over the tree, never
  a filename convention — the moment a segment stops being one function per
  file (which is most of `asm/` right now), filename matching goes wrong
  without erroring.
- **The ELF's own symbol table contains size-0 `*ABS*` "functions" that are
  not separate functions.** `undefined_funcs_auto.us.txt` /
  `undefined_syms_auto.us.txt` auto-generate `name = 0xADDR;` linker-script
  stand-ins for any name referenced from one file but not (yet) defined in
  it; when the reference and the `glabel`/`alabel` defining it both end up in
  the same not-yet-organized `asm/<ADDR>.s` file, the *definition* still
  resolves as a real function, but a second, spurious `*ABS*` size-0 symbol
  for the same name shows up in `objdump -x`'s output too. Checked one by
  hand: `func_80059278` is an `alabel` for a shared branch-target *inside*
  the single large hand-written function `func_800591B0` in `asm/59DB0.s`,
  not a function of its own. 57 such placeholders exist right now; counting
  them would inflate the "total functions" denominator by names that are not
  distinct functions. `tools/progress.py` excludes any `*ABS*`/size-0 symbol
  from the denominator and reports the exclusion count in `--verbose` output
  so it stays visible. Suggest: if `objdump`'s output ever needs parsing
  again elsewhere in this project, treat `*ABS*`+size-0 as "not a function
  with a body" on sight — it is a reliable, cheap signature for this class of
  placeholder.
