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
| `func_8003A2C8` | menu | **P!** | v0/v1 mode/modeBits inversion; permuter 35 → 25, capped | grinding |
| `func_8002CF6C` | saves | **P** | same-length ring permutation | grinding |
| `func_80020D8C` | models | **P** | +2 ring phase shift | queued |
| `func_8001A154` | lights | **P** (was mis-called **W**) | permuter cracked it: drop the redundant `& 0xFFFFU` blue mask (phantom ring pop) + fold `flare.scaledSize`. The 'no source lever' verdict was a FALSE FLOOR. | ✅ matched (permuter) |
| `mainUpdateZBCheck` | main | **P** (was **I** via -mips1 flag bug) | permuter cracked it once flags fixed: compare `(u16*)row+columns` directly, no cached pixel. | ✅ matched (permuter) |
| `func_80047CD8` | fx | **I** | does not compile under `-DNON_MATCHING` in-TU (noted by trace agent) | needs candidate fix |
| `func_8005A948` | models_5B300 | **I / P?** | import returned no improvement instantly — check base compiles | needs recheck |
| `func_8002C69C` | saves | **P?** | 18w register-only; import status TBD | grinding |
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
| `func_80012574` | track | **P** | f14/f18 projection-web register permutation; frame exact. |
| `func_8000D018` | track | **B — merged-TU blocker** | NOT permuter, NOT hand-lever. See below. |

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
