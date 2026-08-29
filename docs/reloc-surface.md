# Synthesizing the overlay relocation surface

Implemented. Tool: `tools/reloc_surface.py`. Generated artifact:
`overlay_undefined_syms.us.txt`. Gates: `gmake overlay-syms` writes it,
`gmake check-overlay-syms` fails on drift.

Sections 1-4 are the model and the feasibility evidence (lane
`lane/reloc-synth`); section 5 is what the full implementation turned out to
need and what it measured (lane `lane/reloc-synth2`).

**Question.** Every matched overlay function today carries a hand-derived
`POSTPROCESS` rule and/or a hand-written line in `overlay_undefined_syms.us.txt`.
That bespoke work, not the C, is what gates the 279-candidate overlay pool. Can
the relocation surface for a promoted candidate be derived *mechanically* from
the candidate's object plus the shipped tables, with no per-function hand work?

**Verdict: yes.** `overlay_undefined_syms.us.txt` is now generated in full --
every value line and every alias line -- from `config/overlays.us.json` and the
compiled overlay objects, and the ROM stays byte-identical. 2,928
hand-maintained lines became 1,596 values plus 669 aliases over 630 objects,
with none of the hand file's 8 duplicate names and none of its 168 shadowed
assignments. The measurable overlay candidate pool went from 110/279 to
150/279, and a further 44 that used to be an opaque build failure now report an
exact `.text` size delta.

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

For `overlay20UpdateObjectResource`, that linked-C trial survives only as this
historical report: no attributable trial object, ELF, map, ROM, or report is
retained. Current linked equality therefore proves the assembly fallback only.
Its four runtime-table SYMBOL calls are nevertheless authenticated at function
`+0x14/+0x30/+0x134/+0x168`: resident ORT 202 (`trackGetTrack`), resident ORT
203 (`func_8000FEEC`), Overlay 20 ORT 1690 (`+0xA8`,
`overlay20ConfigureResource`), and resident ORT 101 (`sqrtf`). Fresh C must
preserve those identities; fallback placeholder names do not prove them.

The same qualification applies to `overlay5InitializeAudio`: retained genuine
C is current-layout but diagnostic 211/233. It owns 67 SYMBOL and four LOCAL
records: 29 calls and 21 HI16/LO16 pairs. ORTs 1559/1561 and 1565/1568 were
collapsed behind two friendly names and now have distinct source identities;
all 16 resident callees also require per-overlay zero-carrier aliases. ORT 1558
exports the function and Overlay 18 table-1 record 3 at `overlay18Load+0x14` is
its sole inbound. Exact linked function/module/ROM evidence uses the fallback.

For completeness, at the time of the spike the five candidates named in the brief
(`overlay7DispatchSelection`, `overlay8ScaleOutputs`, `overlay18Load`,
`overlay20BuildTileCommands`, `overlay1CloneRecord`) already link on the
existing surface: promoted, they build first time and reproduce the trial's
2/2/2/4/3 in-range words with no synthesis at all. Their residual is codegen,
not relocation surface. This is historical oracle evidence, not current queue
status; later exact promotions such as `overlay8ScaleOutputs` supersede their
listed residual. The blocked pool was the 100 undefined-reference candidates,
not these.

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
   `text_ownership`. It does now: §5.3.
3. **Relocation sites outside `.text`** (`overlay27UpdateCoordinates`). The
   prototype scans `.rel.text` only. A jump table or initialized pointer in
   `.data` carries the same kind of site and needs the same treatment against
   the `data_rodata` range. In the full run no candidate failed this way -- the
   class the trial reports for `overlay27UpdateCoordinates` is now
   `schedule-divergence-at-site` -- but the generator still scans `.rel.text`
   only, so the limit stands.

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

## 5. The full implementation

The spike proved the addend model. Building the generator on top of it turned
up three corrections to that model, all of them cases the spike's sample had
not exercised.

### 5.1 The object list comes from the linker script

The spike filtered build artifacts by whether the Makefile mentioned the
object's name, to skip stale ones. That silently dropped the **21 overlay
objects that reach the link through a pattern rule** and are never named
literally -- overlay 34's, 35's, 94's, 104's and 105's among them, which is
exactly why a handful of tracked values looked unreproducible. `mickey.us.ld`
names every input object explicitly, so it is the authoritative list: 630
objects, not 609. Replaying the historic hand-maintained file against the
complete list scores **1,840/1,840 values agree, 0 disagree**, up from the
spike's 1,773 on the incomplete one.

### 5.2 A value line is also needed for symbols this build *defines*

The spike only valued **undefined** symbols. The hand-maintained file also
assigned symbols the C defines -- `gOverlay77PositiveDivisor`,
`gOverlay57Countdown`, `gOverlay79RaceFlags` -- and dropping those changed ten
instruction words in overlay 77.

