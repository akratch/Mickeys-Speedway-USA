# Matching triage: which tool each walled function needs

A living routing log for the hard tail. Every remaining unmatched function is
walled by a *specific* mechanism, and each mechanism has a *right tool*.
Sending a function to the wrong tool burns effort (a permuter cannot fix a
structural gap; hand analysis cannot out-search the register allocator). This
document records, per function, the wall class and the tool — so work is routed,
not guessed.

Scores are the workbench `diagnose` differing-word count at the time of the
note; they are point-in-time, not tracked numbers. Recompute before acting.

## Wall classes and the tool each needs

| Class | Mechanism | Right tool | Why |
|---|---|---|---|
| **P — permuter-tractable** | register / colour / schedule *allocation permutation* (opcode+frame already exact; only *which* register/slot differs) | **decomp-permuter** (`tools/permute.sh`, capped `-j4` niced) | The winning source spelling exists but is not derivable by reasoning; brute-force search over spellings finds it. |
| **P! — permuter-stuck** | same as P, but the 20-min cap ends above 0 | **permuter, more budget** (re-seed, longer cap, or a manual scratch variant) then escalate to F if it plateaus | Close but the search space is large; more iterations or a better base often closes it. |
| **I — import-blocked** | the candidate does not cleanly isolate (`Syntax error in base.c` on import) or does not compile under `-DNON_MATCHING` in-TU | **fix the candidate first** (manual scratch / `--preserve-macros`), or route to **F** (trace-lever needs no permuter import) | The permuter never actually searches these until the base compiles standalone. |
| **F — Fable + workbench** | a *specific* compiler decision with a known *source lever* (ugen ring phantom-pop, float-invariant load form, declaration-order stack home, call-arg colour affinity) | **Fable subagent + Tier-2 traces** (`docs/breakthrough-campaign.md`, workbench `diagnose`/ring traces) | The lever is identifiable from a trace and applied by a targeted edit; the permuter would not stumble onto it efficiently. |
| **S — structural RE** | genuinely wrong shape: missing/extra code, wrong control flow or types | **Fable/Opus reverse-engineering + reference donors** (`mips_to_c`, JFG/BK/etc. with PROVENANCE) | Not an allocation problem at all; needs real decompilation. |
| **W — hard wall** | pure list-scheduler slot-fill (a *ready* zero-dependency op lands in a fixed latency bubble) or interference-forbidden colour with no lever | **permuter (last resort); else record as toolchain-heuristic wall** | No C source steers the decision; only a rescheduler/permuter might, and some are genuinely unreachable through the project toolchain as source. |

## Per-function log (resident register-only / schedule-only tail)

Updated as the permuter campaign and analysis proceed.

> **2026-08-27 root-cause correction.** The first sweep classed 8/12 targets as
> **I (import-blocked)** on a `Syntax error in base.c`. That was **not** the
> candidates — it was a bug in `tools/permute.sh`: its `-mips2` flag correction
> ran `gmake -n <obj>`, which prints nothing when the object is already built,
> so the scratch silently stayed at the importer's default `-mips1` and the
> search explored the wrong instruction space. Fixed (touch the source first;
> warn instead of failing silently). Those functions are really **P-class** and
> are being re-searched at the correct ISA. Lesson: a permuter that "finds
> nothing instantly" is almost always a setup/flag fault, not a hard function.

