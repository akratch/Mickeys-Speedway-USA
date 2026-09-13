# Whale address reuse and instruction accounting

Lane `wv-j`, 2026-09-14. **Retained: 169 masked / 340 raw differences,
14,456 bytes, delta zero, frame 0x138, first masked mismatch +0x50,
1,253 relocations.** The improvement from 172 is committed as
`3f957fc384de02424e7c4202190a49398977299b`. The function remains guarded
`NON_MATCHING`; no executable bytes are promoted.

There are two results: another ordinary source placement repairs three
instruction words, and a separate case-12 control reaches the solved reset
slot **and** later address reuse at target width. That control has substantial
collateral and does not replace the better retained source.

## The banked three-word repair

In case 10's empty-record arm, place `portraitIndex = 0x4A` immediately
after `sprintf(&text[0], D_o058_5DB8)`. The assignment then occupies the
target call slot at +0x34F8. The local's address does not escape, neither call
argument reads it, and the assignment precedes every subsequent use. Calls,
argument values, global accesses and per-path behavior are unchanged.

Exactly three executable words change, at +0x34EC, +0x34F0 and +0x34F8;
each equals the corresponding target word directly. The format-string
`R_MIPS_LO16` moves from +0x34F0 to +0x34EC with its instruction. Its identity
is unchanged; every other candidate relocation record is unchanged. This is
not the previous lane's stronger claim that all changed words are outside
the relocation surface.

The retained source has 3,476 aligned exact rows, 128 naming rows, one
immediate row and eight paired structural rows. Its sole gap pair remains
candidate +0x12F8 against target +0x1260. All 720 temporary draws retain
their complete order; producer events remain 5,072. The source is compiled
and locally improved, not object-exact or C-linked/ROM-exact.

## What the four-byte witness actually spends

Rebasing the previous lane's value-producing reset and independent title
induction onto the entry source gives **2,364 masked / 2,440 raw**, 14,460
bytes, frame 0x138 and 1,255 relocations. The reset at +0x1268 equals the
target. Adding the banked portrait repair preserves those positional scores
and width; the displaced later stream explains why the local repair is not
visible as three positional points on this witness.

The following accounting compares the entry 172 body with that rebased
witness. Physical source lines are preserved. Producer events come from
`draw_census`; final word attribution comes directly from the stock object's
line information, cross-checked against the preserved scheduler trace.
Event totals include directives, aliases and macro producers.

| Source line | Operation or inherited line label | Producer events before/after | Final words before/after | Word delta |
| --- | --- | ---: | ---: | ---: |
| 645 | First colour call and early reset | 9 / 10 | 7 / 8 | +1 |
| 651 | Late reset replaced by independent induction initialization | 2 / 2 | 1 / 0 | -1 |
| 656 | Title-loop increments; extra line directive | 3 / 4 | 2 / 2 | 0 |
| 689 | Row-loop exit and hoisted coordinate addresses | 11 / 10 | 10 / 8 | -2 |
| 692 | Restore saved position | 4 / 3 | 2 / 3 | +1 |
| 693 | Restore saved offset; one fewer alias event | 5 / 4 | 2 / 2 | 0 |
| 696 | Transition guard; one fewer alias event | 7 / 6 | 3 / 3 | 0 |
| 698 | Transition selector; three fewer alias events | 8 / 5 | 3 / 3 | 0 |
| 712 | Return and following transition-entry address setup | 3 / 4 | 2 / 4 | +2 |

The reset's +1/-1 cancels. The address change is **-2 +1 +2 = +1 word**:
the row exit stops preparing the shared position address, restoration needs
its own direct-address prefix, and the transition reconstructs the full
address. All other final line counts agree. The additional event at line 712
is an address macro producing two instructions; it does not mean that the
source `return` itself gained two operations. The capture at line 660 changes
neither draws nor emissions.

There is a second useful accounting view. Both objects contain **3,576
nonzero owned words**. The baseline has 38 no-ops and the witness 39;
section padding is excluded. The baseline's selector uses a branch-likely
slot containing a duplicated transition load. The witness loses that fill,
leaves a no-op at +0x154C, and keeps one copy of the load in the transition
body. Thus the address producers gain one nonzero word, the duplicated load
loses one, and the empty slot adds one no-op. Removing an arbitrary producer
event is not a four-byte repair.

The rebased witness's complete gap lists are candidate +0x1514, +0x15C0,
+0x15C4 and +0x1C84 against target +0x1508, +0x152C and +0x1C78. These are
alignment edit locations, not necessarily the identities of the instructions
whose emission moved.

## A new address-reuse witness, with its costs

The new intervention converts the coordinate's **address**, through the
configured ABI's full-width unsigned integer, back to its original signed
pointer type. It leaves the signed value accesses and their timing intact.
It changes neither the capture nor either title induction. This is separate
from the previously exhausted unsigned-value capture experiments and the
display-list address experiments.

Attempt 3 applies that representation only to the restoration and transition
compound update:

```c
*(s32 *)(u32)&D_o058_5E9C = savedPosition;
/* Existing intervening control flow. */
*(s32 *)(u32)&D_o058_5E9C += arg0 * 0xF;
```

