# Working in this repository

Mickey's Speedway USA, clean-room matching decompilation. The US ROM rebuilds
byte-identically today; every commit must keep it that way.

## The one rule that outranks the rest

**Nothing ROM-derived is ever tracked in git.** Not asm, not instruction text,
not hexdumps, not extracted assets, not workbench ledgers. Not "temporarily",
not "just to show the diff", not inside a JSON blob where it is hard to see.
This has already gone wrong once: two workbench `ledger.jsonl` files were
committed and pushed with the ROM's own disassembly quoted in their diff-site
records, and undoing it took a history rewrite. That is the incident these
gates exist to make impossible.

### Never track

| | |
|---|---|
| `asm/`, `assets/`, `expected/`, `baseroms/` | splat output and ROM images |
| `*.z64` `*.n64` `*.v64` `*.bin` | ROM images and extracted binaries |
| `.decomp-workbench/**` | except `campaigns/*/manifest.json` — paths and hashes only |
| instruction text | mnemonics + operands, in any file, in any format |
| hexdumps, byte arrays, base64 of ROM bytes | same content, different clothes |
| `tools/ido/`, `tools/binutils/` | proprietary toolchain binaries — gitignored; if one is ever staged the `binary-blob`/`oversize` rules catch it, not a path rule |

Quoting *a* couple of instructions in a comment to explain why a function is
named what it is: fine, and `docs/modules.md` does it. Pasting a function's
disassembly: not fine. The detectors draw that line at 40 mnemonic tokens and
1.0 per KiB — the widest-margin legitimate file in this tree carries 16.

### Before every commit

Run these, in order — `gmake verify && gmake cleanroom && gmake check-docs`:

```sh
gmake verify        # must print the expected SHA1
gmake cleanroom     # or: gmake cleanroom CLEANROOM_ARGS=--staged
gmake check-docs    # derived numbers in the docs still match the tree
```

If matching progress changed (a function moved from asm to C, a name was
adopted), also run `gmake scoreboard` and commit the README diff it produces;
`gmake check-scoreboard` fails otherwise.

The gates run automatically once `gmake setup` (or `gmake hooks`) has pointed
git at `.githooks/`:

- **pre-commit** scans the index — what the commit would record.
- **pre-push** scans every commit tree actually leaving the machine, so a
  commit made before the hooks existed, or in another clone, still cannot ship.
- **CI** re-runs both on push and PR, plus the non-ELF-derived subset of
  `check-scoreboard` (`scoreboard.yml`) — see the table below for why it can't
  run the whole thing.

Everything else below is manual — nothing but the clean-room sweep is wired
into a hook or CI:

| Command | What it checks | Needs a build |
|---|---|---|
| `gmake verify` | ROM rebuilds byte-identically | yes |
| `gmake cleanroom` | no ROM-derived content (hook + CI enforced) | no |
| `gmake check-docs` | derived numbers in the docs match the tree | no |
| `gmake check-scoreboard` | README's Progress block matches the tree *right now* — needs the ELF, so CI can only run `--check-partial` (no baserom to build one from); see `docs/CONTRIBUTING.md#checks` | yes |
| `gmake audit-decoders` | the clean-room detectors aren't inventing words — run after touching `tools/cleanroom_detectors.py` | no |
| `gmake progress` | prints matched functions/bytes/symbols | yes |
| `gmake scoreboard` | regenerates README's Progress block from the tree | yes |

**What these do and do not deliver.** The hooks are client-side. They are
opt-in per clone (`core.hooksPath`), and anyone can step over them with
`--no-verify`. CI catches what reaches the remote, but by then the objects are
published and the remedy is a history rewrite. So this is defence in depth
against mistakes, not a barrier against a determined bypass.

There is one server-side layer now: a GitHub ruleset, `protect-master`
(id `20111399`, active on `master`), blocks force-push and branch deletion —
genuinely non-bypassable, no `--no-verify` for it. It does not restrict
*content*, though: GitHub refused a push ruleset that would have blocked by
file path/extension/size, because push rules require an org-owned repo and
this is a personal fork. A required status check on `master` is still the only
way to block bad content before it lands (it would mean pull requests instead
of direct pushes to `master`), and that workflow change hasn't been made.
Blocking force-push also has a cost worth remembering: purging `master`'s
history again — as already happened once — means disabling `protect-master`
first.

**What the gates are and are not.** They catch *mistakes* — an asm dump, a
hexdump, a ledger, a base64 blob, a leak spread across files — measured against
this repository's whole history on one side and real ROM fixtures on the other.
They are **not** an adversary-proof boundary: detecting arbitrarily-encoded data
is undecidable, a file under ~96 machine words slips under every threshold, and
`--no-verify` skips the lot. `docs/CLEANROOM.md` lists the measured limits. The
load-bearing guarantees are structural — the `.decomp-workbench` path whitelist
and manifest schema check, the ROM path and binary rules, the tool-level ledger
redaction, this policy, and `protect-master` (force-push/deletion only — see
above for why it can't restrict content). A green gate is not permission to
skip reading your own diff.

**Never pass `--no-verify`.** Never lower a threshold to make a file pass. If a
file is genuinely a false positive, restructure it, or add it to the allowlist
in `tools/cleanroom_detectors.py` with a written reason that survives review.
If something ROM-derived is already committed, it has to be rewritten out of
history (`git rebase -i`, `git filter-repo`) — a follow-up commit that deletes
the file still ships the bytes.

## Permitted sources, and disclosing them

`docs/CLEANROOM.md` is the policy; read it before adopting anything from
elsewhere. In short: the five named published retail-derived decomps (Diddy
Kong Racing, Jet Force Gemini, Perfect Dark, Banjo-Kazooie, Conker's Bad Fur
Day) may supply names and even adapted function bodies **with a `PROVENANCE`
note at the point of use**. Anything leaked is forbidden outright. Reference
repos, their baseroms, and their build outputs live in `~/Desktop/dev/decomp-refs/`
— outside this repo, never committed.

## Evidence discipline

`docs/modules.md` §1 defines four evidence tiers (A byte-identity, B call
graph, C string correspondence, D structural inference) and the Tier A
adoption threshold. Two things follow from it:

- **Declare the tier inline, per symbol.** A claim with no stated method is a
  bug in the document, not a fact about the ROM.
- **A sub-threshold adoption is argued individually**, in the table in §1.2,
  not waved through.

## Derived numbers are recomputed, never remembered

Every count in the docs — function totals, matched bytes, segment sizes, the
percentages in `README.md` — is derived from the build and the tree. Recompute
them from the underlying lists; do not carry a number forward from an earlier
message or an earlier version of the file. `gmake check-docs` re-derives what
is mechanically checkable and fails on drift; `tools/progress.py --verbose`
shows its working. A count audit already found several of these stale at once.

## Build quickstart

```sh
gmake setup     # venv, deps, toolchain, baserom SHA1 check, splat, git hooks
gmake -j8       # build/mickey.us.z64
gmake verify    # byte-compare against 507341c0a40ca3e9a7cee969b396ee53facfb548
gmake progress  # matched functions/bytes/symbols
gmake check-docs
```

`gmake`, not `make` — macOS's built-in make is too old. Bring your own legally
dumped ROM at `baseroms/mickey.us.z64`.