| Function | TU | Class | Evidence / note | Status |
|---|---|---|---|---|
| `func_8002C94C` | saves | **P** | globalcolor s5/s6 inversion; permuter base 40 → **0** | ✅ matched (permuter) |
| `func_8003A2C8` | menu | **P!** | historical configured records (object no longer retained) are 5/32 with six exact relocations; the surviving isolated object omits menu's no-unroll flag. Forced v1/v0 plus shared `modeBits` reaches diagnostic 1/32, but the last memory-first comparison carrier is ring-only and not color-reachable | one unchanged configured V0; park absent a new allocator mechanism |
| `func_8002CF6C` | saves | **F/P!/W** | retained configured full-TU and isolated C agree at 79/88 words, frame `0x48`, all 11 relocations exact; `savedFlag` is colored `a3` versus target FIFO `t1`. Source, volatile-field, flag-lattice, phantom-pop, and bounded-permuter routes are exhausted | one unchanged configured V0; then park absent a new allocator mechanism |
| `func_80020D8C` | models | **P/W** | full-TU and isolated C agree at 35/48 words, frame `-0x8`, no relocations, with a two-register temp-lane phase shift from `+0x38`; the sole caller passes four arguments while only three are consumed | one unchanged configured V0; then park absent a new UGEN temp-class mechanism |
| `func_8001A154` | lights | **P** (was mis-called **W**) | permuter cracked it: drop the redundant `& 0xFFFFU` blue mask (phantom ring pop) + fold `flare.scaledSize`. The 'no source lever' verdict was a FALSE FLOOR. | ✅ matched (permuter) |
| `mainUpdateZBCheck` | main | **P** (was **I** via -mips1 flag bug) | permuter cracked it once flags fixed: compare `(u16*)row+columns` directly, no cached pixel. | ✅ matched (permuter) |
| `func_80028FCC` | main | **F/W** | configured full-TU and retained isolated objects agree at 17/27 words/frame `0x18`; target materializes three boolean temporaries and a shared epilogue while thirteen structural/ABI forms and all 119 flag groups remain flat | one unchanged configured full-TU/linked V0, then park |
| `func_overlay_009_F0000540_1866BB8` | o9 mixed TU | **P!/W** | retained standalone evidence omitted required `-Wab,-r4300_mul`; 121/129 normalized words/frame `0x58` agree, with a four-way saved-FPR color cycle and ten runtime records | one identity-correct configured full-TU/linked V0, then park if reproduced |
| `func_8005A7A0` | models_5B300 | **F/W** | retained configured full-TU candidate is exact-sized at 106 words but has frame `0x50` versus target `0x38`, ten frame/carrier/schedule sites, and shifts the second loader call `+0xBC` → `+0xC0` | one unchanged configured full-TU/linked V0, then park |
| `overlay59PrepareEntry` | o59 standalone | **P!/W** | retained configured evidence is 53/62 normalized words/frame `0x28`; target loads descriptor values through `v0` and moves them to `a0` in call delay slots, while all natural source forms load `a0` directly | one identity-correct configured/linked V0, then park if reproduced |
| `overlay19ClassifyEdge` | o19 standalone | **P!/W** | retained configured evidence is 110/120 words, frameless and relocation-free; six `t3/v1` allocation sites plus four final load-order sites survived source, trace, forced-color, and bounded-permuter probes | run the one missing 119-flag lattice, then park if canonical flags remain best |
| `overlay1ResolvePathPoint` | o1 end TU | **P!** | retained evidence is exact-sized at 152 words with ten raw/eight normalized schedule/register sites and 19/22 runtime tuples; source had an invented clear alias and a nine-argument declaration for the observed seven-argument ABI | ABI/identity-correct V0, then one bounded annotated-target permutation if reproduced |
| `overlay14ResetMode` | o14 standalone | **F/W** | retained evidence is 56 words/frame `0x30`, with 11 raw/seven normalized prologue sites; the extracted target object omits eight loader-owned records and collapsed the `+0x54` SYMBOL callee to the wrong local placeholder | identity-correct V0, full flag lattice, then at most three `s3`-lifetime probes |
| `overlay1FindBestRecord` | o1 tail TU | **P!** | retained configured full-TU and isolated C agree at 18/30 words, frame zero, with one 12-site runtime-normalized `a1/a3` carrier exchange from `+0x04`; the candidate has all four runtime tuples, while linked range/module/ROM identity proves assembly fallback only | V0 plus three declaration/scope forms; stop if all are flat, otherwise combine strict improvements only within a six-run cap |
| `rcpClearZBuffer` | rcpFast3d | **F/W** | retained isolated evidence has 11 positional sites but only four schedule-aligned register fields at `+0x74..+0x80`; full flag/source/permuter routes are exhausted and the historical full-TU four-site object is absent | reprove V0, then one UGEN FIFO/web trace and at most two indicated forms |
| `overlay62Update` | o62 standalone | **P!** | retained configured evidence is 294 words/frame `0x88` with five raw/four normalized allocation sites; all 71 runtime tuples are exact and the isolated 11-word ranking has a macro-fidelity defect | V0, missing flag lattice, then one 20-minute macro-faithful annotated batch |
| `func_80047CD8` | fx | **P!** | the real `NON_MATCHING` TU now compiles: 226/234 exact rows, 8 register words from `+0x298`, identical temp lane, pool slot 28, and no copy site; corrected MIPS2/stack-aware two-worker search ran 205→60 at the four-minute cap, no zero | p10 allocation plateau |
| `func_80045BBC` | diCpu | **F/W** | 60 words/frame `0x30`; eight raw/two value-normalized sites, target 24 versus candidate 18 static relocations, with a final `t6/t4` FIFO web | unchanged full-TU/linked reproof, then park absent a symbolic-address/FIFO mechanism |
| `func_overlay_014_F0001830_1871108` | o14 standalone | **F/W** | retained 196/201 normalized words and 21-record/exact-table shape, but all eight calls used one false callee identity; five case-7 load/increment schedule sites remain | ABI-correct V0, then one target-shaped positive-index call rung; park if flat |
| `levelGetCounts` | level | **F/W** | retained full-TU 256/259 normalized words and 37 records; one `a0/v0` web plus target `D_800CF420` versus candidate `D_800CF3E0+0x40` endpoint identity | one unchanged full-TU/linked reproof, then park |
| `func_8002B524` | memory | **F** | retained 102/116 words/frame `0x58`, 12 exact relocations; slot-record/data carrier, normal-versus-likely lower guard, and `sp+0x38` versus target `sp+0x3C` call-live home | five-build cache/guard/combination/spill ladder, then park if flat |
| `func_overlay_073_F0000000_18CAAC0` | o73 standalone | **P** | retained 90/100 raw and 92/100 relocation-normalized words/frame `0x8`; eight `a2/a3` allocator sites. `D_80` uses LOCAL base `+0xEB0`/addend `+0x80`; `D_0/D_4/D_8` use base `+0xF80` with `0/+4/+8` | annotated V0, one forced-color diagnostic, then at most two trace-led forms and one bounded batch |
| `func_8005A948` | models_5B300 | **F/W** | surviving configured evidence is the earlier 11-word state; the later raw-array p10 result has historical three-site `t7`/target-`t8` evidence but no retained configured/linked artifact, and broad source/flag/permuter routes are exhausted | reprove p10, then one ugen FIFO trace and at most its indicated address-producer form |
| `func_80055B24` | anim | **P** | typed `void` callback alias removes the generic trap placeholder's phantom return web; metadata-only rebind restores the exact call identity | ✅ matched (ABI reconstruction) |
| `func_8002C69C` | saves | **P!** | coherent cursor re-cache reduced 18→11 register-only words at exact 28-word size; first `+0x10`, four allocator webs; corrected MIPS2/stack-aware reseed reached cost 55 but no zero | plateau; F-route web-existence lever next |
| `func_80041CE4` | particles | **P!** | recorded configured full-TU result is 27/153 register-only, but surviving isolated import omitted required `-Wab,-r4300_mul`; ten source-faithful probes and bounded search were flat | one configured reconciliation V0, then park if reproduced |
| `func_8002BB40` | memory | **F** | retained configured full-TU C is 72/72 words with all eight HI/LO relocations; stale standalone import falsely reports 30 register words | exact reproof/promotion queued |
| `func_800320F0` (`runlinkEnsureJumpIsValid` donor alias) | runlink | **P** | retained canonical object has 101 words/frame `0x20` and 21 relocations; linked ROM `0x32CF0..0x32E84` is byte-identical | ✅ matched; remove from living unmatched queues |
| `levelInit` | level | **F/W** | retained configured full-TU C is 394/516 words/frame `0x80`, first `+0x238`, with all 110 relocation tuples exact; ten source families, 119 flags, and bounded permutation are exhausted | one unchanged configured/linked V0, then park absent new allocator evidence |