The exact reset remains at +0x1268. A single materialized position address
now serves restoration, the later transition load and its store. The selector
again has the branch-likely load fill. Owned width is exactly 14,456 bytes.
This is a source reachability witness for the requested pair of properties.

It is **not an admissible improvement**: 1,000 masked / 1,127 raw differences,
1,254 relocations, 719 draws and 5,057 producer events. The direct transition
comparison reloads the global after the indirect store; the compiler also
reloads the selector for its following comparison. Earlier title-loop sharing
changes and the complete draw order differs. Target width includes those
compensating costs, rather than proving target shape.

Its aligned residual is 2,715 exact rows, 843 naming, 18 immediate and 33
paired structural rows. Complete gaps are candidate +0x14E8, +0x15C4,
+0x15CC, +0x15D0 and +0x15D8 against target +0x12E8, +0x1348, +0x1358,
+0x150C and +0x152C. Relative to the rebased witness, the tool's aligned
comparison adds 171 paired differing rows overall, including 16 in the
+0x1200 window and 35 in +0x1400. No register cycle is interpreted as a
temporary-pool phase proof.

## Directed controls

Attempt numbers count material source cells, excluding unchanged baseline and
witness reproductions. All case-12 controls include the banked portrait fix
and retain the exact reset at +0x1268. Every frame remains 0x138.

| Attempt | Source intervention | Masked / raw | Byte delta | Relocations | Draws / events |
| --- | --- | ---: | ---: | ---: | ---: |
| 1 | Default portrait assignment after call; retained | 169 / 340 | 0 | 1253 | 720 / 5072 |
| 2 | Converted address at restoration and all later position uses | 2457 / 2531 | -16 | 1250 | 719 / 5057 |
| 3 | Converted address only at restoration and compound update | 1000 / 1127 | 0 | 1254 | 719 / 5057 |
| 4 | Invert transition selector and exchange its two arms | 2359 / 2435 | -4 | 1255 | 720 / 5072 |
| 5 | Attempt 3 with a plain direct load on the update RHS | 2485 / 2557 | +4 | 1254 | 719 / 5062 |
| 6 | Converted address only at restoration | 2317 / 2397 | +8 | 1255 | 721 / 5069 |
| 7 | Converted address only at compound update | 2351 / 2428 | +4 | 1255 | 721 / 5072 |

The two-site ablation is complete: neither site, each site separately, and
both sites have measured controls. Broader tail conversion, a direct-load
RHS, and reversed successor ordering supply additional negative controls.
Neither the scalar score nor target extent conceals their replacement gaps.

The packet stops after banking the qualifying 169 result and completing this
directed experiment. This is a scoped evidence stop, **not a claimed
three-attempt stall or an impossibility result**. The new address witness
does not authorize repeating the excluded capture/pressure families. A further
packet needs a new mechanism to retain attempt 3's reuse while preserving
the original global-load sharing and draw sequence. Use attempts 2/3/5 as
the address-versus-load controls; merely retaining width or the solved slot
is now an insufficient resumption test.

## Landscape and proof receipts

Before any allocation interpretation, `web_footprint --every-colour` ran on
the unchanged 172 source: 930 rows across 141 webs, including nine without
a second same-kind colour. Of the remaining rows, 317 have accepted target
size, 546 change size and 58 have declined/unverifiable force receipts.
Seven same-kind singles improve 172; the best measures 157. Cross-kind
forces were not included, no combination was measured, and no forced floor
is claimed. That landscape is historical after attempt 1 changes the source;
none of its web numbers is used to explain a new source control.

The assignment gate returned `base-only`. Stock full-TU, private-copy,
prepared-input capture and instrumented objects pass content-section,
relocation and symbol fidelity. The actual asm-processor input still has
conditional directives, which the current context checker refuses; expanding
it with the same compiler preprocessor and flags supplies the accepted
self-context and per-candidate context comparisons. The failed raw-input
check is retained, not relabeled as a pass. Named Ucode and complete index
captures map each measured source to procedure zero.

Fresh retained-source preflight reports 1,253 runtime/static records on each
side, 1,237 offset/type agreements and 673 stable identity agreements. It
resolves 860 candidate identities and leaves 393 unresolved. This is partial
relocation evidence, not an exact-identity proof. The fresh-lane alias failure
was repaired with `gmake overlay-syms`, without a tracked alias change.

`finalize_plateau.py` banks the guarded 169 source and handoff. The initial
long summary exceeded the shard's existing size limit; shortening that
summary fixed the gate without changing its threshold or removing history.
Gates run through `tools/gates.sh`: ROM verification, cleanroom, check-docs,
check-scoreboard and check-tooling pass. The ROM proof exercises the assembly
fallback. No diagnostic is promoted, merged or pushed.

Private packet: `matching-evidence/wv-j-address-reuse-20260914.tar.gz` under
Git's common directory. It preserves source/object pairs, exact prepared and
expanded inputs, hypotheses, scores, first mismatches, complete residuals,
draw and scheduler evidence, the historical landscape, procedure maps,
fidelity receipts, gate logs and reproduction scripts. Instruction listings,
raw traces and toolchain captures remain private.
