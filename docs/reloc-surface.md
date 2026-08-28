# Synthesizing the overlay relocation surface

Feasibility spike, lane `lane/reloc-synth`. Prototype: `tools/reloc_surface.py`.

**Question.** Every matched overlay function today carries a hand-derived
`POSTPROCESS` rule and/or a hand-written line in `overlay_undefined_syms.us.txt`.
That bespoke work, not the C, is what gates the 279-candidate overlay pool. Can
the relocation surface for a promoted candidate be derived *mechanically* from
the candidate's object plus the shipped tables, with no per-function hand work?

**Verdict: yes for the addend surface, which is the part that blocks the link.**
The procedure reproduces 100% of the tracked surface the current build exercises
and, applied to candidates that previously died at link, links 14 of 19 tried.
It does not synthesize `.text` extents, section externalization, or the alias
block; those remain separate mechanical problems, two of which are already
solved data (see §5).

## 1. The model

`docs/overlays.md` §5.1-5.4 establishes that a module ships **unrelocated**:
`runlinkDownloadCode` patches each site named by the module's own `reloc1` /
`reloc2` tables after the DMA. What the ROM image therefore stores at a
relocation site is not an address but the record's **stored addend** - the value
the runtime adds its base to.

A C translation unit cannot express that. IDO emits an ordinary `R_MIPS_26` or
`R_MIPS_HI16` + `R_MIPS_LO16` reference to a symbol that has no address in this
build. The project's existing answer is a placeholder extern whose *value* is
supplied as a linker-script assignment, so the linked instruction word carries
exactly the addend the retail image carries. `overlay_undefined_syms.us.txt`
says so in its own header: "Raw overlay relocation addends used by adopted C."

The consequence this spike tests is that **the value is not a judgement call**.
For a candidate whose schedule agrees with the target at the site, it is
readable from the ROM:

| relocation | required symbol value |
|---|---|
| `R_MIPS_26` | `SYNTHETIC_VMA \| (stored_imm26 << 2)` |
| `R_MIPS_HI16` + `R_MIPS_LO16` | `(stored_hi << 16) + sign_extend16(stored_lo)` |
| `R_MIPS_32` | `stored_word` |

minus whatever addend the object's own instruction already carries. Subtracting
the object's addend is what lets one base symbol serve many struct-field
references: IDO puts the field offset in the instruction, so only the base
belongs in the linker script.

Three measured facts anchor the model, taken from the decoded tables at run
time (`tools/overlay_tables.py`):

- Every `SYMBOL` `R_MIPS_26` site in every module stores immediate zero. Those
  are the cross-module and resident calls the runtime resolves; the placeholder
  therefore has to link to the module base, which is what the existing
  `--redefine-sym <sym>=func_overlay_NNN_F0000000_...` rules do by hand.
- Every `LOCAL` and `SYMBOL` `R_MIPS_HI16` site stores immediate zero, and its
  `R_MIPS_LO16` partner stores the target's byte offset. That is why the
  hand-written aliases are named `D_<offset>` and are assigned that offset.
- `JUMP` `R_MIPS_26` sites store the target's module offset shifted right two,
  i.e. an ordinary intra-module `jal` at the synthetic VMA already produces the
  shipped word with no fixup at all.

## 2. Mapping an object offset to a module offset

The one thing the addend formula needs is where in the module the object's
`.text` sits. `config/overlays.us.json` answers it directly: it records one
contiguous `text_ownership` row per source file, so

    module_offset = row(source_stem).offset + object_text_offset

with no name convention, no heuristic, and - importantly - no linked ELF. The
link is exactly what is missing when a promotion fails to resolve, so an earlier
draft that read `build/mickey.us.elf` was wrong in the only case that matters.

A site the module's relocation table does not name is not a relocation site in
the shipped image; reading an addend there reads an instruction word. When any
site for a symbol *is* corroborated by the table, the tool ignores the ones that
are not. That is the ROM's own statement, not a heuristic, and it is what makes
the procedure tolerant of a candidate whose schedule diverges away from the
sites in question.

## 3. Evidence

### 3.1 Replaying the hand-derived surface

