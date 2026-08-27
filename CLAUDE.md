# Working in this repository

Mickey's Speedway USA, clean-room matching decompilation. The US ROM rebuilds
byte-identically; every commit must keep it that way.

## Private/public release posture

This is the private canonical repository. Work lands through private lanes on
`campaign/unchain` and is pushed only to the private `origin`. The public
release is the rewritten mirror at `../mickey-public`, published at
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
| `gmake audit-decoders` | the clean-room detectors aren't inventing words. Run after touching `tools/cleanroom_detectors.py` | no |
| `gmake check-fixtures` | the detectors still *catch* real ROM in every encoding and wrap width, the direction `audit-decoders` cannot see. Fixtures are generated from the baserom at run time and never written to disk, so this cannot run in CI | no (needs a baserom) |
| `gmake progress` | prints matched functions/bytes/symbols | yes |
| `gmake scoreboard` | regenerates README's Progress block from the tree | yes |
| `gmake clean` | removes `build/` | no |
| `gmake distclean` | `clean` plus extracted state (`asm/`, `assets/`, linker script, auto-generated `undefined_*`); recovering needs `gmake extract` and a baserom | no |

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
