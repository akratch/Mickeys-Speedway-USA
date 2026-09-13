<!-- plateau-handoff:overlay13UpdateRecord:start -->
### `overlay13UpdateRecord` plateau handoff

- source: `src/overlays/o013/overlay13ProcessRecord.c`
- score: 96/161 words
- frame: 0x20
- relocations: 5
- first mismatch: +0x2C
- summary: SetLinkSlot ABI resolves all five relocations; retained numeric floor is NON_EQUIVALENT due to countdown/result snapshots, with corrected controls preserved.

- geometry: Target and configured C remain exact at `0x284`/644 bytes/161 words with `0x20` frame; the owned Overlay 13 range is `+0x284..+0x508`, ROM `0x186ED9C..0x186F020`, followed by `overlay13ProcessActive` with no padding.
- ABI/flags: The configured candidate is `s16 *overlay13UpdateRecord(Overlay13Record *, s32)` under overlay game-code `-O2 -mips2 -32` and the canonical symbol-redefine/trim postprocess.
- relocation proof: Target and candidate each contain five records with all five offsets/types aligned. Four stable/effective identities align; the `R_MIPS_26` call at `+0x20` remains unresolved because no canonical `overlay13Prepare` definition/alias or exact sibling witness exists.
- diagnosis: Workbench reproduces 96 raw/masked positional differences from `+0x20`, with 63 opcode differences, 31 register differences, and 92 alignment gaps.
- caller/donor result: One Overlay 13 runtime call at `+0x550` and the export authenticate the entry. Donors remain weak: Conker `func_151A6350` leads at 0.0588 Jaccard and supplies no source or relocation witness.
- maintenance result: Current proxy tooling yielded no genuine lever, so no C-body, flag, or permutation hypothesis was attempted under the authorization.
- next action: Preserve the fallback pending unique same-overlay call-proxy evidence or another explicitly authorized mechanism. Previously exhausted loop, result-type, pointer-lifetime, flag, and permutation families remain closed.

#### 2026-09-13, lane `j1`: call authentication and semantic counterexample

The current baseline is 161 words, frame 0x20, masked 96, first masked +0x2C:
86 aligned exact, 34 naming, two immediate and 45 structural rows including
six candidate-only and six target-only words. The old 65-word header was
stale and is corrected. Procedure 0 has 66 draws and 244 emissions; full-TU
stock/traced text is identical. Static tables have five candidate versus three
extracted target records. Runtime target ownership has five records.

The call at owned +0x20 is no longer an unknown same-overlay proxy. Mickey's
runtime table binds it to resident +0x32BF0 (canonical address 0x80033040), whose symbol
is SetLinkSlot. The canonical C defines a three-argument signature with
narrow unsigned tag/count fields. Replacing overlay13Prepare's four-argument
proxy declaration/call with that exact signature and symbol changes no owned
instruction, draw count, per-line count, emission total or draw order. This
source-authentic call correction is retained; no shared header is changed.
Fresh function_preflight.py is complete: all five candidate and runtime
records align in offset, type, stable identity and effective identity, with
static proof and no unresolved records. The former relocation blocker is closed.

Reading the target control flow also reveals a semantic deficit hidden by the
historical score. The fall loop's entry and backedge test the old tick value
before decrementing; the retained diagnostic decrements before each test.
With state one and one tick, for example, the target executes the fall body
once while the retained C skips it. The target also snapshots the remaining
ticks into the result during each fall iteration, which the retained C omits.
The retained numeric floor is therefore explicitly NON_EQUIVALENT, not a
semantically proved near-match. Its source comment now states that limitation.

Separate private corrections preserve each attempt. Correcting the entry
snapshot alone scores 141 and adds one word; correcting the backedge too
scores 147 at the same extent, with 87 exact, 34 naming, four immediate and
42 structural rows including eleven gaps. Adding the loop-result snapshot
scores 151 with two extra words, 87 exact, 35 naming, three immediate and 43
structural rows including twelve gaps. None changes total draws or draw order:
the guard corrections redistribute emission records and the result update
adds two. A further natural while-loop control retains 66 draws but grows by
four words and regresses; it also makes the gravity/target reads loop-local,
so it is not an adopted equivalence proof.

Stop early under ADR 0018 after the authenticated ABI correction and measured
countdown/result controls, alongside the prior loop/lifetime negatives.
Do not repeat the unresolved-proxy diagnosis: it is resolved by runtime
identity. The next action is to reconstruct the target's complete countdown
and return-value semantics before solving the coupled floating-point and
result/constant allocation schedule. Retain the guarded numeric floor and
the corrected private candidates as distinct evidence; no variant is exact.
No colour sweep, flag lattice or permutation was repeated. Commands: assignment
gate, runtime-table identity decoding, canonical callee/source inspection,
function_preflight.py (complete five-of-five identities),
configured stock/traced compilation, draw_census.py comparisons,
residual_map.py --object --against, finalize_plateau.py and tools/gates.sh.
ROM verification covers the retail assembly fallback only.
<!-- plateau-handoff:overlay13UpdateRecord:end -->