`tools/reloc_surface.py --audit` runs the procedure over every overlay object
the Makefile links and scores the synthesized values against the tracked file:

    tracked-value replay: 1773/1773 agree (100.0%), 0 disagree,
                          982 not tracked, 0 unresolved, 27 stale objects skipped

That is the honest test the spike was asked for, at far greater scale than three
hand-picked functions: every symbol value in `overlay_undefined_syms.us.txt`
that the current build actually exercises - including the heaviest bespoke
rules, `overlay_008.c.o` (32 values behind 46 hand-written `--redefine-sym`
arguments), `overlay_001_tail.c.o` (62), `overlay50Initialize.c.o` (17), and
`overlay_009.c.o` (6 plus a 40-line filter spec) - is reproduced exactly from
the baserom and the atlas, with zero refusals.

### 3.2 Replaying the link itself

The 982 symbols the tool values that are *not* in the tracked file are ones the
link defines by other means: other overlays' functions, resident functions, and
absolute aliases from `undefined_syms_auto.us.txt`. Compared against the linked
ELF's own symbol values:

    linked-ELF cross-check: 979 agree, 3 disagree, 0 undefined

The three are `D_80000039`/`D_8000003A`/`D_8000003B` in
`overlay41UpdateColorRecords.c.o`, referenced only by an `R_MIPS_LO16` with no
`R_MIPS_HI16` partner. A lone `LO16` site does not observe the upper half, so
the tool reports the low half. The emitted instruction word is identical either
way; the value differs, the ROM does not.

Total independent agreement: **2752 relocation-symbol values reproduced, three
partial in an unobservable half, none wrong.**

### 3.3 Unblocking candidates that could not link

`tools/promotion_trial.py` classifies all 279 overlay candidates:

| class | count |
|---|---:|
| `text-differs` (already links) | 110 |
| `build-error`: undefined reference to a placeholder | 100 |
| `build-error`: `cannot grow .text` | 31 |
| `build-error`: `refusing to trim nonzero bytes` | 22 |
| `build-error`: digest / truncated relocation | 5 |
| `build-error`: other | 11 |

The undefined-reference class is the one this procedure addresses. Nineteen were
tried end to end - promote, compile, synthesize, rename the undefined symbols to
per-object aliases, emit the assignments, relink, diff the ROM:

| result | count | functions |
|---|---:|---|
| links, ROM produced | 14 | `overlay1ActivateObject`, `overlay1AdvanceGauge`, `overlay2QueryNode`, `overlay5InitializeAudio`, `overlay13DrawActive`, `overlay13UpdateRecord`, `overlay17CalculateEndpoints`, `overlay20UpdateObjectResource`, `func_overlay_002_F0000C90_1857A88`, `func_overlay_014_F00009F4_18702CC`, `func_overlay_022_F0000A7C_1878B84`, `func_overlay_022_F0000D30_1878E38`, `func_overlay_027_F0000624_187BFFC`, `func_overlay_029_F00010C4_187E374` |
| still fails | 5 | `func_overlay_007_F0000324_185C1AC`, `overlay15InitStars`, `overlay1InterpolatePath`, `overlay1MeasureCurves`, `overlay27UpdateCoordinates` |

Every one of the 14 produced **zero out-of-range differing bytes**: the
synthesized surface disturbs nothing outside the promoted function. The
remaining in-range word counts range from 7 to 196 and are genuine codegen
differences - which is the point. Those candidates now have a linked-ROM oracle
and an exact in-range word count where before they had a link error.

For completeness, the five candidates named in the spike brief
(`overlay7DispatchSelection`, `overlay8ScaleOutputs`, `overlay18Load`,
`overlay20BuildTileCommands`, `overlay1CloneRecord`) already link on the
existing surface: promoted, they build first time and reproduce the trial's
2/2/2/4/3 in-range words with no synthesis at all. Their residual is codegen,
not relocation surface. The blocked pool is the 100 undefined-reference
candidates, not these.

## 4. Limits

The five failures are three distinct classes, and none of them contradicts the
addend model.

