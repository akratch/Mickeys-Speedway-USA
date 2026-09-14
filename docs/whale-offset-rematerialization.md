# Early offset sharing versus rematerialization

Lane `wv-m`, 2026-09-14. **Retained: 169 masked / 340 raw differences,
14,456 owned bytes, delta zero, frame 0x138, first masked mismatch +0x50,
1,253 candidate and target relocation records.** No new executable bytes
are promoted.

The restoration-order witness reproduces 2,483 masked / 2,553 raw,
delta -4 and the complete original 720-draw sequence. Its four-byte deficit
is a net count: **three early address words missing and two extra tail words**.
The [previous accounting](whale-attempt21.md) is confirmed. It does not mean
that adding one instruction, without other changes, restores target layout.

A new composition shares the offset address from the first title coordinate
through capture and restoration, retains the position address and the reset
slot, and reproduces **both every per-line draw count and the complete
720-draw sequence**. It scores 2,483 masked / 2,554 raw at delta -16.
It holds the offset in a saved register across calls, whereas the target
rematerializes its temporary address. General address reuse and original draw
order can coexist; the target's specific rematerialization pattern remains
unrecovered. This is not an improved plateau candidate.

## What the new composition proves

Attempt 2 carries the offset's address through the configured ABI's unsigned
integer conversion in the existing, dead `cursor`. Its signed loads and stores
cover the first title coordinate, saved-offset capture, restoration,
decrement, sign test and clamp. The row-rendering accesses remain direct.
The position-address conversion and exchanged restoration order are inherited
unchanged. The pointer does not escape, all addresses retain their original
identities, and call values/order and signed arithmetic remain unchanged.

This produces one saved offset address serving the early and late accesses,
but adds a guard producer load/draw. The assembler eliminates that load from
final output; the producer draw still affects the full sequence.

Attempt 13 composes this pointer with the already authenticated selector-local
copy: the final state assignment reads `opponent`, which already contains the
selector, instead of reloading the selector global. This branch crosses no
intervening call or selector store. Its per-line draw count now agrees with
the retained baseline at every line, and all 720 drawn registers agree in order.
The reset at +0x1268 is byte-identical to the target. The position address still
serves restoration and the transition load/store; no second materialization
is introduced on that path.

The new source has 14,440 owned bytes and 1,245 candidate relocation records.
Those are diagnostic geometry and partial identity evidence, not a linked
C or ROM-exact proof. The saved offset address replaces the target's two early
temporary-address materializations, including its post-title rematerialization.

## Final-instruction accounting

Counts are from stock full-TU objects within the owned function extent,
using their source-line information; directives and section padding are
excluded. Lines retain the inherited physical numbering.

| Region | Retained to inherited witness | Retained to attempt 13 |
| --- | ---: | ---: |
| Reset placement, lines 645/651 | 0 words | 0 words |
| Early offset geometry, lines 647/650/657 | -3 words | -3 words |
| Restoration and transition tail | +2 words | -1 word |
| Total | -1 word | -4 words |

The early composition is -2 +2 -3 words: no early hoisted temporary address,
one saved address at the coordinate, and no post-title rematerialization.
The tail is -4 +2 +1 -1 +1 words at lines 689, 693, 695, 697 and 717.
Attempt 13's selector copy returns line 730 to its original two-word count;
the preceding selector comparison gains a word. Removing a producer draw
therefore does not imply removing a final instruction.

## Directed source controls

All fifteen sources, stock objects, full residual maps, prepared inputs,
context checks, named procedure maps, instrumented fidelity receipts and draw
traces are preserved privately. Every source control retains the exact reset
slot. Scores count differing words, and every frame remains 0x138.

| Attempt | Masked / raw | Byte delta | Draws | Original complete order |
| --- | ---: | ---: | ---: | --- |
| 01 | 2523 / 2592 | -4 | 722 | no |
| 02 | 2486 / 2557 | -16 | 721 | no |
| 03 | 2456 / 2530 | +8 | 722 | no |
| 04 | 2455 / 2529 | -16 | 719 | no |
| 05 | 2523 / 2592 | -4 | 722 | no |
| 06 | 2513 / 2585 | -4 | 721 | no |
| 07 | 2513 / 2585 | -4 | 721 | no |
| 08 | 2471 / 2542 | -4 | 719 | no |
| 09 | 2485 / 2554 | -4 | 720 | yes |
| 10 | 2496 / 2565 | -12 | 722 | no |
| 11 | 2486 / 2557 | -16 | 721 | no |
| 12 | 2479 / 2552 | -16 | 719 | no |
| 13 | 2483 / 2554 | -16 | 720 | yes |
| 14 | 2521 / 2591 | -4 | 721 | no |
| 15 | 2487 / 2558 | -16 | 722 | no |

1. Extend a transparent offset pointer through the early loads and tail. It
   folds to direct global accesses and adds two draws; no early sharing returns.
2. Convert that pointer at its definition. It keeps a shared saved address,
   changes the guard draw and loses sixteen bytes.
3. Convert only the witness's offset sign guard. It adds three final words and
   two draws without changing the early address geometry. The ownership
   preflight correctly rejects its eight-byte overrun; no gate is weakened.
4. Use `savedOffset` as the decrement RHS. The compiler deletes the earlier
   three-word offset restoration, which has become dead, rather than merely
   removing a two-word reload. The complete draw order is lost.
