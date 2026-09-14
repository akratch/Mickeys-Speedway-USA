# Whale load sharing after address reuse

Lane `wv-k`, 2026-09-14. **Retained: 169 masked / 340 raw differences,
14,456 owned bytes, delta zero, frame 0x138, first masked mismatch +0x50,
1,253 relocations.** No candidate beats 169 at target width. The guarded
implementation is unchanged; no executable bytes are promoted.

The [previous address-reuse witness](whale-address-reuse.md) reproduces
1,000 masked / 1,127 raw differences at target width. Its reset occupies
the target colour-call slot, and one position address serves restoration
and transition. The new controls recover both redundant tail reloads while
retaining that reuse, but lose twelve bytes and retain the changed draw
sequence. They do not recover the original title-loop offset-address sharing.

## Forwarding is recoverable without losing the position address

Attempt 1 uses the same converted signed lvalue for the comparison following
the compound update. The comparison consumes the computed value instead of
reloading the direct global. The store also moves into the comparison branch's
slot. This removes three instructions, not merely the two-word reload.

Attempt 2 saves the transition selector in `opponent` at its existing zero
test and uses that local for the later comparison with four. There is no
intervening call on this path, the position and selector have distinct
authenticated identities, and `opponent` is dead after the row loop. The
selector reload disappears, but its value now occupies a saved register.

Attempt 21 composes these two independently measured changes. Both reloads
disappear, and the same materialized position address still serves the
restoration store, transition load and transition store. The first colour
call and its reset remain exact at +0x1264/+0x1268. The composition is
**2,472 masked / 2,542 raw, delta -12, 1,252 relocations, 719 draws and
5,055 producer events**. It is a diagnostic, not an improved plateau body.

Its complete gaps are candidate +0x14E8 and +0x1630 against target
+0x12E8, +0x1348, +0x1358, +0x150C and +0x152C. The three early missing
offset-address words survive. Aligned rows are 2,258 exact, 747 naming,
236 immediate and 368 paired structural. The positional score is dominated
by displacement and must not be compared with the target-width witness as
though both had the same alignment.

## Directed address controls

All rows below start from the inherited reuse witness. Scores count differing
words; byte deltas exclude section padding. None restores the baseline draw
order. Each source, stock object, instrumented object, prepared input, context
check, procedure map, fidelity receipt and full residual is preserved privately.

| Attempt | Intervention | Masked / raw | Byte delta | Draws / events |
| --- | --- | ---: | ---: | ---: |
| 1 | Same converted lvalue in position comparison | 2476 / 2546 | -12 | 719 / 5057 |
| 2 | Preserve selector value across indirect store | 2475 / 2546 | -4 | 719 / 5055 |
| 3 | Converted address for every case-12 offset access | 2526 / 2592 | -28 | 718 / 5057 |
| 4 | Signed full-width position-address conversion | 1000 / 1127 | 0 | 719 / 5057 |
| 5 | Direct update destination, converted-address load | 2454 / 2528 | -8 | 718 / 5057 |
| 9 | Widened position-address conversion | 1072 / 1194 | 0 | 721 / 5060 |
| 17 | One-case selector switch with default | 1000 / 1127 | 0 | 719 / 5057 |
| 18 | Converted offset address only in title coordinate and capture loads | 3545 / 3550 | +24 | 723 / 5069 |
| 19 | Converted offset address only in restoration and decrement | 2459 / 2532 | +8 | 721 / 5058 |
| 21 | Compose attempts 1 and 2 | 2472 / 2542 | -12 | 719 / 5055 |

Signed and unsigned full-width address conversions have identical content
sections, relocations and symbols. Widening changes two draws at restoration
without repairing the gap list. The selector switch reproduces the original
witness. Neither is a route to recovering alias sharing.

The offset controls distinguish changes to reads from changes to stores.
Restricting conversion to the two earlier loads avoids adding indirect stores,
but changes allocation outside case 12 and adds six instructions. Restricting
it to the two later stores also fails. Thus the all-access control's failure
cannot be repaired simply by choosing either of these two subsets.

## The two smaller scheduling pairs

Separate controls use the retained 169 source. A value-producing grid reset
scores 170; an unsigned grid-row table index scores 173. Both retain target
width and the complete original draw order, but neither repairs the grid
preheader pair.

A block-local countdown walking coordinate reaches the target register choices
in the call-free node loop. It scores 176 at target width with unchanged draws:
one fewer aligned naming row, but a later generated spill moves from 0x64 to
0x60, introducing eight immediate differences. The guard-slot pair remains.
Appending the declaration to the function's locals reproduces this result;
inserting it in the middle changes many existing homes. Using existing `x`
instead preserves the spill home but adds naming costs elsewhere, scoring 172.
An `erase` carrier scores 211 and introduces another gap. A narrow coordinate
or an existing address-taken scalar each adds sixteen bytes and four draws.
Reversing the two preheader definitions is inert on both the original and
independent-coordinate sources.

The historical order climb required at least three statements. A final finite
audit tests the other nine independent two-statement runs, excluding calls,
escaped locals and cross-dependent assignments. Their scores in source order
are 169, 175, 171, 169, 175, 171, 170, 169 and 169, all at delta zero with
unchanged draw order and the original gap pair. None is adopted.

## Preservation and resumption

Thirty source cells were measured, in addition to unchanged baseline and
witness reproductions. The final three audit cells give 170, 169 and 169;
they add no improvement, gap repair or draw-order change. The directed
forwarding, address-representation and smaller emission controls are complete.
This is a scoped plateau, not an impossibility claim or a reason to repeat
the excluded split/pressure families.

The next concrete case-12 question is the early offset-address sharing in
attempt 21. It already retains the solved reset, position-address reuse and
both tail forwarding repairs. A further intervention must recover the early
sharing and original draw sequence together, without the global allocation
cost of attempt 18. Merely removing another reload or retaining the reset
does not meet that resumption test.

The fresh same-kind colour landscape contains 930 rows across 141 webs:
317 accepted target-width outcomes, 546 size changes and 67 other outcomes
(including webs without another same-kind colour). Seven singles beat 169;
the best measures 154. The suggested three-force packing predicts 141 but
was not compiled, so it is not a measured floor. These numbers belong only
to the unchanged 169 body, not to the source controls above.

Assignment returned `base-only`. The stock full-TU baseline, private copy,
prepared-input capture and instrumented output pass section, relocation and
symbol fidelity. Every source cell passes comparison against the baseline's
compiler-expanded declaration context and a named Ucode/index procedure map.
The current preflight resolves 860 candidate identities, leaves 393 unresolved,
and reports 1,237 offset/type and 673 stable-identity agreements out of 1,253
records per side. These are partial relocation proofs, not exactness.

The fresh-lane link failure was repaired with `gmake overlay-syms`; no tracked
alias change remains. Closing gates use `tools/gates.sh`. ROM verification
exercises the assembly fallback. The private packet is
`matching-evidence/wv-k-load-sharing-20260914.tar.gz` under Git's common
directory; it holds measurements, commands, sources, objects, traces and
validation receipts. Nothing is merged or pushed by this lane.