### Already resolved by the Fable/workbench (F) route — pattern reference
`func_8003A520` (ugen FIFO phantom-pop), `func_800508D4` (float-invariant load
form), `func_80055970` (declaration-order stack home), `func_80038750` (call-arg
colour affinity). These are the signature of class **F** — see
`docs/breakthrough-campaign.md`.

### Structural / donor (S) route — reference
SDK/libultra donors (`__osSpSetPc`, `__osContRamRead/Write`, `__osBlockSum`,
`__osPfsReleasePages`, `osPfsDeleteFile`, `joyInit`) matched via BK/JFG donors.
`func_8004B1DC` (font, 509w) is an open S target (largely-wrong body, multi-hour RE).

## Routing rule of thumb
1. Frame-exact + small word-diff + register/colour/schedule → **P** (permuter first — cheapest, and it has cracked cases hand analysis called unlevered).
2. Permuter import fails → **I**: fix the base or hand it to **F**.
3. Permuter improves then plateaus → **P!** then **F** if a trace shows a real lever.
4. Wrong shape / missing code → **S**.
5. Proven no-lever scheduler slot-fill → **W** (permuter last resort; else record the wall).

## Structural survey findings (2026-08-27, Opus S-class agent)

Most "structural" resident functions turned out to be **P (permuter)** once the
shape was understood — another false-floor cluster. Newly routed:

