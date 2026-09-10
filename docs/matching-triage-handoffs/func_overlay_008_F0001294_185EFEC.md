<!-- plateau-handoff:func_overlay_008_F0001294_185EFEC:start -->
### `func_overlay_008_F0001294_185EFEC` plateau handoff

- source: `src/overlays/o008/overlay_008.c`
- score: 636 differing words
- frame: 0xC8
- relocations: 137
- first mismatch: +0x0
- summary: Exact-size structural reconstruction; 24 excess frame bytes and 18 opcode edits remain. Next: scoped allocation/alias analysis, not flags or permutation.

## 2026-09-10 region partition (lane `lane/w8-recon`): the residual is naming, not structure

No source change is adopted here. The pass replaces the standing description of
this residual ("24 excess frame bytes and 18 opcode edits") with a measured
partition, and closes one axis with a complete lattice.

### The residual, split by cause

Aligning the two instruction sequences on a register-erased shape --- opcode,
function code, shift amount, immediate and branch offset kept, every GPR *and*
FPR selector erased --- and classifying each aligned pair:

- function: 1259
- positional differing (masked): 636
- byte-exact once displacement is removed: 727
- **displacement tax**: **104 (16% of the residual)**
- **register-naming-only rows**: **489 (77%)**
- rows differing in instruction kind, or unalignable: 55 (9%)

So this whale is *not* a displacement problem and never was: fewer than a fifth
of its residual is instructions sitting in the wrong place, and 55 words is the
whole of what is actually wrong. Displacement is confined to one clean -1 run
over +0x228..+0x46C; everywhere else the two sides are in phase.

### The dominant term is two float-register transpositions

Of the 489 register-naming rows, **209 differ only in a floating-point register
selector**, and they are dominated by two transpositions that run the length of
the function and go in both directions:

- `$f2` exchanged with `$f12`: 117 sites
- `$f10` exchanged with `$f16`: 89 sites
- everything else (`$f4`/`$f6`, `$f18`/`$f0`, `$f8`/`$f6`): about 60 sites

Two float webs hold each other's colours, and a second pair does the same. The
integer side is 267 rows and is a position-dependent rotation of `t0`-`t9`
(+1 through the middle of the function, reversing near the tail) plus a
`v0`/`v1`/`a0`/`a1` shuffle in the entry region. **The callee-saved integer
allocation is already exact: there are zero `s`-register substitutions anywhere
in the function.** No prior pass recorded that, and it removes a whole family of
hypotheses.

This is the first lever to attack. It is one allocation decision replicated
across a third of the function; the earlier "scoped allocation/alias analysis"
route was right about the axis but was aimed at the integer side.

### The 24 excess frame bytes are six words of declared local, and declaration order is not the axis

A typed stack-home census settles what the frame difference is and is not.
Every sp-relative reference on both sides is a plain load or store --- neither
side forms a stack address in a register --- so the census is complete:

- **19 distinct homes and 50 accesses on each side.** The outgoing-argument
  slots, the two double saves, the four saved-register slots and the return
  address are at *identical* offsets on both sides.
- The nine non-save homes correspond one-for-one, with matching access counts
  and matching widths: two float locals (2 accesses each), one 5-access int,
  three 3-access ints, two 2-access ints, and the 2-access int at +0x44 which
  is at the *same* offset on both sides.
- So the candidate has neither an extra local nor a missing one. The +24 bytes
  are 6 words of *declared* storage the target function did not have.

The frame responds to declared local bytes and nothing else here: growing the
`colorEnabled` array to 4 and 6 elements moves the frame 0xC8 -> 0xD0 -> 0xD8
in 8-byte steps while leaving the emitted code identical (636 masked, 727
aligned-exact). Reaching the target's 0xB0 therefore needs the candidate's
declared locals to shrink by exactly 24 bytes.