5. Use a native case-local signed pointer instead of the existing `void **`
   carrier. The case-12 outcome is unchanged from attempt 1; other naming
   differences mean the complete objects are not identical.
6. Sequence the original left-associated partial coordinate into `columnX`
   before adding the indirect offset. This removes one coordinate draw,
   without repairing early geometry.
7. Exchange the independent capture assignments on attempt 6. The executable
   result is unchanged; capture emission order is not the remaining lever.
8. Use the direct global at capture on attempt 6. It removes the capture draw
   and one earlier coordinate draw, leaving 719 draws.
9. Restore the single coordinate expression on attempt 8. Original complete
   draw order returns at delta -4, but early rematerialization is still absent.
10. Save the compound-update value into `savedOffset` for the guard. This adds
    a result draw and a final word rather than eliminating the guard draw.
11. Test the compound-update expression directly. It reproduces attempt 2's
    executable result; one fewer directive is not a code or draw repair.
12. Explicitly compute the offset in dead `opponent`, store it, then test the
    scalar. Both the redundant guard draw and one result draw disappear,
    leaving 719 draws at unchanged width.
13. Compose the shared pointer with the selector-local final state copy.
    Both the original complete order and every per-line draw count return.
14. Move the pointer's address conversion to its tail uses. The conversion
    folds away there; early sharing is lost again.
15. Keep the address in a case-local ABI-width integer until each dereference.
    It retains the saved-address geometry and adds two draws.

## Colour evidence

The inherited witness's fresh same-kind landscape contains 926 entries across
140 webs: 11 target-width outcomes, 850 size outcomes and 65 other outcomes.
Five target-width forces improve its unforced positional score; the best is
1,825. Its baseline has unequal geometry, so no positional packing is claimed
as an additive floor.

The fresh attempt-13 landscape includes both save kinds: 1,908 entries
across 142 webs, with 58 target-width outcomes, 1,623 size outcomes and
227 other outcomes. Of the target-width outcomes, 49 beat its unforced
positional score; the best is 778. None reaches 169. The unforced object
passes content-section, relocation and symbol fidelity against the source cell.

Its shared offset address is web 375. The accepted `p1:w375=c7` diagnostic
puts it in the target temporary register but inserts three stack save/reload
pairs around calls. Those six words, less one other word in the row loop,
add twenty bytes: delta -16 becomes +4. It still loads the preserved pointer
rather than rematerializing the target constant address. The per-window
aligned comparison improves twenty paired rows over +0x1200 to +0x1700,
but introduces new gaps. No forced object is a source improvement.

All 1,908 measurements and the complete JSON were written before the optional
text renderer began enumerating subsets of its 49 winners. That enumeration
was stopped after 115 seconds; the process exits 143, not success. The saved
JSON and plain winners summary are complete. No additive prediction is used
against the unequal-geometry baseline. Future exhaustive runs can use `--json`
to avoid this optional rendering step. Interrupted setup runs are labelled
and excluded from these measurements in the private packet.

## Scope and preservation

The scoped address-representation experiment is closed without an admissible
improvement. Transparent pointers, conversion placement, native pointer and
integer carriers, coordinate/capture scheduling, and explicit guard dataflow
have measured controls. The fresh single-force landscape supplies no qualifying
allocation repair. The already-excluded flag, title-induction and split-pressure
families were not reopened. This is a scoped exclusion, not an unrestricted
impossibility proof or a claimed three-attempt information stall.

In particular, the new witness prevents claiming that generic early address
sharing and original draw order cannot coexist. What remains unproved is the
joint target-specific condition: rematerialized early offset addresses, reused
late position address, exact reset placement, original draw order and target
width. Retain the original restoration-order witness as the anchor; attempt 13
is a counterexample to an overbroad incompatibility claim, not a better starting
score. A further packet needs a new route to known-address rematerialization,
not another equivalent pointer cast or a colour force that preserves the pointer
through stack traffic.

Fresh configured and private-copy baseline builds reproduce 169 masked / 340
raw at delta zero. The compiler-expanded baseline accepts self-context
comparison, and every source control passes context and named procedure-zero
mapping plus stock/instrumented/indexed fidelity. Retained-source preflight
proves 1,237 offset/type and 673 stable-identity agreements across 1,253 records;
393 candidate identities remain unresolved. These are partial relocation proofs.
Workbench `diagnose` reports `structure-mismatch`; its generic `constant-audit`
footer does not establish an incorrect source constant. The retained lever is
the measured address-rematerialization distinction above, not a literal change.
The initial fresh-lane link failure was repaired with `gmake overlay-syms`;
no tracked alias changes remain. Oversized attempt 3 remains boundary-rejected.

`finalize_plateau.py` preserves the guarded 169 body and concise handoff.
`tools/gates.sh verify cleanroom check-docs check-scoreboard check-tooling`
passes all five gates with their true exit statuses retained in the private
packet. ROM verification exercises the assembly fallback, not this C body.

Private packet: `matching-evidence/wv-m-offset-sharing-20260914.tar.gz` under
Git's common directory. The inherited wv-l packet is referenced separately,
not duplicated. All instruction listings, traces, objects and compiler captures
stay outside tracked files. No source control is merged or pushed.
