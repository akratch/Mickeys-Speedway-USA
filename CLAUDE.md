# Working in this repository

Mickey's Speedway USA, clean-room matching decompilation. The US ROM rebuilds
byte-identically; every commit must keep it that way.

## Private/public release posture

This is the private canonical repository. Work lands through private lanes on
`campaign/unchain`, is merged from there into the private `master`, and is
pushed only to the private `origin`. **Both branches, every time** -- run
`tools/land.sh` after each integration batch. It pushes `campaign/unchain`,
merges it into `master`, re-verifies the ROM from the merge result (the merge
can combine two lanes' generated artifacts in ways neither side produced
alone, which `verify` on `campaign/unchain` does not cover) and pushes
`master`. Leaving `master` behind means it gets merged by hand later.

The `upstream` remote is Rare's public Jet Force Gemini repository, not ours.
Never push there.

The public release is the rewritten mirror at `../mickey-public`, published at
`https://github.com/akratch/mickeys-speedway-usa-decomp` from its `master`
branch through its remote named `public`. Their commit IDs and documentation
are intentionally different.

Commit decomp work privately in function-sized units. A change may be mirrored
publicly only after it is integrated into `campaign/unchain` and the canonical
private tree passes `gmake verify`, `gmake cleanroom`, `gmake check-docs`, and
`gmake check-scoreboard`. Copy only the reviewed public-safe paths; never merge
the private branch or copy the whole private tree into the public mirror.
Create a separate plain public commit with no automated co-author or generator
trailer, regenerate the public scoreboard, repeat the applicable gates in the
public mirror, scan outgoing text/history for private workflow material and
secrets, confirm `git remote -v`, and push only public `master` to `public`.

Never publish private branches or tags, `.codex/`, `AGENTS.md`, `CLAUDE.md`,
local-agent/campaign/orchestration files, workbench or scratch state, absolute
workstation paths, baseroms, extracted assets, ROM-derived data, or compiler
binaries. The standard decomp permuter remains eligible public tooling. Do not
publish experimental candidates, plateaus, or lane-only work. Reproduce an
accepted public contribution in a private lane and integrate it privately
before mirroring the proven result back to public. Never force-push the private
`origin` and never add it as a remote in the public mirror.

## Nothing ROM-derived is ever tracked in git

Not asm, not instruction text, not hexdumps, not extracted assets, not
workbench ledgers. Not temporarily, not to show a diff, not inside a JSON blob.
Two workbench `ledger.jsonl` files were once committed with the ROM's own
disassembly inside them, and undoing it took a history rewrite.

### Never track

| | |
|---|---|
| `asm/`, `assets/`, `expected/`, `baseroms/` | splat output and ROM images |
| `*.z64` `*.n64` `*.v64` `*.bin` | ROM images and extracted binaries |
| `.decomp-workbench/**` | except `campaigns/*/manifest.json`: paths and hashes only |
| instruction text | mnemonics + operands, in any file, in any format |
| hexdumps, byte arrays, base64 of ROM bytes | same content, different clothes |
| `tools/ido/`, `tools/binutils/` | proprietary toolchain binaries, gitignored. If one is ever staged the `binary-blob`/`oversize` rules catch it, not a path rule |

Quoting a couple of instructions in a comment to explain why a function is
named what it is: fine, and `docs/modules.md` does it. Pasting a function's
disassembly: not fine. The detectors draw that line at 40 mnemonic tokens and
1.0 per KiB. The heaviest legitimate file in this tree carries 17
(`symbol_addrs.us.txt`; `include/game/runlink.h` is next at 16), 2.35× under.

## Before every commit

```sh
gmake verify        # must print the expected SHA1
gmake cleanroom     # or: gmake cleanroom CLEANROOM_ARGS=--staged
gmake check-docs    # derived numbers in the docs still match the tree
```

If matching progress changed (a function moved from asm to C, a name was
adopted), also run `gmake scoreboard` and commit the README diff it produces;
`gmake check-scoreboard` fails otherwise.

The clean-room sweep runs automatically once `gmake setup` (or `gmake hooks`)
has pointed git at `.githooks/`:

- **pre-commit** scans the index, what the commit would record.
- **pre-push** scans every commit tree leaving the machine, so a commit made
  before the hooks existed, or in another clone, still cannot ship.