The reason is the same one §1 gives. The module's data is placed by the
*runtime*, not by this link, so a reference to it must carry the stored addend
(its offset within the module's data region) rather than whatever address `ld`
happens to give the definition. A linker-script assignment overrides the
definition, which is what makes that expressible at all.

The one reference that needs no assignment is an **intra-module call**: a JUMP
record stores the target's module offset shifted right two, which is exactly
what the assembler emits for that symbol at the synthetic VMA. So the rule is:

> value every symbol a relocation names, except one defined in this module's
> own `.text`.

That is derivable without a link -- the module's own objects say which names
they define in `.text` -- and it covers resident functions, other modules'
functions, and this module's own data uniformly.

### 5.3 An aliased identity must not also carry a value

`func_overlay_045_F000000C_188C464` is both a cross-module call placeholder
(wants the addend `0xF0000000`) and the generated identity of
`overlay45CreateDescriptor` (wants the real address). The hand file carried
both lines and `ld` silently took the last, which happened to be the alias.
The generated block emits the alias only, so the file has **no duplicate
names**: the 8 the hand file assigned twice, and the 168 values it shadowed
with an alias, are simply not written.

### 5.4 The `.text` extent, as a report instead of a failure

`trim_elf_section.py $@ .text <size>` appears in 588 Makefile rules and the
size is the `text_ownership` row's own extent. A promoted candidate whose
codegen is a different size therefore trips the trim guard at *compile* time,
and the build dies before the link -- which is why 53 of the 279 candidates
used to report nothing but `cannot grow .text` or `refusing to trim nonzero
bytes`.

`tools/postprocess_guard.py` gives every digest-guarded POSTPROCESS pass a
report-and-skip mode, enabled by `PROMOTION_TRIAL` in the environment
(`gmake PROMOTION_TRIAL=1`, or `tools/promotion_trial.py`, which sets it for
its own builds). The guard prints one marker line and skips its pass:

    PROMOTION-TRIAL: text-size-differs (+24 bytes): ... refusing to trim
    nonzero bytes from .text

The resulting ROM is not a valid build -- a skipped normalization leaves the
object un-normalized -- and is never verified; the trial classifies from the
marker. **The normal build never sets the variable**, so `gmake` and
`gmake verify` are untouched, and a usage error is never skipped, so a harness
bug cannot hide behind a green build.

### 5.5 Integration: the window between compile and link

The surface can only be derived once the candidate's object exists and before
the link resolves it -- a window a plain `gmake` does not offer.
`tools/promotion_trial.py` therefore builds, regenerates
`overlay_undefined_syms.us.txt` from the objects on disk, and builds again; the
second pass relinks only. Both the source file and the surface are restored
afterwards.

### 5.6 A resident call is an ordinary addend with a name that is not free

`tools/promotion_trial.py` reported 15 candidates as `resident-symbol-missing`
and 4 as `relocation-truncated (R_MIPS_26)`. Both classes have one cause, and
it is not the addend model.

**What the ROM stores.** Section 5.2's census says a module's `SYMBOL`
`R_MIPS_26` record stores immediate zero, and it makes no distinction between a
target in another module and a target in the resident segment. Measured at the
sites themselves, that holds: overlay 49's three resident calls -- the ones the
Makefile rebinds by hand to `overlay65UpdateReloc` -- are `SYMBOL` `R_MIPS_26`
records at module offsets `0x218`, `0x2c0` and `0x314`, and each stores
immediate zero. The shipped word is `jal 0`; the runtime patches it, exactly as
it patches a cross-module call. The trampoline encoding `0C00CCE8` that 370 of
the 375 *`mainRelocTable`* entries carry (§5.3) belongs to the **resident**
segment's calls *into* overlays, not to a module's calls out of one, and does
not appear at these sites. A resident **data** reference is the other measured
case and needs no patch at all: its `HI16`/`LO16` pair stores the real resident
address, which is why the surface already carries lines like
`D_80000040 = 0x80000040` and why they are correct as they stand.

So a resident call wants the same value every other cross-module call wants,
`0xF0000000`. Nothing about the addend is special.

**What is special is the name.** Adopted C spells the call with splat's
resident auto-name, `func_80029FE4`. That name is global and shared with the
resident segment. A value line for it does not give the overlay an addend, it
*moves the resident function for every resident caller*: assigning
`func_80034448 = 0xf0000000` turns `models.c`, `level.c`, `menu.c`,
`texLoadTextureAddr.c` and four asm objects into `relocation truncated to fit:
R_MIPS_26`. That is the whole of the `relocation-truncated` class, and it is
also why the earlier attempt to rename these placeholders to the *real*
resident symbols failed the other way round: a `jal` from the module's
synthetic `0xF0000000` VMA cannot reach a `0x8…` address either. Neither the
overlay's name nor the resident's name can carry both meanings.

The auto-name shape is not the defining property, either. `overlay5InitializeAudio` calls `alHeapDBAlloc`, `osCreateMesgQueue` and `n_alCSPSetMessageQ` --
ordinary libultra globals -- and valuing those breaks `audio_manager_1050.c`
and the whole libultra link the same way. What the two shapes have in common is
that the **resident side of the link owns the name**, so that is what the
generator measures: `resident_defined_names()` collects the global symbols
defined by every non-overlay object `mickey.us.ld` names, plus the names
`undefined_funcs_auto`, `undefined_syms_auto` and `libultra_undefined_syms`
assign -- 4,886 of them, available before the link and without it. A call to
another *module's* function is deliberately excluded: that is the
generated-identity alias case of §5.3 and must keep its own name.

**The fix is the rebind the tree already does by hand.** Overlay 49's
POSTPROCESS rule rebinds the relocation at `0x218` from `func_800254FC` to
`overlay65UpdateReloc`, a per-module placeholder, and values *that*. The
generator now derives the same thing: `reloc_surface.py generate` renames every
undefined `R_MIPS_26` relocation against a resident-owned name to
`<name>_o<NNN>Reloc` in the object, then values the alias from the site like
any other placeholder. The resident name keeps its real address; the overlay
keeps its stored addend.

Three properties make it safe to run inside `generate`:

- **It is a no-op on the matching tree.** No overlay object in the current
  build carries an `R_MIPS_26` against a resident auto-name -- every matched
  resident call is already rebound by a hand-written rule -- so the generated
  block is byte-for-byte what it was, `--audit` stays at 100%, and
  `check-overlay-syms` reports no drift.
- **It is idempotent.** The alias no longer matches the resident-name pattern,
  so a second pass renames nothing and values the alias from the same site.
  `generate --check` therefore produces the same block whether or not the
  objects have already been rebound. (`--compare` does not rebind: it is a
  read-only report.)
- **It names what it cannot read, and a refusal is total.** Only `R_MIPS_26`
  sites are aliased. A resident symbol reached by *both* a call and a data
  reference is refused -- one placeholder cannot carry two different addends --
  as is a call in an object with no `text_ownership` row, which has no module
  offset. A refused name must then get **no value line under its global name
  either**, and that is the trap that made the first cut of this change worse
  than the problem it fixed: `synthesize()` still reads an addend for the
  refused symbol from its corroborated sites, and emitting
  `func_8002A8C0 = 0xf0000000` to help one overlay produced thirty-odd
  `relocation truncated to fit` errors in `shadows.c`, `camera.c`,
  `charControl.c` and `track.c`. `generate()` now drops a refused resident name
  from every object's values, the reason is reported once, and the trial
  classes the candidate `resident-call-unreadable`.

  **Corroboration is a note, not a refusal.** A call site the module's table
  does not name is where the candidate's schedule has diverged. Under the
  alias, an addend read there can only produce a differing word *inside* the
  promoted function -- which is the measurement the trial exists to take -- so
  the generator emits the value and a `/* NOTE … */` line naming the offsets
  rather than throwing the symbol away. This was measured, not assumed: the
  strict version cost `overlay34SortAndDraw` its 168-word reading and returned
  it to a bare build failure. Notes are printed under their own marker so a
  caller matching `UNRESOLVED` does not read a measurable candidate as a
  failure.

Overlay 59's entry preparer demonstrates both roles in one body. Its calls at
module `+0xB8/+0x140` are local `JUMP` records to `overlay59Release`, while
`+0xD4/+0x104` are resident `SYMBOL` records to `func_80034448`. All four store
zero and collapse to one symbol in the synthetic target object. The candidate
therefore uses separate per-module aliases valued at `0xF0000000`, with
postprocessing preserving the target object's collapsed surface; assigning
the resident global itself would corrupt every resident caller of
`func_80034448`.
The function's other two records are LOCAL HI16/LO16 at function
`+0x08/+0x10`, resolving through base `+0xA20` plus stored addend `+0x5A4` to
module `+0xFC4` (`gOverlay59DescriptorTables`). Genuine C has all six records;
fallback and target retain only the four collapsed calls. The function is
unexported. These are table-2 records 0/1 (descriptor pair), 2/3 (release
JUMPs) and table-1 records 1/2 (ORT 135 acquisitions). Four local inbounds are
table-2 records 25/28/29/30 from `overlay59Advance+0xAC/+0x224/+0x2A8/+0x300`.


#### What it measured

Re-trialled: the 56 overlay candidates whose `NON_MATCHING` body names a
resident target (splat auto-name or `D_8…`), which is a superset of the 15
`resident-symbol-missing` and every `relocation-truncated` case among them.
**`resident-symbol-missing` and `resident-call-unreadable` are now zero.** The
15 the lane was pointed at:

| candidate | before | after |
|---|---|---|
| `overlay1UpdateAimedTransient` | `resident-symbol-missing` | `text-differs` 59 words (2 out of range) |
| `overlay4UpdateObjectMotion` | `resident-symbol-missing` | `text-differs` 15 |
| `overlay5InitializeAudio` | `resident-symbol-missing` | `text-differs` 22 |
| `func_overlay_011_F0001E4C_186A694` | `resident-symbol-missing` | `schedule-divergence-at-site` |
| `func_overlay_011_F00022E8_186AB30` | `resident-symbol-missing` | `schedule-divergence-at-site` |
| `overlay11UpdateMenu` | `resident-symbol-missing` | pre-current-identity diagnostic C: 16 annotated raw differences, 2 after runtime normalization |
| `func_overlay_026_F0000D24_187B11C` | `resident-symbol-missing` | `text-differs` 39 |
| `func_overlay_027_F0000064_187BA3C` | `resident-symbol-missing` | `schedule-divergence-at-site` |
| `func_overlay_029_F00005C4_187D874` | `resident-symbol-missing` | `schedule-divergence-at-site` |
| `overlay34CreateRecord` | `resident-symbol-missing` | `text-differs` 36 (4 out of range) |
| `overlay37RenderEffect` | `resident-symbol-missing` | `text-differs` 138 |
| `func_overlay_041_F0000854_1887B8C` | `resident-symbol-missing` | `rom-size` |
| `func_overlay_046_F0000120_188E518` | `resident-symbol-missing` | `rom-size` |
| `func_overlay_071_F0000278_18C9D98` | `resident-symbol-missing` | `text-differs` 28 |
| `overlay94UpdateController` | `resident-symbol-missing` | `text-differs` 13 |

`overlay11UpdateMenu` owns Overlay 11 `+0x1398..+0x184C` with no padding and
exactly 102 runtime records: table-1 134..193 are 60 SYMBOL records; table-2
172..213 contain 36 LOCAL address records, five LOCAL JUMPs, and one same-
overlay SYMBOL. Its 76 address records cover resident/BSS identities `D_0`,
`D_cfgA/B/C`, `D_flags`, `D_count`, `D_table`, `D_paramA/B/C/D`, `D_modeFlag`
and local `D_1B8/D_1BC/D_1C4/D_1CC/D_204/D_menuBase+0x1C4`; all candidate
offsets/types agree. The 26 calls are `+0x08` to resident `func_80028F54`,
`+0x84/+0xA8/+0xF0/+0x100` to `func_80000F94`, `+0x13C` to Overlay 45
`+0x1BF4`, `+0x160` to `func_8002554C`, `+0x1B8` to Overlay 66 `+0`,
`+0x1C0/+0x1C8/+0x1D8` to `func_800290AC/func_800291D8/func_800006BC`,
`+0x1E0` to Overlay 11 SYMBOL `+0x2BF4`, LOCAL JUMPs
`+0x208/+0x398` to `+0x1058`, `+0x220/+0x3B0` to `+0x1130`, and `+0x3E4`
to `+0x2948`, plus `+0x25C/+0x41C` to `func_80005820`,
`+0x328/+0x370/+0x46C/+0x490` to `func_80028374`, `+0x330/+0x474` to
`func_80028528`, and `+0x3DC` to `func_8003A754`. The function is unexported;
its sole inbound is table-2 LOCAL JUMP record 68 at module `+0x930` from
`func_overlay_011_F0000150_1868998+0x7E0`. Linked equality proves fallback
only; clean current source and restored zero carriers are uncompiled.

Nine now carry an in-range word count and a linked-ROM oracle. Four are
`schedule-divergence-at-site`, which is the honest answer and a codegen problem:
`func_overlay_029`'s two remaining refusals are `ext_o0_2a4c0` and
`ext_o0_6ec00`, each demanding two and three different addends from sites that
disagree. Two moved to `rom-size`, which is a different scaffolding question.

`relocation-truncated (R_MIPS_26)` also disappears: the one case in this set,
`func_overlay_066_F00004E0_18C6948`, is now `text-differs` at 181 words. The
class is closed by construction -- it existed only because a resident-owned
name was being assigned -- but only the candidates whose source names a
resident target were re-trialled here, so a full `--overlays-only` sweep is what
would confirm the other three.

Two side effects worth recording. `overlay34SortAndDraw`, which is *not* one of
the 15, went from `text-differs` 168 to `resident-call-unreadable` and back to
168 across the two refusal policies -- it is the measurement that settled §5.6's
note-versus-refusal question. And `func_overlay_057_*`'s divergence report now
names `func_800508B4_o057Reloc` rather than a bare placeholder, which is the
aliasing showing its working.

#### A stale log is a wrong class

Six of the candidates above stayed in `resident-symbol-missing` after the
surface had already valued them, because `promotion_trial.py` classified from
`log1 + log2`. The first pass links against the *stale* surface -- that is the
whole point of the two-pass integration (§5.5) -- so its `undefined reference`
lines survive into the concatenation and `UNDEF_RE` reported symbols the second
link resolved perfectly well. Markers still come from both passes, since a
POSTPROCESS marker is printed by the compile; every *link* diagnostic now comes
from the second pass alone.

## 6. What the trial measures now

`tools/promotion_trial.py --overlays-only`, all 279 overlay candidates, lane
`lane/reloc-synth2`:

The counts and per-function scores below are the historical linked,
relocation-masked spike result. They are not the later raw isolated ranking;
for example, both Overlay 1 angle scans show four schedule words here but five
raw positions when their unresolved local-data addend is included.

| class | before | after |
|---|---:|---:|
| `text-differs` -- links, N words differ in range | 110 | **150** |
| `text-size-differs` -- links, with an exact size delta | 0 | **44** |
| `build-error` | 169 | **85** |

Nothing came out `exact` or `text-exact`; every candidate that links has real
codegen work left, which is the point -- what changed is that 194 of 279 now
carry a number instead of a build failure.

The 150 that link, by in-range words:

| words | 1-2 | 3-4 | 5-8 | 9-16 | 17-32 | 33-64 | 65+ |
|---|---:|---:|---:|---:|---:|---:|---:|
| candidates | 7 | 11 | 13 | 30 | 22 | 27 | 40 |

The 44 size reports, by delta: 24 are shorter than the module owns and 19
longer (one guard reports a non-size digest); **18 are within ±16 bytes**, the
range where a codegen nudge is plausible. The extremes (`-472`, `+456`) are
candidates whose shape is wrong, not their scheduling.

The 85 remaining build errors, by cause -- each needs different work, which is
why they are named rather than pooled:

| cause | count | what it means |
|---|---:|---|
| `schedule-divergence-at-site` | 49 | the candidate's instructions differ *at* a placeholder's own sites, so no consistent addend exists. §4.1; the synthesizer reports the conflict rather than inventing a value |
| `resident-symbol-missing` | 15 | an undefined splat auto-name in the resident address space. Not a relocation-surface problem: the tree does not define that function yet |
| `rom-size` | 14 | the image's own size moved and no guard marker explains it |
| `relocation-truncated` | 4 | the reference does not fit its field at the synthesized value |
| `unresolved-placeholder` | 3 | undefined with none of the above |

`schedule-divergence-at-site` is now the dominant class, and it is the honest
one: it says the candidate does not yet agree with the target *where the
relocation table can see it*, which is a codegen problem, not a scaffolding
problem. The spike's alias-coupling and non-`.text` failure classes are gone --
`overlay1InterpolatePath` and `overlay1MeasureCurves` link now, because the
generator owns the alias block (§5.3).

### 6.1 The 31 candidates within 8 in-range words

    1  overlay80InitializeContact          o80    4  overlay1FindNextAngle              o1
    1  overlay97InitScale                  o97    4  overlay1FindPreviousAngle          o1
    2  overlay18Load                       o18    4  overlay20BuildTileCommands         o20
    2  overlay7DispatchSelection           o7     4  overlay3FindClosestObject          o3
    2  overlay84AdvanceCurrent             o84    4  overlay43FilterImage               o43
    2  overlay8ScaleOutputs                o8     4  overlay62Update                    o62
    2  overlay99RenderSegments             o99    4  overlay84LoadCurrent               o84
    3  overlay101DrawClock                 o101   6  func_overlay_022_F0000000_1878108  o22
    3  overlay1CloneRecord                 o1     6  func_overlay_041_F0001650_1888988  o41
    3  overlay40FadeRecords                o40    6  overlay68PromoteSecondary          o68
    4  func_overlay_014_F0000000_186F8D8   o14    6  overlay74Update                    o74
                                                  6  overlay7CommitSelection            o7
    7  func_overlay_022_F0000A7C_1878B84   o22    8  func_overlay_009_F0000540_1866BB8  o9
    7  func_overlay_038_F0000000_1885D10   o38    8  func_overlay_073_F0000000_18CAAC0  o73
    7  overlay14ResetMode                  o14    8  overlay1AssignRecordIndex          o1
                                                  8  overlay1ResolvePathPoint           o1
                                                  8  overlay20UpdateObjectResource      o20

`overlay84LoadCurrent`'s four-word row is historical. A bounded
annotated-target permutation produced exact 72-word C, promoted in
`306a1c31`. Its five runtime relocation roles, linked owned range, complete
Overlay 84 image, and preserved full ROM are byte-identical. It no longer
belongs in the `NON_MATCHING` queue.

`func_overlay_022_F0000000_1878108`'s six-word entry is that historical linked
trial. The surviving isolated object has five stack-home/store-order words but
omits the TU's required `-Wab,-r4300_mul`; fresh configured V0 must reconcile
the count and all 21 runtime relocation sites before either score is reused.

`overlay8ScaleOutputs`'s two-word row is historical; the function was promoted
in `07e04a0f`. Its two raw LO16 fields become exact under the four proved
runtime LOCAL tuples, and it no longer belongs in the NON_MATCHING queue.

`overlay1AssignRecordIndex` owns Overlay 1 `+0x36A0..+0x3750`, 44 words with
no padding. Its runtime contract has seven records: table-1 record 128 is a
SYMBOL call at function `+0x28` through ORT 257 to resident `GetRomlistInfo`;
table-2 LOCAL pairs 458/459, 460/461, and 462/463 at
`+0x44/+0x48`, `+0x70/+0x74`, and `+0x90/+0x94` all resolve through BSS base
`+0x83E0` plus addend `+0x1D8C` to module `+0xA16C` (`D_1D8C`). Retained
diagnostic C emits only five records and is 34/44 literal, 36/44 after runtime
normalization. Clean source now expresses all seven identities but is
uncompiled. ORT 1219 exports `+0x36A0`; resident relocation 61 at ROM `0xB894`
from `func_8000AA38+0x25C` is the sole inbound. Linked equality proves fallback
only.

Overlay 9 `+0x540`'s eight-word entry is likewise historical masked linked
evidence, not a retained configured candidate. Its surviving standalone object
omits the TU-required `-Wab,-r4300_mul` and has nine raw/eight normalized sites.
The exact runtime contract is ten records: three `LOCAL` HI16/LO16 pairs to
module `+0x18B0`, `SYMBOL` calls to resident `mathDiffAngle` and
`func_8002A8C0`, and one `LOCAL` HI16/LO16 pair to BSS `+0x1930`. The synthetic
target object collapses the zero-field calls to one local placeholder, so the
runtime table—not that object—is the callee-identity authority.

Overlay 1 `+0x7D6C`'s eight-word entry is the addend-normalized result; the
retained isolated ranking has ten raw sites. Only 19/22 candidate runtime
tuples agree: `D_218` LO16 and `D_1D88` HI16 exchange `+0x090/+0x094`, and
`D_1BA4` LO16 is at `+0x14C` rather than target `+0x148`. The first clear pair
resolves to existing local `D_220`; the former `D_220_Clear` name had no
linker/runtime identity.

Overlay 14 `+0x498` (`overlay14ResetMode`) owns 18 table-2 records. Four LOCAL
HI16/LO16 pairs at `+0x20/+0x2C`, `+0x1C/+0x30`, `+0x18/+0x34`, and
`+0x14/+0x38` resolve respectively to module data `+0x1D3C` (`D_FC`),
`+0x1D24` (`D_E4`), BSS `+0x1EF8` (`D_128`), and data `+0x1D2C` (`D_EC`).
The SYMBOL call at `+0x54` resolves through ORT 1652 to Overlay 14 `+0x1B54`
(`overlay14ReleaseOwner`). Four more LOCAL pairs at `+0x84/+0x88`,
`+0x8C/+0x90`, `+0x94/+0x98`, and `+0x9C/+0xA4` resolve to module data
`+0x1D38`, `+0x1D18`, `+0x1D1C`, and `+0x1D20`; the LOCAL JUMP at `+0xB0`
targets `overlay14MoveCommandCursor` at `+0x578`. The synthetic fallback target
object exposes only ten records, omitting four loader-owned pairs and collapsing
the zero-field call. Retained pre-current-alias C proves 17 identities; fresh C
proof of the repaired SYMBOL alias remains pending. The function has no padding
or export and has five local inbound JUMPs: table-2 records 226, 238, 241, 267,
and 270 from `overlay14AdvanceCommand` twice, `overlay14StepCommand`,
`overlay14DispatchCommand`, and `overlay14CallUpdate`.

Overlay 7 `+0x894` (`overlay7DispatchModes`) owns 23 text and seven switch-table
records. The text surface comprises LOCAL pairs to mode arrays at module
`+0x1264/+0x14BC`, a LOCAL pair to the switch table at `+0x18F4`, a SYMBOL
pair through ORT 1579 to resident BSS `D_800D3128`, two SYMBOL calls through
ORT 171 to resident `mathRnd`, nine local calls to `overlay7CreateEntry`, and
four local calls to `overlay7AppendEntry`. The seven `R_MIPS_32/LOCAL` data
records at module `+0x18F4..+0x190C` target case labels inside
`+0x894..+0xAA0`. ORT 1471 exports the function; five resident calls and
Overlay 25 table-1 record 15 are its six inbounds. Retained genuine C is
128/131 raw and 129/131 normalized, while linked equality is fallback-only.

Overlay 7 `+0xCCC` (`overlay7DispatchSelection`) owns 13 records: a SYMBOL
HI16/LO16 pair through ORT 1579 to resident `D_800D3128`; SYMBOL calls through
ORT 284 to `camGetMode` and ORT 1580 to Overlay 59 `overlay59AppendValue`;
LOCAL pairs to module `+0x1BA8/+0x100C/+0xFCC/+0xFEC`; and a LOCAL JUMP to
`overlay7CreateEntry` at `+0x228`. Retained genuine C has every offset/type and
is 58/60 after runtime normalization. ORT 1510 exports the function and
fourteen calls arrive from Overlays 1, 7, and 8; linked equality is fallback-only.

Overlay 7 `+0xDBC` (`overlay7CommitSelection`) owns 17 records: a SYMBOL pair
through ORT 1579 to resident `D_800D3128+2`; two calls to `mathRnd`; calls to
resident `func_800031E8` and `amSndPlay`; LOCAL pairs to module
`+0x1714/+0x18B4/+0xFC0/+0x1BA0/+0xFC4`; and a LOCAL JUMP at `+0xF4` to
`overlay7ReleaseEntry` at `+0x000`. Retained pre-identity-repair C is 69/72
runtime-normalized; current source expresses the corrected identities but is
uncompiled. ORT 1345 exports it and seventeen calls arrive from resident code
and Overlays 1, 7, 36, 86, 90, and 91. Linked equality is fallback-only.

`func_overlay_014_F0001830_1871108`'s ownership trial proves module growth and
the exact seven-entry switch payload only. Its retained isolated candidate used
one false callee for all eight calls; repaired six-identity, 21-record, linked C
proof is still required before its five normalized schedule sites can be reused.

Overlay 99 `+0x800` (`overlay99RenderSortedEntries`) has ten authoritative
runtime records: table-1 records 20..25 call at
`+0x09C/+0x1E4/+0x220/+0x258/+0x32C/+0x360` to
`camGetProjZ`, `camGetPtr`, `func_80022E80`, `func_8002AA50`,
`mtxf_transform_point`, and `func_80022FD4`, plus LOCAL HI16/LO16 pairs at
`+0x1F4/+0x21C` and `+0x260/+0x264` for module base `+0x1410` with addends
four and eight (table-2 records 49..52, transform-Z and intensity-scale
constants). Its assembled fallback target retains only the six generic calls,
so the runtime table is authoritative. The function is unexported; table-2
record 57 at `overlay99RenderSegments+0x1D4` is its sole inbound. Retained
218/233 C used false frame gaps; clean `CameraSprite + MtxF` source is uncompiled
and linked equality proves fallback only.

Resident `levelGetCounts` owns 37 records: calls to `func_8002B280` at
`+0x24/+0xC8/+0x280/+0x2E0`, `piRomLoad` at `+0x30/+0x290`,
`piRomLoadSection` at `+0x124/+0x310`, `mmFree` at
`+0x26C/+0x274/+0x3E0`, and `align4` at `+0x2D4/+0x2EC`, plus twelve
HI16/LO16 pairs. Retained C has every offset/type but binds the endpoint pair
`+0x44/+0x50` to `D_800CF3E0+0x40` instead of `D_800CF420`; source now names
the target identity and is uncompiled. ORT 518 has sole inbound Overlay 18
table-1 record 56 at `overlay18Initialize+0x8`. Linked equality is fallback-only.

Resident `func_80028FCC` owns three `R_MIPS_26` records to `func_80028FB8` at
`+0x14/+0x30/+0x4C`, all exact in retained 17/27 configured C. ORT 663 exports
the function, but exhaustive resident relocation, overlay SYMBOL, direct-JAL,
literal-pointer, object-reference, and source scans authenticate no inbound.
The staged shared-result source is uncompiled; linked equality is fallback-only.

Overlay 61 `+0x1648` has eleven authoritative records: resident calls at
`+0x14/+0x34/+0x4C/+0x6C/+0x8C/+0x140/+0x154` to `packOpen`,
`packOpenFile`, `packFileSize`, `func_8002B280`, `packReadFile`, `mmFree`, and
`packClose`; two calls at `+0xC0/+0xDC` to Overlay 68 `+0`; and a LOCAL
HI16/LO16 pair at `+0x24/+0x28` with stored addend `+0x164`. The current
guarded source names that pair `gOverlay61SavePathReloc`, but the linker-symbol
ledger still exposes only `D_164`; identity-correct C linkage remains pending.

Overlay 31 `+0x6B0` (`overlay31InitializeBuffers`) has a retained prior C
object with 54 static records: 16 calls and 19 HI16/LO16 pairs, all at the
target's opcode-compatible sites. The shipped tables authenticate 47 SYMBOL
records and seven local JUMPs. Data pairs resolve to `gOverlay31MaxLine`,
`gOverlay31MaxPoint`, both vertex buffers, triangle and rectangle sources,
configs, point pool, line records, dummy assets/count, and effect records/count.
Resident calls resolve to `reset_particles`, `func_8002B280`, `piRomLoad`,
`func_800355A0`, `func_80034448`, `func_8001F520`, and `mmFree`; seven local
calls cover the four config builders, pool, records, and reset helper. The
unannotated fallback target preserves only the 16 calls, so this runtime decode
is the identity authority. The current replay still needs a fresh exact C
object proving all 54 roles before its claimed 245/245 words can be promoted.

Overlay 68 `+0x1250` (`overlay68RebuildSecondaryEntry`) owns 19 runtime
records. Table 1 pairs 52/53, 60/61, and 63/64 at function
`+0x08/+0x1C`, `+0x1A4/+0x1B0`, and `+0x1BC/+0x1C0` resolve through reserved
DATA1 `+0x1498`/ORT 1850 to resident `D_8007A1F8`; 54/55 at `+0x74/+0x90`
resolve to `func_8002B280`; 56/59 at `+0xB8/+0x198` to `piRomLoadSection`;
57 at `+0xC0` to `func_800291C4`; 58 at `+0xCC` to `levelGetBlurEffect`; and
62/65 at `+0x1B4/+0x1CC` to `mmFree`. Table 2 LOCAL pairs 14/15 and 16/17 at
`+0x04/+0x0C` and `+0x34/+0x38` share data base `+0x15B0`, addend `+0x14`,
module identity `+0x15C4`; record 18 at `+0x68` calls local
`overlay68PayloadLimit` (`+0`). Retained diagnostic C emits all 19 sites, but
its friendly aliases split shared identities; clean source now unifies them and
awaits regenerated metadata plus compilation. ORT 1163's sole inbound is
resident relocation 4, `func_80004FE0+0x4C8`; there are no local or
cross-overlay callers. Current linked equality proves fallback only.

Overlay 41 `+0x000` owns exactly three runtime records: a SYMBOL HI16/LO16
pair at function `+0x14/+0x28` resolving to `D_800D6B58`, and a SYMBOL call at
`+0xD0` resolving to resident `func_8000D16C`. The retained prior C has these
offsets/types, while the assembled fallback target retains only the call. The
current guarded decrement-reorder replay has no surviving C object, so its
three static identities and claimed 73/73 words still require fresh proof.

Overlay 41 `+0x1650` (`func_overlay_041_F0001650_1888988`) owns four runtime
records: SYMBOL HI16/LO16 at `+0x08/+0x0C` resolving to resident
`D_800D6C58` (`gOverlay41Slots`), and LOCAL HI16/LO16 at `+0x98/+0xA0`
resolving through initialized-data base `+0x1DE0` plus `+0x54` to module
`+0x1E34` (`D_0[0x15]`). Candidate C places the LOCAL LO16 at `+0xA4`; target
uses `+0xA0`. ORT 1461 exports the function; resident relocations 325 and 326
call it twice from `func_800517E0`. Fallback equality does not prove C.

Overlay 21 `+0x10C` (`overlay21ApplyPriorities`) owns nine runtime records: a
SYMBOL call at `+0x1C` to resident `camGetPtr`; LOCAL pairs at `+0x24/+0x28`
for object count and `+0x30/+0x40` for the object array; and count-reload pairs
at `+0x114/+0x11C` and `+0x19C/+0x1A0`. The retained exact-looking objects
expose only the fallback's three generic records and depend on extracted
assembly. A fresh C object must prove all nine tuples before the dirty
scoreboard's 456-byte credit is accepted.

Overlay 45 `+0x764` owns 24 runtime records: 13 external calls, one local call
to `+0x1158`, two SYMBOL HI16/LO16 pairs for the resource head and resident
scissor state, and three LOCAL pairs for retained constants. The historical p4
body collapses six call roles, including two distinct matrix identities, onto
one offset-zero carrier. Its apparent equality also rewrote the three constant
LO16 instruction fields at `+0x160/+0x168/+0x174`, which ADR 0002 prohibits.
Fresh C must preserve all 24 semantic identities and emit those addends
naturally from the already-owned initialized data.

Overlay 34 `+0x0` (`overlay34InitStorage`) owns eight runtime records: SYMBOL
JUMPs at `+0x28/+0x74` to resident ORT 82 (`func_8002B280`), then LOCAL
HI16/LO16 pairs at `+0x34/+0x38`, `+0x80/+0x84`, and `+0xB8/+0xC4` for module
data `+0`, `+4`, and `+8` (`gOverlay34Records`, `gOverlay34Pointers`, and
`gOverlay34Count`). Its standalone target object retains only four static
records and bakes the final `+8` addend; runtime normalization is authoritative.

Overlay 73 `+0x0` (`func_overlay_073_F0000000_18CAAC0`) owns eight LOCAL
records: `+0x1C/+0x3C` resolves through base `+0xEB0`, addend `+0x80`, to
module `+0xF30`; pairs at `+0x140/+0x144`, `+0x14C/+0x160`, and
`+0x164/+0x170` resolve through base `+0xF80` with addends `0/+4/+8`.
The fallback target statically retains only the first pair, so runtime tables
authenticate the other six. ORT 1248 exports `+0`; resident relocation 90 at
`func_8000AA38+0x42C` is the sole inbound.

Overlay 80 `+0x11C` (`overlay80UpdateContact`) owns 20 records. SYMBOL calls
at function `+0x30/+0xFC/+0x240/+0x294` resolve through ORTs 371, 101, 381,
and 967 to resident `func_8005776C`, `sqrtf`, `func_8005AD64`, and
`func_8005ABA8`. Eight LOCAL HI16/LO16 pairs resolve through data base
`+0x3F0` with addends `+4,+8,+0xC,+0x10,+0x14,+0x18,+0x1C,+0x20`. ORT 1290
exports `+0x11C`; resident relocation 132 at `func_8000AEEC+0x3B0` is the sole
inbound. The retained genuine-C baseline has the expected topology but is
142/180 words; canonical linked equality is fallback-only. A later exact
source claim has no retained winning object and must independently reproduce
all 20 tuples before receiving credit.

Resident `func_8005A948` owns 13 static records: HI16/LO16 pairs to
`D_800D7D04` at `+0x00/+0x04`, `D_800D7CF4` at `+0x34/+0x38` and
`+0x134/+0x13C`, and `D_800D7CF8` at `+0xB4/+0xB8` and `+0xD0/+0xD4`, plus
calls to `piRomLoadSection` at `+0xC8/+0x124` and `func_8002B314` at `+0xF8`.
It has no export-table entry or overlay runtime record; `func_8005A7A0+0x104`
is its sole caller. Retained isolated C agrees on all tuples but is 83/94 words
and omitted the TU's canonical `-Wo,-loopunroll,0`; linked equality is fallback-only.

Overlay 62 `+0xD4` (`overlay62Update`) owns 71 runtime records: 43 SYMBOL and
28 LOCAL, comprising 21 calls and 25 HI16/LO16 pairs. The genuine C object has
all 71 at exact offsets/types; the fallback target retains only 29 incomplete
generic records. ORT 1444 exports `+0xD4`; resident relocation 302 at
`func_80038E1C+0x3A4` is the sole inbound. Runtime tables, not friendly proxy
names or fallback ELF records, are the identity authority.

Overlay 7 `+0xEDC` (`overlay7FillValues`) is an exact 11-word C island inside a
mixed TU. Its only records are LOCAL HI16/LO16 at `+0x00/+0x04`, resolving
through base `+0x1910` plus addend `+0x2AA` to BSS `gOverlay7ValuesEnd`.
ORT 1517 exports it; Overlay 1 table-1 relocation 167 calls it from
`overlay1UpdateObjectPhysics+0x8C0`. Linked range/module/ROM equality is
C-produced; its two inert source aids are tracked in the cleanup queue.

Overlay 19 `+0xD78` (`overlay19ClassifyEdge`) owns no static or runtime
relocations. It is unexported. Its sole inbound is table-2 JUMP index 6 at
module `+0xCF4`, from `overlay19FindAdjacent+0xD8`; linked equality currently
comes from the fallback, not the retained 110/120 C producer.

Overlay 1 `+0x63CC` (`overlay1UpdateCountdown`) is exact C after masking seven
records: LOCAL pairs at `+0x00/+0x04`, `+0x10/+0x14`, and `+0x30/+0x38` for
BSS addends `+0x1DA0/+0x1D94/+0x1D9C`, plus a SYMBOL call at `+0x34` to
Overlay 1 ORT 1525 (`+0x5BA4`). ORT 1535 exports it; table-2 index 954 stores
its sole inbound function pointer at module data `+0x821C`. Its legacy inert
condition is cleanup-tracked and does not authorize guards in unmatched C.

Resident `rcpClearZBuffer` owns an exact HI16/LO16 pair at `+0x00/+0x04` to
resident BSS `D_800D2FAC` and is exported as ORT 765. Three shipped SYMBOL
records call it: Overlay 60 module `+0x2194` from
`func_overlay_060_F0000334_18BA10C+0x1E60`, Overlay 91 module `+0x518` from
`overlay91Render+0x5C`, and Overlay 99 module `+0xC8C` from
`overlay99RenderSegments+0xE8`. Its fourth direct caller is resident
`rcpClearScreen+0x48`; no other shipped overlay record targets ORT 765.

Resident `func_8003A2C8` owns exact HI16/LO16 pairs at `+0x00/+0x04` to
`D_8007C090` and at `+0x24/+0x28` plus `+0x3C/+0x40` to `D_800D3128`. ORT
606 exports it. Its direct callers are resident `func_80027EC0+0x80`, Overlay
46 table-1 record 60 at module `+0x684` (`overlay46ReleaseState+0x70`), and
Overlay 60 table-1 record 206 at module `+0x156C`
(`func_overlay_060_F0000334_18BA10C+0x1238`); no other shipped overlay record
targets ORT 606. Linked equality currently proves the fallback only.

Overlay 1 `+0x7B64` (`overlay1FindBestRecord`) owns four LOCAL records:
table-2 884/885 at `+0x00/+0x08` resolve through BSS base `+0x83E0` and addend
`+0x220` to module `+0x8600` (`gOverlay1BestRecords`/`D_220`), while 886/887
at `+0x04/+0x14` use addend `+0x1D88` to resolve to module `+0xA168`
(`gOverlay1SelectedType`/`D_1D88`). The assembled fallback literalizes the
second pair, so the runtime table is authoritative. The function is unexported;
its only callers are `overlay1CreateRecord+0x30` and `+0x144`, table-2 LOCAL
JUMP records 889 and 895 at module `+0x7C0C/+0x7D20`. Promotion requires
metadata-only rebinding for both C identities; current linked equality proves
fallback only.

Overlay 1 `+0x378` (`overlay1FindType5ByKey`) owns table-1 SYMBOL record 1 at
function `+0x14`/module `+0x38C`, resolving through ORT 128 to resident
`func_8000572C`. Source now calls that identity directly and metadata rebinds it
to the Overlay 1 zero-addend placeholder. ORT 1485 exports `+0x378`; its sole
inbound is table-2 SYMBOL record 548 at module `+0x4018`, from
`overlay1TransitionState+0x40`. There are no resident, cross-overlay, or local
JUMP inbounds. Retained pre-HEAD/current-body C is 22/39 with the outbound site
at the exact offset/type; current linked equality proves fallback only.

Resident `func_80020D8C` has no static relocations and is exported as ORT 374.
Eight shipped SYMBOL calls target it: Overlay 57 table-1 record 258 at module
`+0x3288` (`overlay57ApplyValue+0x50`), Overlay 60 record 337 at `+0x2268`
(`func_overlay_060_F0000334_18BA10C+0x1F34`), and Overlay 82 records 12..17
at `+0x310/+0x320/+0x368/+0x38C/+0x3BC/+0x3DC` from
`overlay82Update+0x2D0/+0x2E0/+0x328/+0x34C/+0x37C/+0x39C`. Resident
`func_8001BB10+0x60` is the ninth call; it passes an additional owner/context
argument in `a3` that the callee overwrites without reading. No other shipped
overlay record targets ORT 374; linked equality currently proves fallback only.

Resident `func_8002CF6C` owns 11 exact records: R_MIPS_26 calls at `+0x0C` to
`joyMessageQ`, `+0x18` to `func_80070170`, `+0x28` to `func_8002B280`,
`+0x54` to `func_8002CCE4`, `+0x60` to `packCalculateGameChecksum`, `+0xC4`
to `packCalculateGlobalFlagsChecksum`, `+0x120` to `mainResetPressed`, `+0x13C`
to `func_8002C8B4`, and `+0x144` to `mmFree`, plus a HI16/LO16 pair at
`+0x80/+0x90` to `D_8007A304`. ORT 505 exports it; `joyRead+0x130` is the
sole authenticated caller, with no overlay SYMBOL, resident runtime-table, or
absolute-pointer inbound. Current linked equality proves fallback only.

Resident `debug_text_width` owns five exact records: R_MIPS_26 calls at `+0x18`
and `+0x30` to `sprintfSetSpacingCodes`, `+0x28` to `vsprintf`, and a
HI16/LO16 pair at `+0x4C/+0x50` to `D_8007CE98`. ORT 862 exports it, but all
375 resident relocation entries, every shipped overlay SYMBOL record, direct
JAL and literal-pointer scans, and source references are empty. Current linked
equality proves fallback only.

Resident `func_80045BBC` owns 24 static records: HI16/LO16 pairs for
`D_8007CFE8` at `+0x04/+0x08`, `D_80705014` at `+0x18/+0x20`,
`D_8007CFE0` at `+0x1C/+0x24`, `D_80705018` at `+0x28/+0x30`,
`D_8007CFE4` at `+0x2C/+0x34`, `D_8070501C` at `+0x38/+0x40`,
`D_800D5D40` at `+0x78/+0x8C`, `D_800D5D48` at `+0x7C/+0x80`,
`D_80083A80` at `+0xB4/+0xC8`, and `D_80083A88` at `+0xB8/+0xC4`;
R_MIPS_26 `_bcopy` calls at `+0x4C,+0x6C,+0x88`; and `packWriteFile` at
`+0xD4`. Genuine C emits 18 exact tuples but literalizes the three
`D_80705014/18/1C` pairs. Resident runtime records, ORT export rows at offset
`0x4576C`, overlay SYMBOL inbounds, and stored-pointer inbounds are all zero;
`func_80045CAC+0x64` is the sole direct caller. Current linked equality proves
fallback only.

Resident `func_8004BA8C` owns nine exact static records in retained genuine C:
HI16/LO16 pairs to `D_800D60E4` at `+0x04/+0x08`, `D_800D6628` at
`+0x14/+0x28`, and `D_800D6644` at `+0x34/+0x38` and `+0x48/+0x54`, plus
an R_MIPS_26 call to `func_8004D39C` at `+0x40`. ORT 880 exports resident
offset `0x4B63C`; resident callers are `func_8004B1DC+0x1E4/+0x294` and
`func_8004BCC4+0x12C`, while Overlay 41 table-1 record 88 at module `+0x1CD0`
and Overlay 45 record 22 at `+0x3B0` call it from `overlay41DrawItem+0x4C`
and `overlay45ConfigureLayout+0x9C`. No additional direct JAL, resident
runtime-table, overlay SYMBOL, or stored-pointer inbound is authenticated.
Current linked equality proves fallback only; the two overlay proxy names still
need metadata-only rebinding to this shared identity when clean V0 is compiled.

Resident `func_8002B7AC` targets 12 static records: HI16/LO16 pairs to
`D_800D21B0` at `+0x08/+0x0C`, `D_800D21A8` at `+0x44/+0x48`,
`D_800D20A8` at `+0x50/+0x5C` and `+0x60/+0x6C`, and `D_800D1CA8` at
`+0x64/+0x68`, plus R_MIPS_26 calls to `ReleaseUnusedLinkSlots` at `+0x3C`
and `func_8002B8A8` at `+0x8C`. Genuine C carries the same identities, but
eleven sites are four bytes early and the first `D_800D20A8` LO16 is twelve
bytes early because the function is one instruction short. ORT 593 exports
resident offset `0x2B35C`; resident runtime and overlay SYMBOL inbounds are
zero, and `func_80026FB4+0x5F8` is the sole direct caller. Current linked
equality proves fallback only.

Resident `func_80047CD8` owns four exact R_MIPS_26 records to
`func_800349A4` at `+0x19C,+0x1F0,+0x244,+0x278`. It has no resident runtime
record or ORT export. Authenticated inbounds are resident
`func_80009414+0x520`, Overlay 69 table-1 record 9 at
`overlay69DrawSortedGeometry+0x530`, and Overlay 88 table-1 record 12 at
`overlay88DrawSortedGeometry+0x530`; the overlay proxy names now disclose the
shared draw-cone identity and await regenerated metadata. Current linked
equality proves fallback only.

Resident `func_80041CE4` owns nine exact static records in retained genuine C:
HI16/LO16 pairs to `D_8007C894` at `+0x04/+0x08`, `D_8007C88C` at
`+0x48/+0x4C` and `+0x1E4/+0x1F0`, and `D_7C900` at `+0x9C/+0xA4`, plus an
R_MIPS_26 call to `func_800349A4` at `+0x158`. Resident runtime records, an
ORT export at offset `0x41894`, overlay SYMBOL inbounds, and stored-pointer
inbounds are all zero. `partDraw+0xEC` is the sole direct caller. Current
linked equality proves fallback only.

Resident `func_80019DE8` targets three static records: R_MIPS_26
`mathOneFloatRPY` at `+0xBC` and a `D_800CB290` HI16/LO16 pair at
`+0xC8/+0xE0`. Genuine C has the same types and identities at
`+0xC0,+0xCC,+0xE4`, all four bytes late. Resident runtime records inside the
function are zero. ORT 358 exports offset `0x19998`, but all resident runtime
and overlay SYMBOL records have zero inbounds; direct callers are
`lightDefaultObjectLight+0x38` and `func_8001A008+0x74/+0xC4`, with no stored
pointer. Current linked equality proves fallback only.

Resident `func_8002B524` owns 12 exact tuples in retained diagnostic C:
HI16/LO16 pairs to `D_8007A278` at `+0x04/+0x08`, `D_8007A270` at
`+0x1C/+0x3C`, `D_8007A27C` at `+0x4C/+0x50`, and `D_800D1C60` at
`+0x90/+0x94`; R_MIPS_26 `runlinkGetAddressInfo` at `+0x74`; and
`func_8002BB40` at `+0x134,+0x160,+0x180`. ORT 547 exports offset `0x2B0D4`.
Resident direct callers are `runlinkSuspendCode+0xB4`,
`runlinkResumeCode+0xD8`, and `func_80034448+0x12C/+0x1D0`; five overlay
SYMBOL calls occur at Overlay 2 `+0x11A8`, Overlay 18 `+0x320/+0x334`,
Overlay 19 `+0x1B4`, and Overlay 35 `+0x5C4`. ROM `0x7AE40` carries one
R_MIPS_32 function pointer consumed by `RevealReturnAddresses`; no resident
runtime-table record targets ORT 547. Current linked equality proves fallback
only, and overlay proxy names await identity-explicit metadata regeneration.

Resident `func_8002BB40` owns eight exact tuples in retained diagnostic C:
HI16/LO16 pairs to `D_8007A270` at `+0x08/+0x64`, `D_800D21B0` at
`+0x14/+0x18`, and `D_800D1C60` at `+0x28/+0x2C` and `+0x40/+0x44`.
The C and assembled-target tuple sets agree although serialized ELF row order
differs, which promotion must explicitly recheck against the relocation gate.
Resident runtime records, an ORT export at offset `0x2B6F0`, overlay SYMBOL
inbounds, and stored pointers are zero. Direct callers are
`func_8002B3A8+0xE0` and `func_8002B524+0x134/+0x160/+0x180`. Current linked
equality proves fallback only.

Overlay 40 `+0x690` (`overlay40FadeRecords`) owns five SYMBOL HI16/LO16 pairs:
timer at `+0x00/+0x04`, current at `+0x0C/+0x10`, target at `+0x38/+0x3C`,
duration at `+0x44/+0x50`, and output at `+0x9C/+0xA0`. They resolve to
`D_800D6C4C`, `D_800D6C52`, `D_800D6C50`, `D_800D6C4E`, and `D_800D6C54`.
The retained current-layout C object emits all ten roles; the fallback target
retains none statically, making the runtime table the identity authority.

Eleven of these were not measurable before this lane. They are the sweep's next
targets: within eight words is the range where the permuter closes candidates.

## 7. What is still hand-written

- **Section externalization.** `externalize_elf_section.py` takes the expected
  payload as a hex literal in the Makefile, which is the one part of the
  machinery not derivable from addresses alone. Five rules use it.
- **The `POSTPROCESS` trim sizes themselves.** They are the ownership row's
  extent and could be emitted from the atlas rather than written out per file;
  this lane made the *failure* derivable, not yet the rule.
- **Relocation filters and instruction normalizations.** Genuinely per-function
  reviewed assertions; nothing here suggests they are mechanical.
- **A lone `R_MIPS_LO16`** still determines only the low half of its symbol's
  value (§3.2). Byte-identical output, non-canonical symbol value.

## 8. Cleanroom note

`tools/reloc_surface.py` reads the baserom and `config/overlays.us.json` at run
time and emits only symbol names and the addresses/values the link already
requires. It writes no extracted data, embeds no ROM bytes, and prints no
instruction text. This document quotes no ROM words.

The generated `overlay_undefined_syms.us.txt` is tracked, and is the same class
of content as the hand-maintained file it replaces: symbol names and the
relocation addends the link requires, in the same two line forms. It is
smaller than what it replaced (2,265 lines against 2,928) and carries no
instruction text, so the clean-room detectors see strictly less than before.
