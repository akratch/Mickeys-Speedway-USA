# Tooling backlog

Friction hit while decompiling, one bullet per idea, concrete enough to act on
without having been there. Entries stay until they are fixed or consciously
dropped. Two audiences: `tools/find_known_objects.py` is this project's own
tool, and the `decomp-workbench` items are for that tool's maintainer
(`~/Desktop/dev/n64-decomp-workbench`).

## `tools/find_known_objects.py`

- **`--min-unmasked-words N`** (and perhaps `--max-masked-fraction`). The tool
  prints the masked-word count and leaves judging to the reader, so a 0x10-byte
  function with 2 of its 4 words masked lands in the table next to a 0xAC0-byte
  whole-object match. `docs/modules.md` §1.2 sets the threshold at 6 unmasked
  words and it has been enforced by hand in a throwaway script every time. A
  policy enforced by prose rather than by the tool that produces the evidence
  will drift.
- **A `candidates` column, and a `--unambiguous` flag.** The worst output is a
  row where several *different* reference symbols match one address: it looks
  like a strong identification until you notice three names on consecutive
  lines. One pass hit it eleven times (ROM `0x17F8`, `0x1B74`, `0x27A0`,
  `0x27C4`, `0x545C`, `0x8D18`, `0x1BE90`, `0x29B98`, `0x29BA8`, `0x29C90`,
  `0x2CB44`). Print the number of distinct matching reference symbols, and drop
  rows above 1 on request.
- **A guard against semantically vacuous matches.** `return 0` and
  `return x->field_2c` match ROM-wide uniquely and would be adopted by any rule
  based on uniqueness alone; they were rejected only because a human read the
  four instructions. Flag any match whose unmasked words are drawn from a very
  small set of common encodings (`jr ra`, `move`, a single `lw`/`addiu`)
  regardless of how unique it is.
- **`--exclude` glob, and an explicit empty-`.text` count in the footer.** The
  tool takes one directory and globs `**/*.o` under it, so restricting a run to
  "libultra only" or "excluding asset blobs" means building a filtered tree of
  symlinks outside the repo first. It also silently skips objects with an empty
  `.text` (see `docs/references.md`), which is correct behaviour but makes
  "scanned N objects" overstate what was compared.
- **`occ`/`romocc` count occurrences of *these bytes*, not of *this
  function*.** A routine that appears twice because the game genuinely links
  two copies is indistinguishable from one that matches twice by coincidence.
  The `epiread`/`epiwrite` pair (`romocc=2` at both `0x730A0` and `0x730F0`)
  had to be separated by reading Mickey's own `jal` targets. An option to
  resolve masked `R_MIPS_26` targets against already-adopted names would have
  decided it mechanically.
- **`--emit-symbols`**, writing `symbol_addrs` lines directly, plus a list of
  already-known addresses so re-runs show only what is new. The transcription
  step is manual today.
- **Print all candidates on one row.** Two libultra objects can be
  byte-identical to each other — `piacs.c`/`siacs.c`,
  `vigetcurrframebuf.c`/`vigetnextframebuf.c`, `__ll_rem`/`__ull_rem` inside
  `ll.c` — differing only in the relocated globals the tool masks.
  Disambiguation comes from the call graph. One row per blob with every
  candidate name on it makes the ambiguity visible instead of looking like two
  findings.

## Build and toolchain

- **`gmake expected`** (copy `build/` to `expected/` from a known-good build),
  so asm-differ's `-o`/`-3` object mode is usable. In ROM mode asm-differ
  derives a function's end address from the *next symbol in the map file*, and
  splat's tail padding pushes that a long way out: `./diff.sh strchr` shows the
  16 real instructions and then keeps going through the next two objects.
  Zero-diff is still unambiguous, but the output is noisy, and the workaround
  (`./diff.sh --format=plain <sym> | grep -c '|'`) is folklore that should be a
  flag.