- **CI** re-runs both on push and PR, plus `check-scoreboard --check-partial`
  (`scoreboard.yml`).

Everything else is manual; nothing but the clean-room sweep is wired into a
hook or CI:

| Command | What it checks | Needs a build |
|---|---|---|
| `gmake verify` | ROM rebuilds byte-identically | yes |
| `gmake cleanroom` | no ROM-derived content (hook + CI enforced) | no |
| `gmake check-docs` | derived numbers in the docs match the tree | no |
| `gmake check-scoreboard` | README's Progress block matches the tree right now. Needs the ELF, so CI can only run `--check-partial`; see `docs/CONTRIBUTING.md#checks` | yes |
| `gmake check-overlay-syms` | `overlay_undefined_syms.us.txt` is still what the tree generates. That file is generated by `gmake overlay-syms` from `config/overlays.us.json` and the compiled overlay objects, never hand-edited; a hand edit or a stale promotion shows up here. Run after any overlay promotion | yes (compile-only) |
| `gmake check-nonmatching-builds` | every candidate-bearing TU still compiles with `-DNON_MATCHING`; a TU that does not silently drops its candidates out of the permuter sweep. Run after editing any `#ifdef NON_MATCHING` block | yes (compile-only) |
| `gmake audit-decoders` | the clean-room detectors aren't inventing words. Run after touching `tools/cleanroom_detectors.py` | no |
| `gmake check-fixtures` | the detectors still *catch* real ROM in every encoding and wrap width, the direction `audit-decoders` cannot see. Fixtures are generated from the baserom at run time and never written to disk, so this cannot run in CI | no (needs a baserom) |
| `gmake progress` | prints matched functions/bytes/symbols | yes |
| `gmake scoreboard` | regenerates README's Progress block from the tree | yes |
| `gmake clean` | removes `build/` | no |
| `gmake distclean` | `clean` plus extracted state (`asm/`, `assets/`, linker script, auto-generated `undefined_*`); recovering needs `gmake extract` and a baserom | no |

## Promoting an overlay function

An overlay promotion regenerates *before* it builds. Every step below has cost
a lane a failed cycle, and not one of the errors names the step that is
missing:

```sh
# 1. Re-extract FIRST. Removing a GLOBAL_ASM pragma leaves splat's .s behind;
#    running overlay-syms before this fails on build/.splat-stamp, and
#    building before it fails at link on an undefined reference.
gmake extract

# 2. Add the promoted range by hand to MIXED_TU_EXACT_C_RANGES in
#    tools/overlay_atlas.py. `gmake overlay-atlas-write` will NOT add it --
#    it reports "overlay artifacts current" -- and editing
#    config/overlays.us.json directly is overwritten. Keep each overlay's
#    entries SORTED BY OFFSET, or `gmake build/.splat-stamp` dies with
#    `ValueError: invalid overlay N mixed-TU exact range` from a traceback
#    that names neither the entry nor the ordering rule.

# 3. Regenerate the derived tables.
gmake overlay-syms
gmake overlay-atlas-write
.venv/bin/python tools/refresh_atlas_digest.py

# 4. Build, then regenerate the alias list AGAIN and rebuild.
#    overlay-syms derives its aliases from the compiled overlay objects, so
#    on a first promotion the pre-build run cannot see the new symbol. One
#    pass links with undefined references and R_MIPS_26 overflows.
gmake -j8
gmake overlay-syms
gmake -j8 && gmake verify

# 5. Confirm.
gmake check-overlay-syms
gmake promotion-proof SYMBOL=<symbol>
```

Overlay objects do **not** depend on `mk/overlays.mk`. Editing a POSTPROCESS
rule there triggers no rebuild, and the resulting link failure looks as though
the rule is wrong when it is already correct -- `rm` the object.

**`tools/reloc_surface.py` rewrites resident-call symbol names inside the
compiled overlay objects, and only `gmake overlay-syms` applies it.** So a link
failing with `R_MIPS_26 ... relocation truncated to fit` against resident
symbols is almost always a build-state problem rather than a source one: an
overlay is linked at `0xF0000000` and cannot `jal` a `0x8000xxxx` entry point
directly, so it must go through the generated `*Reloc` surface entry. Run
`gmake overlay-syms`, then build.

This recurs after `gmake extract` **and after editing `symbol_addrs.us.txt`**,
which re-runs splat and rebuilds every object, silently discarding the renames.
`tools/land.sh` regenerates before it verifies for exactly this reason.

