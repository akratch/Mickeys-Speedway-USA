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

## The reset reaches the slot: lane wv-i

Lane `wv-i`, 2026-09-13. **Retained: 172 masked / 343 raw differences,
14,456 bytes, delta zero, frame 0x138, first masked mismatch +0x50.** Five
already-early index definitions now occupy their target call slots. Exactly
15 non-relocation instruction words change, and all 15 become target-exact;
no other executable word changes. The complete relocation surface and all
720 temporary draws retain their original order. No function is promoted.

**Case 12's reset is also source-reachable at its exact target slot,
+0x1268.** That separate witness remains four bytes too long and introduces
later gaps. The retained 172 body therefore keeps case 12 unchanged: its
reset remains at +0x12F8, and its complete gap list is candidate +0x12F8
against target +0x1260. This is an improvement and a scheduling mechanism,
not the requested zero-gap result at target width.

### Availability was only the first barrier

The faithful stock `-Wa,-R` trace identifies the original slot occupant as
the fourth colour argument, a3 = 255. In the inherited 187 body, the first
colour call's scheduling block has seven instruction nodes: five argument
definitions, the outgoing fifth-argument store and the call. The reset is
absent; its producer follows three calls. The candidate call is at +0x1260
and its fourth-argument setup occupies +0x1264.

In the inherited postincrement control, the reset **is available and
independent**: it is an initial ready node with zero predecessors,
aftercycles and latency, and no successors. The fourth-argument definition
has the same recorded properties. as1 selects the reset at block-relative
+0x10, then the third argument at +0x14, the call at +0x18 and the fourth
argument at +0x1C. The reset ends at +0x125C; a3 occupies +0x1268.
No dependency edge makes that early reset unavailable. Its ready-list order
makes it execute before the eventual slot filler.

The fifth-argument constant/store chain is scheduled first, followed by the
zero-height definitions. Neither first-call trace hides a deleted no-op in
the slot: every produced instruction survives in the final block. A hidden
temporary draw or consumed no-op slot is not this call's explanation.

### The source lever is a value-producing argument

Attempt 1 put `(i = 0, 0xFF)` in the fifth argument and gave the first title
array an independent `portraitX` induction next to the loop. The hypothesis
was that the last argument would emit the reset last. It does the opposite:
the compiler emits the discarded comma assignment before all five argument
definitions. The reset is at +0x1254 and a3 still occupies +0x1268.

Attempt 2 changes only that argument to `0xFF - (i = 0)`. The value-producing
expression retains the reset after the four register-argument definitions,
even though the subtraction folds to the same constant. The reset still has
zero predecessors, aftercycles and latency in as1's graph; the changed
ready-list order now leaves it for the delay slot at **+0x1268**. Compared
with attempt 1, only five executable words change, all in this first call
block, with identical width and relocation records. The complete later output
is identical. This is a local scheduling repair, not a pressure intervention.

Both forms initialize the visible index early and retain a base-only title
cursor. Both indices advance once per iteration and have the original
bounded domain. The local's address does not escape, no other call argument
reads it, and every call value/order is preserved. The subtraction is defined
and evaluates to 255; it introduces no runtime arithmetic instruction.

Attempt 3 tests the new expression with the original late reset and shared
array induction still present. The early definition is deleted and the
complete output reproduces 187, including the late reset and gap. The new
argument spelling does not bypass the previously proved dead-definition and
induction-initial-value barriers.

Attempt 4 applies the successful last-argument spelling only at the five
sites whose visible indices were already initialized in the opening colour
call: cases 1, 3, 13, 8 and 10, in source order. Their array inductions and
all later lifetimes remain unchanged. Each call repairs three instruction
words, giving **172 at delta zero**. The reset slots are respectively
+0x140, +0xD64, +0x16A0, +0x25B0 and +0x3350. Every changed word is outside
the relocation surface and equals the corresponding target word directly.
The retained source uses:

```c
fontColour(0xFF, 0x80, 0, 0xFF, (0xFF - (i = 0)));
```

### Complete measurements

Scores are differing words; structural counts exclude unpaired gap words.
All cells preserve the baseline draw count and complete draw order.

| Cell | Masked / raw | Owned bytes | Exact / naming / immediate / structural | Gaps C/T | Emissions |
| --- | ---: | ---: | --- | --- | ---: |
| Baseline | 187 / 358 | 14,456 | 3458 / 134 / 1 / 20 | 1/1 | 5,072 |
| 1: discarded fifth-argument reset | 2375 / 2451 | 14,460 | 2876 / 196 / 230 / 308 | 5/4 | 5,068 |
| 2: value-producing fifth argument | 2370 / 2446 | 14,460 | 2878 / 196 / 230 / 307 | 4/3 | 5,068 |
| 3: retain original late definition | 187 / 358 | 14,456 | 3458 / 134 / 1 / 20 | 1/1 | 5,072 |
| 4: five existing early definitions | 172 / 343 | 14,456 | 3473 / 129 / 1 / 10 | 1/1 | 5,072 |

Attempt 1's complete gaps are candidate +0x1254, +0x1514, +0x15C0,
+0x15C4, +0x1C84 against target +0x1260, +0x1508, +0x152C, +0x1C78.
Attempt 2 removes only the first pair: candidate +0x1514, +0x15C0,
+0x15C4, +0x1C84 against target +0x1508, +0x152C, +0x1C78 remain. Both
have 1,255 candidate relocations. Baseline, attempt 3 and retained attempt 4
have 1,253 relocations and the single +0x12F8/+0x1260 pair.

An independent small C switch-arm experiment reproduces the comma versus
value-producing argument distinction. Otherwise identical function-entry
experiments do not, using either int or ABI-width long locals. The source
recipe therefore has measured context limits; it is not a universal promise
that assignment-expression syntax controls a slot.

### Preservation and resumption condition

The scheduling question is answered and the reusable gains are banked.
This is an early stop at the packet's remaining scope boundary, not a
three-attempt stall or a universal impossibility claim. Case 12's independent
induction still carries the known four-byte later-address-reuse cost.
The new spelling changes none of that later output. The supplied pressure
and composition witnesses introduce their own additional gaps and do not
provide an already-proved zero-gap source to receive this local repair;
their excluded searches are not repeated.

A further case-12 packet can start from attempt 2, where actual slot
placement is solved, and must retain it while recovering later address reuse
and removing every replacement gap at target width. Merely making the reset
available, moving it to another argument through a comma, or adding a second
late reset is no longer an open hypothesis. Recompute any colour landscape
on the chosen source: the supplied 187 resources are historical after the
retained body changes to 172; no forced floor for 172 is claimed.

The assignment gate returned base-only. Configured, private-copy and
instrumented objects pass content, relocation and symbol fidelity. The stock
scheduler captures used here pass that same gate. The actual prepared
baseline input passes self-context comparison; the source attempts preserve
its declaration context. Analysis-only preflight has partial relocation
evidence, not an exact-identity or promotion proof. ROM verification uses the
assembly fallback and does not promote the nonexact C.

`finalize_plateau.py` keeps the candidate guarded and updates its handoff.
The complete private packet is
`matching-evidence/wv-i-delay-slot-20260913.tar.gz` under Git's common
directory. It retains every source/object, complete gaps, scheduler and draw
traces, context/fidelity receipts, standalone controls and gate logs. A manual
listing-reassembly experiment failed fidelity and remains failed diagnostic
evidence; no conclusion depends on it.