1. **Schedule divergence at the site** (`func_overlay_007_F0000324_185C1AC`,
   `overlay15InitStars`). The candidate's instructions differ *at* the
   placeholder's own sites, so no consistent addend exists and the tool refuses
   rather than inventing one. It reports the conflicting values per symbol,
   which localizes the divergence. This is the model's stated precondition: the
   addend is only readable where the schedule agrees.
2. **Alias-block coupling** (`overlay1InterpolatePath`, `overlay1MeasureCurves`).
   `overlay_undefined_syms.us.txt` also carries 624 hand-written *alias* lines
   of the form `func_overlay_001_F0000CA8_184D088 = overlay1InterpolatePath;`
   so that other TUs' generated assembly can reach a friendly name. Promoting
   one function in a shared TU changes which symbols that TU defines and can
   strand such a line. A synthesizer that owns only the value lines cannot fix
   this; it has to own the alias block too, which is mechanical from
   `text_ownership` (§5).
3. **Relocation sites outside `.text`** (`overlay27UpdateCoordinates`). The
   prototype scans `.rel.text` only. A jump table or initialized pointer in
   `.data` carries the same kind of site and needs the same treatment against
   the `data_rodata` range.

Two further limits are worth recording because they are invisible until they
bite:

- A lone `R_MIPS_LO16` determines only the low half of its symbol's value
  (§3.2). Byte-identical output, non-canonical symbol value.
- `overlay_undefined_syms.us.txt` currently assigns eight symbol names twice
  (`gOverlay100Entries`, `gOverlay1ModeObject`, `gOverlay1SubmitArg5`,
  `gOverlay4Groups`, `gOverlay59Entries`, `gOverlay77CallbackArgument`,
  `gOverlay77Handle`, `gOverlay77Selection`). `ld` takes the last, and so does
  the tool, so the audit is consistent - but a generated file would not have
  shadowed lines at all.

## 5. What a full implementation needs

The spike proves the hard part. The rest is bookkeeping, and three of the four
pieces are already derivable from data the tree has:

1. **Own the whole generated block.** Emit `overlay_undefined_syms.us.txt` (or a
   generated sibling included by the link) from `tools/reloc_surface.py` over
   every overlay object, rather than appending to a hand-maintained file. That
   removes the alias-block coupling in §4.2, removes the duplicate assignments,
   and makes the surface regenerate on every promotion instead of being
   maintained per function.
2. **Synthesize the alias lines.** `text_ownership` already maps
   `(overlay, offset) -> source`, and the generated identity spells the offset,
   so `func_overlay_NNN_F<off>_<rom> = <friendly>;` is a pure function of the
   atlas plus the C's own symbol names. This is the same data the
   `--redefine-sym func_...=<friendly>` half of every `POSTPROCESS` rule
   encodes by hand.
3. **Synthesize the `.text` extent.** `trim_elf_section.py $@ .text <size>`
   appears in most rules and the size is literally the `text_ownership` row's
   size. That alone would remove a large fraction of the remaining hand-written
   Makefile text, and would turn the `cannot grow .text` class (31 candidates)
   from a build error into a clear "the candidate's text is the wrong size"
   report.
4. **Extend to non-`.text` sites** (§4.3), and decide the section-externalization
   question separately: `externalize_elf_section.py` takes the expected payload
   as a hex literal in the Makefile, which is the one part of the current
   machinery that is not derivable from addresses alone.

A realistic estimate of the unblocking, from the 14/19 sample against the
100-candidate undefined-reference class: **roughly 70-75 overlay candidates gain
a linked-ROM oracle**, taking the measurable pool from 110/279 to about 185/279.
Add item 3 and the 31 `cannot grow .text` candidates become diagnosable as well.
None of this produces a match by itself. What it produces is the ability to
measure, per candidate, in seconds, without a human deriving an ELF contract
first - which is what the pool has been waiting on.

## 6. Cleanroom note

`tools/reloc_surface.py` reads the baserom and `config/overlays.us.json` at run
time and emits only symbol names and the addresses/values the link already
requires. It writes no extracted data, embeds no ROM bytes, and prints no
instruction text. This document quotes no ROM words.
