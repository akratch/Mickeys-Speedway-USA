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
| `func_8003A2C8` | menu | **P!** | five-web trace: forced v1/v0 plus shared `modeBits` reaches 1/32; last memory-first comparison carrier is ring-only and not color-reachable | handoff |
| `func_8002CF6C` | saves | **P** | same-length ring permutation | grinding |
| `func_80020D8C` | models | **P** | +2 ring phase shift | queued |
| `func_8001A154` | lights | **P** (was mis-called **W**) | permuter cracked it: drop the redundant `& 0xFFFFU` blue mask (phantom ring pop) + fold `flare.scaledSize`. The 'no source lever' verdict was a FALSE FLOOR. | ✅ matched (permuter) |
| `mainUpdateZBCheck` | main | **P** (was **I** via -mips1 flag bug) | permuter cracked it once flags fixed: compare `(u16*)row+columns` directly, no cached pixel. | ✅ matched (permuter) |
| `func_80047CD8` | fx | **I** | configured `-DNON_MATCHING` build is blocked before the target by adjacent `func_800475E8` block-local C99 declarations; diagnostic copy reaches 226/234 exact rows with 8 register words from `+0x298`, pool slot 28, and no move/copy site | target plateau recorded; adjacent TU fix required |
| `func_8005A948` | models_5B300 | **I / P?** | import returned no improvement instantly — check base compiles | needs recheck |
| `func_8002C69C` | saves | **P!** | coherent cursor re-cache reduced 18→11 register-only words at exact 28-word size; first `+0x10`, four allocator webs; corrected MIPS2/stack-aware reseed reached cost 55 but no zero | plateau; F-route web-existence lever next |
| `func_80041CE4` | particles | **P?** | 27w register-only | queued |
| `func_8002BB40` | memory | **P?** | 30w register-only | queued |
| `runlinkEnsureJumpIsValid` | runlink | **P?/W** | 35w register-only; larger search space | queued (low odds) |
| `levelInit` | level | **W?** | 122w register-only; likely too large for the permuter search | queued (low odds) |

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
| `func_8002B7AC` | memory | **P** | BSS-base-in-s0 register plateau. |
| `func_80012574` | track | **P!** (was a false **P**-matched) | The configured full-TU candidate is **7/57 words** with an exact frame and `sqrtf` relocation. A bakeoff scratch probe using two empty conditions reached **2/57** by recovering the f14/f18 allocation, leaving only the projection spill store/reload at `sp+0x30` instead of `sp+0x38`; the probe is not retained because the workflow forbids invented guards. **The earlier "permuter → 0" was also a false ceiling** (see §"Permuter scratch fidelity" below) because stack offsets were normalized. The next legitimate attempt needs an F-route declaration/save-order lever or original local-layout evidence; do **not** bank a scratch or permuter zero without `gmake verify`. |
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
| `overlay40FadeRecords` | o040 | **P!** | base 75, no improvement in 20 min | re-seed / longer cap before any hand pass |
| `overlay18Load` | o018 | **P!** | 700 → 450 in 20 min, still descending | extension candidate |