| Function | TU | Class | Note |
|---|---|---|---|
| `func_8004B1DC` | font | **P** | Frame-exact (0x80). The display-list `dList++` idiom batches base+displacement stores; target materialises a fresh pointer per command. m2c's distinct-next-pointer form defeats the batching but spills to a 9th saved reg — net-neutral, so keep the frame-exact base and **permute the rolling-pointer register allocation**. Micro-facts recorded in-source: window-0 y2 is standalone `D_800D64F2`; font ptr is `window->font`. |
| `func_80046AA8` | diCpu | **P** | −3 insns loop-guard, but residual dominated by v0/v1/a0 register swaps in the packed-glyph blit. |
| `func_8002B7AC` | memory | **W — parked** | Candidate is 62 instructions (`0xF8`) versus the 63-instruction (`0xFC`) target, with 62 positional differences from `+0x4`. The same 12 relocation type/symbol identities are all four bytes early; retained workbench prose reports 11 aligned rows but its artifact is absent. Reproduce one full-TU baseline and try at most two forms sharing the early `D_800D21B0` base lifetime with the later `D_800D20A8` cursor while preserving its second HI/LO pair. Do not repeat the exhausted flag lattice or permuter. |
| `func_80012574` | track | **P!** (was a false **P**-matched) | The configured full-TU candidate is **50/57 words**, frame `0x48`, with the sole `R_MIPS_26 sqrtf` relocation exact at `+0xA4`; two callers provide five retail callsites. A bakeoff scratch probe using two empty conditions reached **55/57** by recovering the f14/f18 allocation, leaving only the projection spill store/reload at `sp+0x30` instead of `sp+0x38`; the probe is not retained because the workflow forbids invented guards. **The earlier "permuter → 0" was also a false ceiling** (see §"Permuter scratch fidelity" below) because stack offsets were normalized. The generated ranking's 36-word/size-mismatch row used the wrong per-file flags and must be regenerated after the recipe fix. Next run the bounded saved-local/declaration ladder; do **not** bank a scratch or permuter zero without canonical linked/ROM proof. |
| `func_8000D018` | track | **B — merged-TU blocker** | ✅ matched. Fixed surgically: a typed `#pragma weak trackCamPosTrap = TrapDanglingJump` alias + `objcopy --redefine-sym trackCamPosTrap=TrapDanglingJump` in the track.c.o rule, so the camera-position call passes (f32,f32,f32) single-precision without touching the shared TrapDanglingJump placeholder or its other call sites. |