**Declaration order does not do it.** A complete move-one lattice over all 32
declarations --- 962 compiled and scored candidates, every ordering reachable by
relocating one declaration --- leaves the frame at 0xC8 in *every* case. The set
of frames reached is `{0xC8}`. The best score in the whole lattice is 630
(`colorEnabled` moved to position 16) against a base of 636, and the
aligned-exact count moves 727 -> 733; nothing there is worth adopting. The
standing instruction to "recover the stack-home layout before any further
spelling search" therefore cannot be followed by permuting declarations: the
frame is decided by how many locals exist, not by their order.

None of the 32 locals is dead (each is read and written), so shedding 6 words
means merging six pairs of disjoint-lifetime carriers. That is a bounded search
with a hard stopping criterion --- the frame reaching 0xB0 --- and it is the
second lever.

### Next levers, in order

1. The `$f2`/`$f12` and `$f10`/`$f16` float-web exchanges (209 rows).
2. Six carrier merges to bring the declared local bytes down by 24 (frame
   0xC8 -> 0xB0); the home *set* is already correct, only the count is not.
3. The 55 kind-differing words, which is all that is left of the structure.

Do not re-run the declaration-order family: 962 measured candidates, frame
flat, score flat within 6 words.

Validation: measurements come from `tools/score_symbol.py`'s comparator; the
lattice harness reproduces its baseline exactly (636 masked, first +0x0, frame
0xC8, size delta 0). `gmake verify` passed with the expected US ROM SHA1. The
tracked source is unchanged by this pass.

2026-09-08 decompiler-assisted reconstruction, assignment base `0d21245897f0469ec630a49a07024b5e51ad04b4`:

- The lane independently reproduced the configured full-TU baseline: 1,259 words, 1,129 raw and 1,127 relocation-masked differences, first mismatch `+0x0`, frame `0x180`. The workbench verdict was `structure-mismatch`; its `next:` route was constant-audit before structure and register classes. The structure-buckets and stack-frame-recovery guides were also consulted. The prior donor and flag results below were retained; no donor body, flag lattice, permutation, compiler modification or instruction patch was used in this packet.
- Runtime metadata re-established all 137 records and 63 target identities, including the 15 calls. The local helper overwrites its incoming fourth argument before use, confirming the existing three-argument call. Names in the raw extracted assembly alone are insufficient because every call initially shares a placeholder.
- Fresh m2c generation was attempted after `./generate_ctx.sh`. The aggregate project context failed on an unrelated flexible-array field and then an incomplete SDK structure. An ignored minimal context with the authenticated call prototypes produced the draft. Drafts, reconstructed relocation annotations, objects and diagnostic rows remain only in ignored `m2cfiles/` and `build/wb/o008-reconstruction/`. No generated draft text was copied into tracked source or comments.
- Three semantic defects in the inherited candidate were corrected using Mickey's own instruction/dataflow evidence: the loop now tests the previous count while storing the next count, so an update of one executes one body; both braking-global writes are halfwords rather than one halfword and one erroneous word; the angle-difference helper receives the complete signed 32-bit requested angle. The resident callee subtracts its full arguments and performs one wrap adjustment, so caller-side halfword truncation was not established by its behavior. The local declaration and call now preserve that ABI.
- Float tuning and interpolation accesses now use float pointers. Sequential value/scale phases retain explicit loads while sharing nonoverlapping carriers. The signed-halfword steering fields use a promoted integer carrier for arithmetic and retain explicit narrowing at stores. Paired color-enable values use a two-element array, recovering the observed memory-backed channel flags. Raw steering input and its clamped value stay separate; collapsing them had removed target control/dataflow. Locals now describe these roles instead of draft register names.
- Attempt 25 was the last strict best: 1,259 words / 5,036 bytes, 636 masked and 654 raw differences, first `+0x0`, frame `0xC8` against `0xB0`. This is 491 fewer masked differences and 184 fewer frame bytes than the independently reproduced baseline. Opcode alignment has 18 edit operations (4 replacements, 7 insertions, 7 deletions); this diagnostic is not an exact-match score. The configured static surface has 137 records, 102 offset/type alignments and 88 authenticated identity alignments against 137 runtime records. Identity resolution remains partial; none of these counts establishes object or linked equality.
- Source/ABI improvements were banked throughout the reconstruction artifacts. Notable intermediate measurements were attempt 11's counted loop at 1,256 words / 1,013 masked differences; attempt 16's corrected angle ABI at 1,256 / 1,013; attempt 21's explicit final Boolean alternative at 1,258 / 932; and attempt 25's distinct steering carriers at the exact size / 636. The old exact-size baseline is semantically defective and is not the candidate to restore merely because its size already agreed.
- Stall evidence: after attempt 25, attempts 26 and 27 captured the velocity-history value with shared and separate carriers (both 1,264 words, 1,128 and 1,129 masked differences); attempt 28 moved the history write before disjoint impulse stores (1,262 / 1,092); attempt 29 removed the redundant named sound-address carrier (owned bytes and relocation tuples unchanged); attempt 30 reused the expired integer index for the cooldown load (1,259 / 644); attempt 31 represented all top-speed accesses through one pointer (1,267 / 1,212, and 146 relocation records). These six attempts produced no better retained residual, new target identity or elimination of the remaining carrier/aliasing hypothesis. They are failed source forms, not a proof that allocation is unreachable. Earlier rejected forms include nested phase scopes, a value-position compound Boolean predicate, and a single-use top-speed pointer; their artifacts and measurements are retained.
- The final source retains attempt 29's equivalent cleanup and readable local names. A direct extraction from the configured full-TU object confirmed that the owned 5,036 bytes and all 137 static relocation tuples are unchanged from that retained result. The assembly fallback remains canonical. No bytes are credited and no candidate linked-ROM equality is claimed.
- Next concrete lever: request a specifically scoped causal allocation/aliasing packet on this new source/handoff pin. Start with the 24 excess frame bytes and the different scalar/field-cache webs around impulse history; the remaining opcode edits also include the top-speed address materialization before the local helper. Recover the stack-home layout and register webs with compiler evidence before any further spelling search. Generic permutation and flags remain outside this reconstruction authorization. Exact promotion will still require complete relocation identity/offset/type proof, canonical linked-range extraction, and a passing full-ROM verification.

Validation for the retained reconstruction:

- `tools/finalize_plateau.py` refreshed the guarded source marker and structured handoff with the measured frame, score, relocation count and first mismatch.
- `gmake -j$(sysctl -n hw.ncpu) verify` passed: the canonical ROM SHA-1 is `507341c0a40ca3e9a7cee969b396ee53facfb548`. Independent extraction of the canonical assembly-fallback range `0x185EFEC..0x1860398` also compared equal to the baserom. This validates preservation of the fallback, not promotion of the candidate C.
- `gmake check-nonmatching-builds` passed for all 243 candidate-bearing translation units. `gmake cleanroom`, `gmake check-docs` and `git diff --check` passed. The documentation gate rechecked atlas/alias projections, derived numbers, the pinned donor ledger and current structured handoffs.
- The committed change is limited to `src/overlays/o008/overlay_008.c` and this symbol's handoff. Source snapshots, configured objects, comparison summaries, first-mismatch evidence, hypotheses and rejected forms are retained in ignored `build/wb/o008-reconstruction/attempt-0` through `attempt-31`, with final evidence under `final/`; fresh drafts remain in ignored `m2cfiles/`.

Historical committed evidence from before this packet (measurements below describe those earlier candidates):

Mechanism-driven evidence from the original reconstruction base `4b4109b583ca0a37427e0f38f3fa0fbb14474220` and the size-near reopen base `dd11f1cf18fa33146780764dddbb2e0d5db379c8`:

- The authenticated overlay-8 text range is `+0x1294..+0x2640` (5,036 bytes, 1,259 words). Its only in-tree caller is the same overlay at `+0x6A4`; the call and preceding C establish the three-argument `void (O8P0058Owner *, O8P0058State *, f32)` ABI.
- The new `NON_MATCHING` body is a typed reconstruction of the complete control-flow draft, including the per-update loop, motion/angle integration, resource lifetime, selector flags, and color propagation. It compiles with the TU's configured `-O2 -mips2 -Wab,-r4300_mul` policy; the ordinary build continues to select the authenticated assembly fallback.
- All 137 shipped runtime records resolve to 63 concrete identities. The 15 calls are `overlay8GetIndexed`, local `func_overlay_008_F0001000_185ED58`, three `controlSetRumble`, `func_800299E8`, three `func_800031E8`, three `func_80002FE0`, `func_8002A204`, `mathDiffAngle`, and `func_800031C0`; the local `+0x1000` helper and all rumble calls take three arguments, not the former four-argument proxy forms.
- The former `D_0` source alias conflated three target identities: the motion scalar at reserved module `4095:+0x458C4`, impact gate at `4093:+0x31A4`, and final color gate at `4093:+0x31AC`. They now have separate typed roles. The 40 `D_F8..D_194` fields remain distinct lexical identities but are authenticated as one local target block at `+0x74A8..+0x7544`; expressing them through one C base is rejected because IDO commoned the address and removed 33 target instructions.
- Configured V0 now emits the exact 1,259-word target size in a `0x180` frame versus the target's `0xB0`. Retail's `temp_v0+0x14` path is explicitly `lwc1`, `trunc.w.s`, `mfc1`, and the hazard `nop`; the former integer proxy collapsed those four operations to one. Reusing the already-declared, non-overlapping `var_f0` carrier improves the direct-float form from 1,128 to 1,127 masked differences and reduces alignment gaps from 244 to 48. Raw differences are 1,129, first mismatch is `+0x0`, and the verdict remains `structure-mismatch`.
- The earlier strict gains were: authenticated callees/arity/return types (`1264 -> 1261` words, `0x1B8 -> 0x1B0`, `1223 -> 1211` differences); typed entry parameters (`0x1B0 -> 0x1A8`); direct resource fields (`0x1A8 -> 0x190`); and changing the capped `state+0x192` carrier from `u8` to `s32` (`1211 -> 1030` differences, 798 opcode mismatches). Removing four inert m2c homes reached the `0x180` frame without changing code bytes. The old 1,256-word/1,030-difference basin is retained only as an ignored diagnostic because its `s32` field read contradicts the target operations.
- The full 119-combination flag lattice found no exact result. Removing the TU-specific assembler scheduling flag reached exact size but still left 1,201 differences from `+0x0`; that flag is shared by the translation unit and is not a source-authentic promotion lever.
- Pinned DKR v77 `src/racer.c` supplies a permitted semantic skeleton for the top-speed, spin/drift, steering, sound, and particle phases. It confirms the absolute-speed cap/subtract/floor curve, drift multiplier 68 versus 58, reverse steering sign, signed spin state, rumble, and `+/-0x2EE` yaw clamp; JFG and exact-object donor scans remain negative.
- The linked promotion trial has no measured in-range or collateral differences because it stops on a 24-byte overlay text-size delta; it is not evidence of equality. Resume from the exact-size function by removing automatic homes without replacing explicit single-load carriers with repeated field reads: a prior 13-copy direct-access batch reached a `0x158` frame but regressed structurally. The target has nine non-save homes at `+0x44/+0x60/+0x68/+0x78/+0x84/+0x88/+0x8C/+0x94/+0x9C`; candidate still has `0xD0` excess frame. Do not run flags or generic permutation until the home set is structurally close.
<!-- plateau-handoff:func_overlay_008_F0001294_185EFEC:end -->