- **`gmake setup` should smoke-test each tool it installs** (`m2c.py --help`,
  `diff.py --help`, `build.py` on a trivial file) and fail loudly, rather than
  leaving the breakage to be found mid-task. Both vendored Python tools have
  broken this way: asm-differ died at startup on Python 3.12+ at the old pin
  (`ast.Num` removal; fixed by bumping `0a001ce` → `6299ebf`), and m2c failed
  to import under a fresh venv and then again under `pycparser` 2.23+ (now
  pinned `pycparser<2.23`).
- **Record the Python version a vendored tool was validated against, next to
  its pin.** Both breakages above were "it just fails at startup".
- **The build should assert that every `.s` splat wrote is either assembled or
  deliberately excluded.** `ASM_DIRS` was once a hardcoded two-entry list, so
  naming a subsegment `libultra/foo` made splat write `asm/libultra/foo.s`
  which the build silently ignored — no error, just a link missing an object,
  caught only by the SHA1 check. It is now discovered with `find`, excluding
  `asm/nonmatchings` (those are `#include`d into C objects by asm-processor).
- **Standing rule: a subsegment may only be `hasm` if its output is tracked in
  git.** `hasm` buys "splat won't clobber my hand edits", but `asm/` is
  generated, gitignored and wiped by `gmake distclean`, so hand edits there are
  unrecoverable anyway — while splat's refusal to rewrite an `hasm` file let a
  `macro.inc` change leave `asm/entrypoint.s` stale until the next clean build.
  If `asm/` ever gains a genuinely hand-maintained file, move it out of the
  generated tree first.