### New wall class: **B — merged-TU symbol/placeholder blocker**
`func_8000D018` double-promotes `f32` call args to double because
`TrapDanglingJump` (real function at **0x800333A0**, size 0x1E0) is declared as an
unprototyped placeholder `s32 TrapDanglingJump()` and that ONE placeholder symbol
is reused across incompatible call sites (0/1/2/3 args, ints/pointers/floats) in
already-matched plain-C functions. IDO rejects any incompatible declaration
TU-wide, so a correct `(f32,f32,f32)` prototype cannot be added in isolation.
**Fix (project-level, also improves codebase health):** disassemble and identify
the real distinct function(s) hiding behind the `TrapDanglingJump` placeholder,
give each its real name + prototype, and repoint the call sites. This is a
structural-debt cleanup that unblocks func_8000D018 (and likely other
double-promotion residuals) — a good Fable/Opus task, tracked here.

**Update (2026-08-27): func_8000D018 resolved without the full web cleanup.**
The surgical per-call-site fix already used elsewhere (anim.c `animResetTrap`,
weather.c `rainInitTrap`) applies: declare a typed weak alias
(`#pragma weak trackCamPosTrap = TrapDanglingJump`, `extern void *trackCamPosTrap(f32, f32, f32)`),
call it at the one dangling site, and canonicalize its undefined symbol back to
TrapDanglingJump with `objcopy --redefine-sym` in the track.c.o build rule. The
alias gets the correct prototype in isolation, so IDO passes the three f32 args
single-precision (no `cvt.d.s`/`sdc1`), while every other TrapDanglingJump call
site and the shared placeholder itself are untouched. ROM stays byte-identical.
The broader web cleanup (naming the real distinct callees) remains optional
future health work, not a matching blocker.

## Permuter scratch fidelity (2026-08-27, tooling)

A permuter score of 0 is only trustworthy if the scratch object it searches is
**bit-identical to the real per-TU object** *and* the scorer counts every byte a
`gmake verify` counts. Two false-ceiling causes were found and fixed in
`tools/permute.sh`; a third is documented but not yet fixed. Rule of thumb:
**a permuter 0 that has not passed `gmake verify` is a hypothesis, not a match.**

1. **Post-compile `objcopy` not replicated (fixed).** Some TUs apply an
   `objcopy --redefine-sym A=B` after `cc` via the Makefile's per-file
   `POSTPROCESS` (e.g. `src/main/track.c`: `trackCamPosTrap=TrapDanglingJump`).
   The importer's scratch ran `cc` only. `permute.sh` now recovers that step
   from the same `gmake -n <obj>` dry-run it already uses for flags and appends
   it to the scratch `compile.sh` after `cc`, retargeted to `$OUTPUT`.
   Digest-guarded ELF surgery (`add_elf_relocations.py`, `trim_elf_section.py`)
   is deliberately *not* replicated — it is tied to the matched bytes and would
   abort on a permuted object — such TUs get a warning instead.

2. **Scorer normalizes stack offsets (fixed — this was the actual `func_80012574`
   ceiling).** decomp-permuter's scorer defaults to `stack_differences=False`
   (`src/scorer.py`/`src/objdump.py`), which strips every `sp`-relative offset
   before diffing. A candidate whose only residual is a spill/local at the wrong
   `sp` offset (`sw v1,0x18(sp)` vs `0x1C(sp)`) then scores **0** while its bytes
   still differ and `gmake verify` fails. Proven on `func_80012574`: its
   "winning" candidate was 4 stack-home words off the ROM yet scored 0.
   `permute.sh` now always passes `--stack-diffs`; with it the base is 61 and a
   15-min run plateaus at 16 (honest), instead of a false 0. *This is the fix
   that actually mattered for `func_80012574` — the objcopy replication does not
   touch that function's bytes, since it never references `trackCamPosTrap`.*

