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
| `func_8003A2C8` | menu | **P!** | retained pre-HEAD/current-body configured full-TU and isolated C agree at 27/32 words, frameless, with six exact relocations; the five differences are the initial mode-value web. Its three inert arithmetic aids make that score diagnostic, and clean current-layout V0 is uncompiled. ORT 606 and three callers are authenticated; linked equality is fallback-only. Forced-color 31/32 and route exhaustion are unretained prose | run exactly 119 flags on clean V0, one allocator trace, independent split-web and reversed-comparison forms, then an improving-only combination; optional improving-only batch, cap 122 stock builds plus trace |
| `overlay11UpdateMenu` | o11 standalone | **P!/W** | retained pre-current-identity/current-body C is 285/301 annotated raw, 278/301 literal, and 299/301 runtime-normalized words with all 102 sites; only `+0x138/+0x140` remain. Its three dead frame-padding arrays make that score diagnostic; they are removed and clean V0 is uncompiled. The `+0x8` identity and all 13 SYMBOL zero carriers are repaired; sole local inbound/unexported status are authenticated and linked equality is fallback-only. Historical flags, trace, probes, and search are unretained | clean identity V0, run 119 flags, one trace, at most two natural forms and an improving-only combination; cap 122 stock builds plus trace and batch only after a strict gain |
| `overlay5InitializeAudio` | o5 mixed TU | **F/P!/W** | retained prior-layout configured full-TU and selective C agree at 211/233 raw and normalized words, frame `0x98`, with all 71 offset/type sites. Twenty-two carrier/order/stack-home sites remain, two friendly names collapse four runtime identities, and linked equality is fallback-only | identity-correct V0, exactly 119 flags, one allocator trace, at most three independent natural forms and one improving-only combination; optional improving-only batch, cap 124 deterministic builds |
| `overlay99RenderSortedEntries` | o99 standalone | **F/P!** | retained prior-layout configured full-TU and isolated C agree at 216/233 raw and 218/233 normalized words, frame `0x148`, with ten runtime-backed sites. Fifteen sorted-home/command-web sites remain; six diagnostic call names still need canonical identities, and linked equality is fallback-only | identity-correct V0, exactly 119 flags, four `sorted[]` placements, one trace-selected form, then one improving-only batch; cap 125 deterministic builds |
| `func_overlay_061_F0001648_18C0A10` | o61 standalone | **reproof** | current guarded `do/while (0)` replay matches the claimed exact lane source, but no C object from that spelling survives. Prior C is 78/92 with all 11 runtime sites; exact linked evidence proves fallback only, and the local `+0x164` alias is missing | define the identity-correct local alias, compile current V0 once, then prove 92/92, all 11 tuples, owned range, module, and ROM before removing the guard |
| `overlay31InitializeBuffers` | o31 standalone | **reproof** | current guarded declaration-order replay matches the claimed exact lane source, but no exact C object survives. Prior C is 225/245 with frame `0x40` versus target `0x48`; all 54 shipped runtime identities are decoded and align with its static sites, while C-linked equality remains unproved | compile current V0 once, prove 245/245, frame `0x48`, all 54 decoded roles, owned range, module, and ROM before removing the guard |
| `func_overlay_041_F0000000_1887338` | o41 standalone | **reproof** | current guarded decrement reorder is semantics-preserving and matches the claimed exact lane source, but no C object survives. Prior C is 47/73 with frame `0x30`; exact linked evidence proves fallback only, while all three runtime identities are authenticated | compile current replay once and prove 73/73, frame `0x30`, three tuples, owned range, module, and ROM before removing the guard |
| `overlay21ApplyPriorities` | o21 standalone | **reproof** | dirty unguarded source matches the claimed exact lane body, but no current-layout C object survives. Exact-looking retained objects are fallback assembly; target is 114 words/frame `0x28` with nine runtime records and separate trailing padding | restore guarded ownership in an isolated current lane, compile the candidate once, then prove 114/114, all nine tuples, owned range, module, and ROM before accepting scoreboard credit |
| `func_overlay_045_F0000764_188CBBC` | o45 standalone | **F** | committed p4 evidence supports historical natural 634/637 words, frame `0x88`; its three LO16 residuals were hidden by prohibited instruction externalization, and distinct call identities were collapsed. Exact linked evidence proves fallback only | current V0, all 24 identities, then at most four natural field/object accesses to the owned constants; no broad permutation or externalization |
| `overlay97InitScale` | o97 standalone | **F/P!** | retained configured C is 143/144 raw/normalized words, frameless, first/only `+0xD0`, with zero relocations. Its artifact predates HEAD, but source and physical boundaries are unchanged. Candidate derives the values cursor as `a1+0x3E`; target uses equivalent `a3+2`. The isolated three-word row omitted `-Wab,-r4300_mul`; sole caller is `func_8000AA38+0x7C` | current V0; assign bounds first, then independently try `(u8 *)bounds+2` and `&bounds->bounds[1]`; conditional narrow-scope fourth build only after drift plus strict gain; no generic permutation |
| `func_8002CF6C` | saves | **F/P!/W** | retained pre-HEAD/current-body full-TU and isolated C agree at 79/88 words, frame `0x48`, all 11 relocations exact; one savedFlag/current-byte/final-copy FIFO owns nine sites. Its synthetic padded struct/volatile buffer makes 79/88 diagnostic; natural scalars replace it and clean V0 is uncompiled. ORT 505 and sole caller are authenticated; linked equality is fallback-only. Historical flags, trace, forms, and search are unretained | run 119 flags on clean V0, one trace, and at most two mutually exclusive savedFlag-lifetime forms; cap 122 stock builds plus trace and batch only after a strict gain |
| `func_80020D8C` | models | **P/W** | retained prior-layout full-TU and isolated C agree at 35/48 words, frame `0x8`, no relocations; thirteen sites span texture-address FIFO, frame-count, coupled frame/count, and multiply-result webs. Its redundant low-half mask on an `s16` store makes 35/48 diagnostic; the mask is removed and clean V0 is uncompiled. ORT 374 and nine calls across four functions are authenticated; linked equality is fallback-only. Historical flags, trace, source forms, and search are unretained | run 119 flags on clean V0, one allocator trace, one natural form per web family, then an improving-only combination; cap 122 stock builds plus trace and batch only after a strict gain |
| `func_8000DDE4` | track | **P!/W** | retained isolated current-body C is 94/118 words, frame `0x28`, first `+0x24`, with 24 register-field sites and exact calls at `+0x114/+0x128`; its import omitted `-Wab,-r4300_mul`, no configured full-TU C object survives, and the historical 15-word basin is unretained | configured/isolated V0, merge `recordIndex/passCount`, then reuse dead `key` as `nextValue` only on strict gain; one ten-minute declaration/lifetime sweep only from an improved rung |
| `func_80019DE8` | lights | **F/P!/W** | retained configured full-TU C has the exact `0x38` frame but 64 words versus target 63, 19/63 positional matches, 44 shared-word differences plus one trailing nop, first `+0x44`; all three relocation identities agree but sit `+4` late. Only V0 is artifact-backed | fresh V0, retain the full flag lattice, two mutually exclusive value-delta-first forms, one trace-selected form, then one improving-only annotated batch |
| `debug_text_width` | diprint | **F/P!/W** | retained prior-line-layout configured full-TU C is 59/66 words, frame `0x138`, first `+0x38`, with five exact relocations; no compiled selective object survives. Seven sites are one current-byte/classification web plus branch-likely newline lowering. ORT 862 has no caller after exhaustive resident/overlay/jal/pointer/source scans; linked equality is fallback-only. Historical source, flags, trace, and search are unretained | current V0, run 119 flags, one trace, separate-byte and newline/tail forms plus an improving-only combination; cap 122 stock builds plus trace and batch only after a strict gain |
| `func_8001A154` | lights | **P** (was mis-called **W**) | permuter cracked it: drop the redundant `& 0xFFFFU` blue mask (phantom ring pop) + fold `flare.scaledSize`. The 'no source lever' verdict was a FALSE FLOOR. | ✅ matched (permuter) |
| `mainUpdateZBCheck` | main | **P** (was **I** via -mips1 flag bug) | permuter cracked it once flags fixed: compare `(u16*)row+columns` directly, no cached pixel. | ✅ matched (permuter) |
| `func_80028FCC` | main | **F/W** | retained pre-current-line-layout full-TU and isolated C agree at 17/27 words/frame `0x18`; target materializes three boolean temporaries and a shared epilogue. Historical structural, flag, trace, and search outcomes are unretained | current-layout V0, retained flag lattice, one reused normalized-result form, then one improving-only trace-selected lifetime/epilogue form |
| `func_overlay_009_F0000540_1866BB8` | o9 mixed TU | **P!/W** | retained standalone evidence omitted required `-Wab,-r4300_mul`; 121/129 normalized words/frame `0x58` agree, with a four-way saved-FPR color cycle and ten runtime records | one identity-correct configured full-TU/linked V0, then park if reproduced |
| `overlay7DispatchModes` | o7 tail TU | **P!/W** | retained prior-layout full-TU and isolated C are 128/131 raw and 129/131 runtime-normalized, frame `0x20`, first `+0x44`, with all 23 text and seven switch-table records. The two normalized sites are one `t3/t4` flag-value web. Its redundant low-ten-bit mask made that score diagnostic; the mask is removed and clean V0 is uncompiled. ORT 1471 has six callers; linked equality is fallback-only | clean identity-correct V0, retain 119 flags, one traces-off-faithful allocator trace, and at most two trace-selected natural scalar/scope forms; cap 121 stock builds plus trace and park after two flat forms |
| `func_8005A7A0` | models_5B300 | **F/W** | retained configured full-TU candidate is exact-sized at 106 words but has frame `0x50` versus target `0x38`, ten raw/seven relocation-normalized sites, and nine exact tuples; the second loader call shifts `+0xBC` → `+0xC0` | one unchanged configured full-TU/linked V0, then park |
| `overlay59PrepareEntry` | o59 standalone | **P!/W** | retained pre-HEAD configured full-TU and isolated C agree at 52/62 raw and 53/62 runtime-normalized words/frame `0x28`; nine sites are one descriptor-value/call-argument web. All six identities are exact, but current alias spelling is uncompiled; no padding, no export, and four local calls from `overlay59Advance` | exactly 119 flags including identity-correct V0, one trace, split descriptor input/acquisition result carrier, then only after strict gain one line/scope form; cap 121 stock builds plus trace |
| `overlay19ClassifyEdge` | o19 standalone | **P!/W** | retained pre-HEAD/current-body configured full-TU and isolated C agree at 110/120 words, frame/relocations zero. Six carrier sites differ at `+0x138/+0x140/+0x148/+0x154/+0x15C/+0x164`; y/z load pairs reverse at `+0x194/+0x198` and `+0x1B0/+0x1B4`. Unexported; sole inbound is `overlay19FindAdjacent+0xD8`. Historical route claims are unretained; linked equality proves fallback only | exactly 119 flags including V0, one trace, one natural carrier form, one line/association form, improving-only combination; cap 122 stock builds plus trace, park if both probes flat |
| `overlay99BuildHeightGrid` | o099 standalone | **F/W** | retained pre-p8 isolated C is diagnostic 104/114 normalized words/frame `0x28`, with seven final-grid address sites plus three magnitude sites and all 29 tuples; its recipe omitted `-Wo,-loopunroll,0`. The unretained p8 reportedly removed seven sites, but both bodies used an invented empty `unused` guard, now removed. Clean V0 and linked C are unproved | clean configured V0, exactly 119 flags, one allocator trace, then only its indicated natural magnitude-carrier form; park if flat |
| `overlay1ResolvePathPoint` | o1 end TU | **P!** | retained evidence is exact-sized at 152 words with ten raw/eight normalized schedule/register sites and 19/22 runtime tuples; source had an invented clear alias and a nine-argument declaration for the observed seven-argument ABI | ABI/identity-correct V0, then one bounded annotated-target permutation if reproduced |
| `overlay14ResetMode` | o14 standalone | **F/W** | retained pre-current-alias full-TU and isolated C agree at 45/56 raw and 49/56 normalized words, frame `0x30`; seven sites are one prologue schedule and all later text is exact. The shipped function owns 18 records, while the fallback target object exposes ten. Current source repairs the `+0x54` SYMBOL callee, but fresh compiled identity proof is pending. Historical probes, lattice, trace, oracle, and search outcomes are unretained | current identity V0, run and retain 119 flags, one trace, at most three natural `s3`-lifetime forms, then improving-only batch; cap 122 stock builds plus trace |
| `overlay1FindBestRecord` | o1 tail TU | **P!** | retained isolated and genuinely prior-layout full-TU C agree at 18/30 words, frameless, with one 12-site `a1/a3` carrier exchange and all four runtime tuples. The configured producer proves the isolated recipe's omitted `-Wab,-r4300_mul` inert. It is unexported with two local callers; current-layout C is pending and linked equality is fallback-only. Historical flag, trace, source, and search outcomes are unretained | current V0, run and retain 119 flags, one allocator trace, and at most three trace-informed declaration/scope forms; cap 122 stock builds plus trace, park after three flat forms |
| `overlay1FindType5ByKey` | o1 start TU | **P!/W** | retained pre-HEAD/current-body configured full-TU and isolated C agree at 22/39 words, frame `0x30`, first `+0x1C`; 17 sites split into loop/cursor and key-comparison FIFOs. Runtime proves the outbound `+0x14` identity to resident `func_8000572C`; source and metadata now express it. ORT 1485 and sole inbound `overlay1TransitionState+0x40` are authenticated; linked equality is fallback-only. Historical flags, trace, forms, and search are unretained | current identity V0, run and retain 119 flags, one trace, split array/cursor and scoped record-key forms, then an improving-only combination; cap 123 stock builds plus trace and batch only after a strict gain |
| `rcpClearZBuffer` | rcpFast3d | **F/W** | retained configured full-TU C from the preceding declaration state is 103/107 words, frameless, with both relocations exact and one four-site temporary FIFO at `+0x74..+0x80`; isolated C is 96/107 and adds seven schedule sites. ORT 765 and four callers are authenticated; linked equality is fallback-only. Historical flags, probes, trace, and search outcomes are unretained | current-source V0, run and retain 119 flags, one UGEN FIFO trace, and at most two trace-selected forms; cap 121 stock builds plus trace, park if flat |
| `overlay62Update` | o62 standalone | **P!** | retained configured C is diagnostic 289/294 raw and 290/294 runtime-normalized words/frame `0x88`, with four register sites at `+0x44/+0x50/+0x54/+0x64` and all 71 tuples. Its invented empty guard and redundant `red | 0` are removed, so clean V0 is uncompiled. The isolated 11-word row expanded `OVERLAY62_EMIT` and is fidelity-defective | clean macro-faithful V0, exactly 119 flags, one natural shared-negation/shift lifetime, then only after strict gain one 20-minute macro-preserving annotated batch |
| `overlay43FilterImage` | o43 standalone | **F/P!** | retained configured C is diagnostic 39/43 words, frameless, no relocations, with one four-site pixel-expression FIFO at `+0xC/+0x10/+0x20/+0x28`; its two invented empty guards are removed, so clean V0 is uncompiled. The `+0xAC..+0xB8` padding is separate; two local JUMPs are the only callers | authenticate the historical donor-negative route, compile clean V0, retain 119 flags, take one allocator trace, then at most two independent natural forms and one improving-only combination; cap 123 stock builds plus trace |
| `func_80047CD8` | fx | **P!/W** | retained prior-line-layout configured full-TU C is 226/234 raw/normalized words, frame `0x68`, first `+0x298`, with four exact relocations. Five sites are the `cone->mode` web and three the `segmentCount` web. The isolated ranking is only 214/234; allocator/flag/search conclusions are unretained | current-layout V0, retain 119 flags, one allocator trace, one natural form per web and an improving-only combination, then one 20-minute annotated batch; cap 122 deterministic trials |
| `func_80045BBC` | diCpu | **F/W** | retained configured full-TU and isolated C are 60 words/frame `0x30`; eight raw/two value-normalized sites, target 24 versus candidate 18 static relocations, with a final `t6/t4` FIFO web. Only V0 has attributable artifacts and no linked C proof survives | V0 and flag lattice; three independent identity probes plus improving combinations; one trace-led FIFO form, then an improving-only annotated batch |
| `func_overlay_014_F0001830_1871108` | o14 standalone | **F/W** | retained 196/201 normalized words and 21-record/exact-table shape, but all eight calls used one false callee identity; five case-7 load/increment schedule sites remain | ABI-correct V0, then one target-shaped positive-index call rung; park if flat |
| `levelGetCounts` | level | **F/W** | retained configured full-TU C is 255/259 raw and 256/259 address-normalized words, frame `0x58`, with 37 records. One `$v0`/target-`$a0` web remains plus target `D_800CF420` versus candidate `D_800CF3E0+0x40`; the lower isolated score is a reformatted import. Overlay 18 ORT 518 is the sole inbound | V0 and flag lattice; independent endpoint and trace-selected web probes; improving-only combination and annotated batch |
| `func_8002B524` | memory | **F** | retained configured full-TU and isolated current-body C agree at 102/116 raw/normalized words, frame `0x58`, with all 12 tuples exact; ten slot/data carrier sites, two lower-guard structural sites, and candidate `sp+0x38` versus target `sp+0x3C` remain. Linked equality is fallback-only | fresh V0, slot-data cache, split guards, improving combination, then improving-only call-live record; cap five |
| `func_overlay_073_F0000000_18CAAC0` | o73 standalone | **P** | retained configured full-TU and isolated C agree at 90/100 raw and 92/100 runtime-normalized words/frame `0x8`; one `a2/a3` web differs at `+0x40/+0x50/+0x80/+0x84/+0x94/+0x9C/+0xA8/+0xB0`. All eight LOCAL identities are exact; ORT 1248 exports it and `func_8000AA38+0x42C` is the sole inbound. Historical route claims are unretained | V0 in exactly 119 flags, one traces-off-faithful allocator trace, at most two trace-selected natural forms, then one bounded annotated batch; cap 122 stock builds plus batch |
| `func_8005A948` | models_5B300 | **F/W** | retained configured full-TU C compiles the earlier structured-store body at 83/94 raw/normalized words, frame `0x38`, first `+0x40`, with 13 exact static relocations. Current raw-array p10 is unscored; its reported 91/94 result and route outcomes are unretained. Zero runtime records; sole caller is `func_8005A7A0+0x104` | current V0, retain exactly 119 flags, one allocator trace, at most one trace-selected address-producer/lifetime form; optional improving-only 20-minute batch, cap 120 deterministic builds |
| `func_80055B24` | anim | **P** | typed `void` callback alias removes the generic trap placeholder's phantom return web; metadata-only rebind restores the exact call identity | ✅ matched (ABI reconstruction) |
| `func_8002C69C` | saves | **P!/W** | retained configured full-TU and isolated C compile the earlier cursor-update body at 10/28 words, exact frameless shape, first `+0x10`, with 18 register-field differences and no relocations. The current re-cache body's reported 17/28 result and later route outcomes are unretained | current-body V0, retain the full flag lattice, one allocator trace, at most two independent trace-selected forms and one improving-only combination; optional improving-only batch, cap 122 deterministic trials |
| `func_80041CE4` | particles | **P!** | retained configured full-TU and isolated C agree at 126/153 words, frame `0x80`, with all nine relocations exact; the omitted isolated `-Wab,-r4300_mul` flag is inert. The 27 sites span outer/point-count and display-list/command carrier webs; historical route outcomes are unretained | preserve V0, retain flag lattice, two trace-selected forms and improving combination, then one corrected-flag annotated batch |
| `func_8002BB40` | memory | **P** | retained configured isolated C is 42/72 words with 30 register-only differences from `+0x8C` and all eight tuples exact; the matching full-TU result is historical and unretained, while the apparent 72/72 ordinary object is `GLOBAL_ASM` | reproduce V0 and the historical 57/72 JFG form, then bounded dead-argument carrier ladder; six-build cap |
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
| `func_8002B7AC` | memory | **W — parked** | retained configured full-TU and isolated current-body C agree at 62 words versus target 63, frame `0x30`, with 62 raw/normalized positional differences from `+0x4`. The 12 relocation identities agree as a multiset, but eleven records are four bytes early and the first `D_800D20A8` LO16 is twelve bytes early. The historical 11-row aligned result has no surviving artifact; linked equality is fallback-only | fresh V0, then lexical and explicit early-`D_800D21B0`/later-`D_800D20A8` lifetime handoffs; preserve both delay-array pairs and stop after those two forms |
| `func_80012574` | track | **P!** (was a false **P**-matched) | Historical configured measurement is **50/57 words**, frame `0x48`, with the sole `R_MIPS_26 sqrtf` relocation exact at `+0xA4`, but no configured C object survives; the retained isolated object omitted `-Wab,-r4300_mul` and is only 55 words. Two callers provide five retail callsites. A bakeoff scratch probe using two empty conditions reached **55/57** by recovering the f14/f18 allocation, leaving only the projection spill store/reload at `sp+0x30` instead of `sp+0x38`; the probe is rejected because the workflow forbids invented guards. **The earlier "permuter → 0" was also a false ceiling** (see §"Permuter scratch fidelity" below) because stack offsets were normalized. Reproduce configured V0 before the bounded saved-local/declaration ladder; do **not** bank a scratch or permuter zero without canonical linked/ROM proof. |
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
   correct. The historical `func_80041CE4` scratch frame discrepancy is now
   resolved: retained isolated and configured full-TU C are byte-identical at
   frame `0x80`. Other gfx-heavy imports still require this byte-diff preflight,
   because a preserved macro can expand through an unpreserved sub-macro or
   pycparser can alter macro-call argument structure. Treat any gfx-heavy
   permuter zero as unverified until its scratch baseline equals the configured
   full-TU object. (particles.c also carries a
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
| `overlay1AssignRecordIndex` | o001 tail TU | **F!/W** | retained pre-HEAD diagnostic C is 34/44 literal and 36/44 runtime-normalized words/frame `0x38`; five stack-home and three conditional-store sites remain. Its dummy volatile, false fourth argument, private identity, and literal write are removed. Clean V0 now uses the three-argument GetRomlistInfo alias and symbolic `D_1D8C` for all seven records, but is uncompiled. ORT 1219 and sole resident inbound are authenticated; historical lattice, trace, probes, and search are unretained | clean identity V0, run and retain 119 flags, one trace, at most four natural forms and two strict-gain combinations; batch only after a natural gain |
| `overlay40FadeRecords` | o040 | **F/P!** | retained current-layout configured full-TU/isolated C is 98/101 words with a reversed `current`/`output` value-origin web at `+0xC/+0x10/+0x24`, frame `0x8`, and all ten runtime tuples; ORT 1314 and runtime trap inbound `func_8000D978+0x130` are authenticated, while linked equality is fallback-only | V0, exactly 119 flags scored raw, one allocator trace, independent value-origin/declaration probes, then improving-only combinations; cap five source forms |
| `overlay74Update` | o074 standalone | **F/P!** | retained full-TU and isolated C agree at diagnostic 94/100 words/frame `0x60`, but that body used two invented empty guards. They are removed; source-faithful V0 is uncompiled. Eight runtime identities and sole inbound `func_8000AEEC+0x34C` are authenticated; linked equality is fallback-only | clean V0, both resident ABI probes, exactly 119 flags, then one reversed outer OR under the winning recipe; cap 123 deterministic builds |
| `overlay34InitStorage` | o034 | **F→W** | retained configured full-TU/isolated C is 45/50 raw and 46/50 runtime-normalized with exact 50-word/`0x30`-frame shape and all eight identities; target uses `sp+0x1C` and IDO `sp+0x18` at `+0x24/+0x30/+0x78/+0x7C`. The mechanism is one call-live `size` home, but no target trace proves the source lever | V0, retain exactly 119 flags, one allocator trace, then only if supported one post-`size` volatile-home probe; park as W if trace-flat or nonexact |
| `overlay20UpdateObjectResource` | o020 | **P!/W** | retained pre-HEAD/current-layout full-TU and isolated C agree at 90/98 raw/normalized words, frame `0x70`, first `+0xB0`. Four count/entries sites precede a four-site downstream `value0F`/`start` argument cluster; independence is unproved. Runtime tables resolve the four exact calls to `trackGetTrack`, `func_8000FEEC`, local `overlay20ConfigureResource`, and `sqrtf`; C names remain proxies. Sole inbound is `func_8000AA38+0x2CC` | exactly 119 flags including V0, one trace, one natural form per cluster and improving-only combination; optional ≤2,000-candidate batch only after strict gain; cap 122 deterministic builds |
| `func_overlay_041_F0001650_1888988` | o041 | **P!/W** | retained configured-recipe isolated C is frameless and 49/55 words. Four count-web sites exchange `a1/v1` at `+0x10/+0x18/+0xCC/+0xD0`; the commutative float loads reverse at `+0xA0/+0xA4`. Runtime resolves `gOverlay41Slots` to resident `D_800D6C58` and local `D_0[0x15]` to module `+0x1E34`; historical route exhaustion is unretained | current V0 in the 119-flag lattice, multiplication-operand reversal, then only if needed one trace-selected natural count-web form; cap 121 stock builds plus trace |
| `overlay18Load` | o018 | **P!** | relocation-aware pass exposed a 64-bit first-argument ABI; overlay metadata resolves the call to `osSetTime(OSTime)` | ✅ matched (ABI reconstruction) |