- **Upstream (asm-processor): take the prelude path from an argument.** splat's
  IDO preset emits a `nonmatching <name>, <size>` marker before the first
  `glabel`, which asm-processor rejects (".text block without an initial
  glabel"), and splat's nonmatchings `.s` files use `endlabel`, which
  asm-processor's `prelude.inc` does not define. Worked around with
  `asm_nonmatching_label_macro: ""` and by passing
  `include/asm_processor_prelude.inc` to `as`. A project should be able to
  point `build.py` at its own `macro.inc` instead of maintaining a parallel
  one.

## decomp-workbench

- **No documented way to build the *target* object in a splat project, and the
  obvious one stops working the moment you succeed.** `compare` wants
  `target.o candidate.o`, and its README assumes the expected object already
  exists. In a splat project it does not: the nearest thing is
  `asm/nonmatchings/<tu>/<sym>.s`, which has to be assembled by hand — and
  splat *deletes* that file as soon as the C implements the function. This
  project's workaround is `tools/wb_compare.sh`. A `decomp-workbench object
  from-rom --rom X.z64 --vram A --size N` (or a splat-aware `--project` flag)
  would make the target side a first-class input.
- **`compare-dumps` rejects an empty dump with a message about *format*, not
  *emptiness*.** Two files objdump had produced no instructions into gave "no
  GNU-style objdump instruction lines …", which reads as "wrong objdump
  flavour". The real cause was that `objdump -b binary --adjust-vma=V
  --start-address=S` reads `S` in the *adjusted* space, so ROM offsets
  alongside a VRAM adjustment select an empty range. If the file has a `file
  format` header and zero disassembly lines, say "dump contains no instructions
  (empty address range?)" and name the file.
- **Fully-relocated `--rom`-style comparison has no verdict of its own.** Both
  sides are linked, so relocation reasoning is structurally unavailable and the
  only possible answers are `instruction-words-identical` or a real difference.
  That verdict is a *link* oracle, not a *codegen* oracle, and the summary does
  not say which the reader got. Add a `--relocations none|expected` note, or
  refuse `--fail-on-mismatch` without relocation records.
- **`--rom`-style comparison uses one address for both sides**, so an oversized
  earlier function silently misaligns a later one: after `SetLinkSlot` came out
  two instructions short, the next function's target dump started eight bytes
  before its real prologue and reported a structural difference that did not
  exist. Resolve the target address from the target-side symbol table
  independently of the candidate's, and warn when the two differ.
- **The metric line does not say which side is which.** `sha1=121f13dcbd30
  build/wb/X.candidate.objdump` names one file at the end of a line whose other
  numbers are comparisons; it is genuinely unclear whether `insns=98` is the
  target's or the candidate's. Print both paths on a header line.
- **`campaign` wants `--diagnose-best`.** The ranking is the right shape and
  `object basins: N across M variants` is the number that matters — it says how
  many of the variants were wasted effort — but the residual for the winner
  needs another `diagnose-dumps` against a path you reconstruct by hand. Print
  the winning object's path on the summary line. `object basins` also deserves
  to be documented as the headline metric in `docs/campaigns.md`.
- **Nothing models "the compiler is wrong".** Both float functions in
  `src/main/matrix.c` are unmatched because the ROM uses odd single-precision
  FP registers and this project's IDO 5.3 never emits one at any of the nine
  ISA/`-O` combinations tried (`docs/modules.md` §6.2). `diagnose` correctly
  reported `fp=46` and pointed at the allocator playbooks, all of which are the
  wrong advice. A toolchain-capability probe — if the target uses odd single FP
  registers and the candidate's compiler has never emitted one, say so — is a
  compiler-identity finding, not an allocator one, and belongs with the
  existing `toolchain-calibration` document.

### Ledgers, and what the upstream redaction does not cover

The Phase 1 incident was a schema hazard, not an operator error: every ledger
diff-site record carried `"target"`, the target object's disassembly, beside
`"target_word"`. Two `ledger.jsonl` files reached the remote with 126 sites
apiece, between them enough to reconstruct 129 of `ProcessRelocationEntry`'s
146 instructions, and the fix was a history rewrite.

Fixed upstream in `n64-decomp-workbench` (`75a3ee5`, then `674b2a9`): the
ledger writer sweeps `dict`, `list`, `tuple`, `set` and `frozenset`, examines
keys as well as values, matches target-naming keys case- and
prefix-insensitively, re-sweeps allow-listed values, and caps depth. What lands
on disk keeps only per-site bookkeeping — a 16-bit salted `target_digest`, a
`target_opcode_masked` word for at most the first three sites of a list, and
`target_register_count`. Terminal diffs, HTML reports and diagnosis paths are
unchanged, because showing the full diff on the operator's own machine is the
point of the tool and only the file is the hazard.

Four residual limits, all of which matter to this repository:

- **A ledger is still not safe to track, and is not meant to be.** Replaying
  both purged ledgers through the redactor removes every target word and every
  line of target assembly, and the result *still* trips this repo's
  `instruction-dump` and `machine-word-dump` detectors, because the *candidate*
  side is instruction text too. Ledgers stay gitignored and outside the
  `.decomp-workbench` path whitelist.
- **`--html` reports are not redacted.** `view --html` and `diagnose --html`
  render the target's assembly rows into a report at a path the operator names.
  Nothing gitignores it by name, so only the content detectors would catch it.
  Do not park one in the tree.
- **The digest's security property depends on the salt.** The ledger keeps
  `candidate` and `candidate_word` in full, which narrows the target at a diff
  site to a small set of plausible variants, and against a small set a
  known-salt 16-bit digest is an exact-match confirmation oracle. Treat
  `<ledger>.salt` as sensitive; the `workbench-path` whitelist already refuses
  to track it.
- **The redactor does not read string contents.** It cannot carry target
  instruction text under a target-named field at any depth in any container; it
  can still carry it under an innocuous key name, or as a bare list element.
- **A second, older copy of the workbench is vendored into the DKR checkout**
  (`~/Desktop/dev/Diddy-Kong-Racing/tools/decomp-workbench`, version 0.2.0),
  with the same `append_ledger` and the same `"target": expected.assembly`, so
  it carries the original flaw. It was deliberately not patched — a backport
  into an unrelated project's history would diverge from the upstream fix
  rather than inherit it. Re-vendor it at the next opportunity; until then
  treat any ledger it writes as ROM-derived.