3. **Injected gfx-macro expansion (documented, NOT fixed).** The importer keeps
   the TU's `g[DS]P*`/`_SHIFTL`/`gDma*` macros as `#pragma _permuter latedefine`
   entries (`tools/permuter_settings.toml` `[preserve_macros]`) and restores them
   at candidate-compile time (`ast_util.py process_pragmas`). The definitions come
   from the TU's real headers via `cpp -dD -fdirectives-only`, so the *bodies* are
   correct — but for a display-list function like particles.c `func_80041CE4`
   the pruned/round-tripped scratch still reproduces a different frame (-136 vs
   -128) than the full TU. The remaining suspects are (a) a preserved macro that
   expands to a sub-macro *not* in the preserve set, so import-time vs
   search-time expansion order diverges, and (b) pycparser's AST round-trip of
   the macro-call arguments. **What must change:** compile the particles import
   both ways, byte-diff `func_80041CE4` between the scratch object and
   `build/src/main/particles.c.o`, and either widen `[preserve_macros]` to cover
   every gfx sub-macro the function touches or make the scratch include the real
   header directly (suppress the conflicting latedefine). Until then, treat any
   gfx-heavy permuter 0 as unverified. (particles.c also carries a
   `trim_elf_section .rodata 0x28` POSTPROCESS, which is correctly *skipped* by
   fix #1 with a warning — its scratch is not bit-identical for that reason too.)

## Epoch 14 sweep findings (2026-08-28, running log)

The fidelity-fixed batch runner (`docs/permute-batch.md` "Scratch fidelity")
is sweeping the whole NON_MATCHING queue closest-first. Entries here are
routing facts from that sweep; the sweep's `summary.json` is the measurement.

| Function | TU | Class | Note | Status |
|---|---|---|---|---|
| `func_8004D40C` | font | **P** (was recorded as pass-owned "lifetime" plateau, 2 words) | permuter score 205 → 0 in 255 s once the scratch used the TU's real flags; another false floor in the "proven" column | ✅ matched (permuter) |
| `overlay1CloneRecord` | o001 | **I** | import.py's scratch redeclares the function with a conflicting return type against the TU's own prototype (`Overlay1PathState *`); scratch never compiles, base score None | needs a prototype-aware import (workbench backlog #7 preflight would have flagged it) |
| `overlay40FadeRecords` | o040 | **R!** | retained configured full-TU/isolated C is 98/101 words with register-only sites at `+0xC/+0x10/+0x24`, frame `0x8`, and all ten runtime BSS tuples; resident `func_8000D978 +0x130` is the sole direct caller, while linked range/module/ROM proof is fallback-only | V0, reverse the `current`/`output` origin, then one declaration order; stop if both probes are flat, otherwise combine or initialize only strict gains within a five-run cap |
| `overlay74Update` | o074 standalone | **P!/W** | retained configured-isolated C is 94/100 words, frame `0x60`, with one five-site `t3/v0` aggregate-pointer exchange plus one commutative OR; eight runtime records versus four target-static calls, sole caller `func_8000AEEC +0x34C`, and no full-TU/linked C proof | V0, query ABI, no-argument hit ABI; combine only a strict gain, cap four |
| `overlay34InitStorage` | o034 | **F** | exact 50-word/`0x30`-frame shape; target uses `sp+0x1C` and IDO `sp+0x18` at `+0x24/+0x30/+0x78/+0x7C`; eight candidate/runtime relocations | reproof-only V0; park absent a new stack-allocation mechanism |
| `overlay20UpdateObjectResource` | o020 | **P!/W** | retained full-TU and isolated C agree at 90/98 words, frame `0x70`, with eight count/entries-web register sites from `+0xB0`. Four exact runtime calls resolve to `trackGetTrack`, `func_8000FEEC`, local `overlay20ConfigureResource`, and `sqrtf`; fallback symbols are collapsed placeholders | one unchanged full-TU V0; park absent a new pool-to-temp mechanism |
| `func_overlay_041_F0001650_1888988` | o041 | **P** | frameless 55-word candidate has six register-pool words at `+0x10/+0x18/+0xA0/+0xA4/+0xCC/+0xD0`; four candidate/runtime relocations; flag/permuter/globalcolor routes exhausted | reproof-only V0; park absent a new pool mechanism |
| `overlay18Load` | o018 | **P!** | relocation-aware pass exposed a 64-bit first-argument ABI; overlay metadata resolves the call to `osSetTime(OSTime)` | ✅ matched (ABI reconstruction) |
