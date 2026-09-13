# The last gap: instruction identity before line attribution

Lane `wv-h`, 2026-09-13. **Retained: 187 masked / 358 raw differences,
14,456 bytes, delta zero, frame 0x138, first masked mismatch +0x50.**
The guarded C body is unchanged. No new executable bytes are matched.

The baseline has 3,458 aligned exact rows, 134 naming rows, one immediate
row and 20 paired structural rows. The additional candidate-only and
target-only words bring the structural bucket to 22. The sole gap remains
candidate +0x12F8 against target +0x1260.

## The reset is at line 651, not the capture at line 660

The stock compiler listing attributes the visible `i = 0` to source line
651. Its final candidate location is +0x12F8. The target initializes that
same visible index at +0x1268, in the first title-colour call's delay slot.
The first call is source line 645; the coordinate calculation is line 650.
The savedPosition capture at line 660 follows the entire first title loop.
It is not the candidate end of this insertion pair.

The aligner's target-only offset +0x1260 is **an alignment edit location**,
not the identity of the displaced reset. The target's argument setup and
call have also changed order around that reset. Identify the operation from
the surrounding code before turning an aligned insertion into a source line.
Proportional scaling of the emission ordinal to final text offset does not
provide that identity.

The supplied trace confirms the source attribution: line 651 has two
producer records, a line directive and the reset emission. The first colour
call, subsequent title draw and second colour call precede it in ugen's
output. Moving only assembler line labels cannot remove those intervening
calls from the emission sequence.

This is the same early-initialization/induction constraint already recorded
under “The displacement is a fold constraint” in the
[per-symbol handoff](matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md).
That record separately tested constant propagation and induction-initial-value
analysis. Later independent-induction controls repaired the early region and
lost address reuse at the transition. The gap was therefore already being
investigated; the capture work addressed a cost of repairing it.

## Two measured emission controls

Both controls preserve physical line numbers outside the edited span and
use the configured full-TU command. Each has a retained source, stock object,
instrumented object, fidelity result, score, complete trace, draw comparison
and aligned residual map. Neither enters the guarded body.

| Control | Masked / raw | Byte delta | Draws | Emission records |
|---|---:|---:|---:|---:|
| Baseline | 187 / 358 | 0 | 720 | 5,072 |
| 1: early reset and postincrement array use | 2,475 / 2,546 | +12 | 722 | 5,072 |
| 2: fold call-through-reset span onto line 645 | 187 / 358 | 0 | 720 | 5,072 |

**Control 1** asks whether sequencing the visible increment in the title-array
argument avoids the separate induction needed by the earlier repair. It moves
initialization into the first colour call's zero-valued argument, uses
`D_o058_5C98[i++]`, and removes the later explicit increment. The original
defined input domain bounds the array index; i has no escaped address and
is not read by another argument. Calls, argument values and loop tests retain
their original behavior.

The reset actually moves from +0x12F8 to +0x125C, with attribution moving
from line 651 to line 645. It still does not occupy the target's delay slot.
Line 645 gains one emission record; line 651 loses two. The loop preheader
at line 654 gains two draws and two records; line 655 gains two records as
the increment moves into the call argument; line 656 loses three. Lines 650
and 660 do not change their counts. The generated cursor still needs the
initial-index calculation, and the loop's final schedule changes.

The old pair disappears but replacement gaps are candidate +0x1310,
+0x1314, +0x1328 and +0x132C against target +0x1338. Paired residual rows
are 814 naming, 200 immediate and 417 structural. This is not a gap closure
at target width. Equal total emission records did not imply equal executable
size: the function gained three instructions.

**Control 2** isolates physical-line scheduling from execution order. It folds
the statements from the first colour call through the reset onto line 645,
leaving blank lines through 651 so all subsequent positions stay fixed.
It changes no statement or evaluation order.

`draw_census --compare` reports 34 more records at line 645 and removes
13, 10, nine and two records from lines 646, 647, 650 and 651 respectively.
Seven draws are likewise reattributed to line 645. Nevertheless **all 5,072
producer records are identical in sequence after excluding only their line
field**. The complete draw-register sequence is unchanged. Stock content
sections, relocations and symbols are identical to baseline, and the
candidate reset remains at +0x12F8. Both original gap offsets and every
aligned residual window remain unchanged.

This directly separates a census line moving from an instruction moving.
The census remains useful, but its per-line differences need statement and
event-order attribution. Its emission total counts producer events, including
directives and alias records; it is not an executable-word total.

## Stop condition and handoff

This is an **evidence-based early stop**, not a claimed three-attempt stall or
an impossibility result for the function. The measured line-only control
cannot move the reset through the calls. The changed index-use spelling
repeats the already documented induction cost. The committed record already
rules out duplicating the reset and several nearby no-op definitions as a
way to retain early initialization and recover local induction folding.
Continuing with the supplied coordinate/capture pressure families would
repeat the explicitly excluded work.

A future packet needs a new mechanism that keeps early visible-index
initialization **and** a base-only title-array cursor, while retaining the
later address reuse. Require the reset's actual final location, the complete
gap list and target width. Merely changing the line-650/660 census is not a
resumption test.

Assignment gate: base-only. Fresh stock and private-copy baselines reproduce
187; the supplied trace object passes content, relocation and symbol fidelity.
The actual prepared compiler input passes self-context comparison. The fresh
analysis-only preflight reports 1,253 runtime/static records per side, 1,236
offset/type agreements and 671 stable identities, with 858 candidate
identities resolved and 395 unresolved. Relocation exactness is not claimed.
The generic workbench diagnosis remains a mixed residual with a heuristic
constant-audit recommendation; it does not establish this gap's source line.

Private evidence is preserved as
`matching-evidence/wv-h-gap-emission-20260913.tar.gz` under Git's common
directory. It includes both controls, the faithful baseline, prepared input,
stock listing, preflight, commands and comparison reports. All instruction
listings, objects and raw traces remain private.