The trap it hides: a promotion's `gmake verify` can pass in a worktree that
holds renames the *commit* does not carry, so the tree stops linking for
everyone else. The overlay 60 promotion renamed 8 of its 51 resident callees
that way, and no lane worktree created afterwards could build at all until the
remaining 57 rules were added. If a promotion adds resident calls, check that
`mk/overlays.mk` names every one of them.

`check-overlay-syms` is a *drift* check on already-regenerated output, so it
cannot catch a promotion that never regenerated; reading its table entry below
as "run this after promoting" is what leaves the build broken. Likewise
`promotion-proof` refuses with `expected one tracked exact atlas range for
<sym>, found 0` until step 2 is done.

## What the gates cover

The hooks are client-side, opt-in per clone (`core.hooksPath`), and skippable
with `--no-verify`. CI catches what reaches the remote, but by then the objects
are published and the remedy is a history rewrite. They are depth against
mistakes, not a barrier against a determined bypass.

One server-side layer exists: the GitHub ruleset `protect-master`
(id `20111399`, active on `master`) blocks force-push and branch deletion. It
does not restrict *content*: GitHub refused a push ruleset scoped by file
path/extension/size because push rules require an org-owned repo and this is a
personal fork. A required status check on `master` is still the only way to
block bad content before it lands, and would mean pull requests instead of
direct pushes; that change has not been made. Blocking force-push also means a
future history purge of `master` requires disabling `protect-master` first.

The content rules catch mistakes (an asm dump, a hexdump, a ledger, a base64
blob, a leak spread across files), measured against this repository's whole
history on one side and real ROM fixtures on the other. They are not
adversary-proof: detecting arbitrarily-encoded data is undecidable, a padded
file carrying up to 191 machine words (~764 bytes of ROM; the per-file limit is
192) slips under every threshold, and `--no-verify` skips the lot.
`docs/CLEANROOM.md` lists the measured limits. The load-bearing guarantees are
structural: the `.decomp-workbench` path whitelist and manifest schema check,
the ROM path and binary rules, the tool-level ledger redaction, this policy,
and `protect-master`.

**Never pass `--no-verify`.** Never lower a threshold to make a file pass. If a
file is genuinely a false positive, restructure it, or add it to
`CONTENT_EXEMPTIONS` in `tools/cleanroom_detectors.py` with a written reason.
If something ROM-derived is already committed, it has to be rewritten out of
history (`git rebase -i`, `git filter-repo`); a follow-up commit that deletes
the file still ships the bytes.

## Permitted sources, and disclosing them

`docs/CLEANROOM.md` is the policy; read it before adopting anything from
elsewhere. In short: the five named published retail-derived decomps (Diddy
Kong Racing, Jet Force Gemini, Perfect Dark, Banjo-Kazooie, Conker's Bad Fur
Day) may supply names and adapted function bodies **with a `PROVENANCE` note at
the point of use**. Anything leaked is forbidden outright. Reference repos,
their baseroms and their build outputs live in `~/Desktop/dev/decomp-refs/`,
outside this repo, never committed.

## Evidence discipline

`docs/modules.md` §1 defines four evidence tiers (A byte-identity, B call
graph, C string correspondence, D structural inference) and the Tier A
adoption threshold.

- **Declare the tier inline, per symbol.** A claim with no stated method is a
  bug in the document.
- **A sub-threshold adoption is argued individually**, in the table in §1.2.

## Derived numbers are recomputed, never remembered

Every count in the docs (function totals, matched bytes, segment sizes, the
percentages in `README.md`) is derived from the build and the tree. Recompute
from the underlying lists; do not carry a number forward from an earlier
message or an earlier version of the file. `gmake check-docs` re-derives what
is mechanically checkable and fails on drift; `tools/progress.py --verbose`
shows its working.

## Build quickstart

```sh
gmake setup     # venv, deps, toolchain, baserom SHA1 check, splat, git hooks
gmake -j8       # build/mickey.us.z64
gmake verify    # byte-compare against 507341c0a40ca3e9a7cee969b396ee53facfb548
gmake progress  # matched functions/bytes/symbols
gmake check-docs
```

`gmake`, not `make`: macOS's built-in make is too old. Bring your own legally
dumped ROM at `baseroms/mickey.us.z64`.
