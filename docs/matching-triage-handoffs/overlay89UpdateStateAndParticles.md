<!-- plateau-handoff:overlay89UpdateStateAndParticles:start -->
### `overlay89UpdateStateAndParticles` plateau handoff

- source: `src/overlays/o089/overlay89UpdateStateAndParticles.c`
- score: 98 differing words
- frame: 0x90
- relocations: 14
- first mismatch: 0x0
- summary: Parameter-home and guarded-count controls are byte-flat; early z is 97 positional but worse aligned; retained 98 and frame 0x90.
#### 2026-09-13, lane k1: authenticated draw-census follow-up

Fresh configured stock compilation reproduces 544 target bytes,
size delta 4, 98 raw and
98 masked differences, first +0x0.
Candidate frame is 0x90; the procedure-0 census records
29 draws and 230 emission records. Stock and traced full-TU text
compare identically. Static relocation counts are 14 candidate and
12 extracted target, with 0 identical offset/type/symbol tuples.
These are static measurements; overlay runtime identity proof remains separate.

Baseline paired buckets are 53 exact, 37 naming, 20 immediate and 22
structural, with five candidate-only and four target-only words. The candidate
has 137 words and frame 0x90; target geometry is 136 words and frame 0x88.

Three independent source hypotheses were measured after reading the source
and shard. First, replace the local volatile parameter with ordinary s32 and
spell both reads through its address. This tests L144's reload-versus-scheduler
edge distinction. The call and timer lines each lose two emission records,
while every draw and its sequence stay fixed. Full-TU text is byte-identical:
the parameter-home control does not reach the residual.

Second, move the z-coordinate initialization beside x and y, crossing only
nonescaping particle-local constant stores and preserving x/y/z load order.
The 29 draws and 230 emission records remain, but their joint GP/FP order
changes; each bank's own draw subsequence is unchanged. The positional score
improves to 97, but the paired aligned residual adds three naming and two
immediate rows. Four gap words disappear, leaving the total aligned residual
one row worse. This scalar improvement is not adopted.

Third, retain the original count into the second nonzero guard and decrement
inside that guard before the unchanged loop. The u8 count and outer nonzero
check prove identical iterations. Two count lines change emission attribution;
29 draws and their order are unchanged, and full-TU text is byte-identical to
baseline. It does not supply the target's count-transfer schedule.

All three source/object pairs have fresh named-Ucode mapping, stock/traced
full-TU fidelity and retained draw/aligned differentials. Stop early under
ADR 0018: these controls reject the identified parameter-home, coordinate-order
and guarded-count explanations, while the recorded aggregate/address/loop and
flag variants cover the remaining available forms. The frame, saved particle
address and post-call schedule remain unresolved. No colour sweep was repeated
and the original 98-word guarded baseline is restored.

The retained C remains guarded by its original assembly fallback. Sources, stock
and traced objects, frame and scalar measurements, draw profiles and aligned
maps remain ignored under build/k1/overlay89UpdateStateAndParticles.
Commands: lane_status.py, configured stock compilation, draw_census.py,
residual_map.py --object/--against where compared, finalize_plateau.py and
tools/gates.sh. No executable bytes are newly credited.

<!-- plateau-handoff:overlay89UpdateStateAndParticles:end -->
