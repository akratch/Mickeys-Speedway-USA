# Attempt 21: width and draw-order accounting

Lane `wv-l`, 2026-09-14. **Retained: 169 masked / 340 raw differences,
14,456 owned bytes, delta zero, frame 0x138, first masked mismatch +0x50,
1,253 relocations.** No candidate beats 169 at target width. The guarded
implementation is unchanged; no executable bytes are promoted.

The [wv-k composition](whale-load-sharing.md) reproduces **2,472 masked /
2,542 raw, delta -12, 719 draws and 5,055 producer events**. Its two tail
forwarding repairs and shared position address are intact. The twelve-byte
deficit is precisely three missing early offset-address instructions; the
remaining changes cancel in final word count.

A new restoration-order witness recovers the **entire original sequence of
720 draws**, retains the solved colour-call reset and later position-address
reuse, and reduces the deficit to four bytes. It does not recover early
offset sharing and is not an improved plateau body. The original draw order
is therefore reachable on this composition, but it is not sufficient for
the requested joint repair.

## Where the twelve bytes go

The table compares the retained 169 body with unchanged inherited attempt 21.
Physical source lines are preserved. Producer counts come from `draw_census`;
final word counts come from the stock object's source-line information within
its owned symbol extent. Neither section padding nor directives count as final
words. Instrumented content, relocation and symbol fidelity passes.

| Line | Operation | Draws before/after | Events before/after | Words before/after | Word delta |
| --- | --- | --- | --- | --- | --- |
| 645 | First colour call and early visible-index reset | 1/1 | 9/10 | 7/8 | +1 |
| 647 | Second colour call and hoisted offset address | 1/1 | 10/9 | 9/7 | -2 |
| 650 | First title coordinate | 2/2 | 9/8 | 11/12 | +1 |
| 651 | Late visible reset replaced by independent induction initialization | 0/0 | 2/2 | 1/0 | -1 |
| 654 | Title-loop preheader | 0/0 | 5/4 | 2/2 | +0 |
| 655 | Title draw | 1/1 | 13/10 | 8/8 | +0 |
| 656 | Title-loop increments | 0/0 | 3/4 | 2/2 | +0 |
| 657 | Title-loop tail and post-loop address setup | 0/0 | 11/10 | 12/10 | -2 |
| 689 | Row-loop tail and hoisted restoration addresses | 3/3 | 11/9 | 10/6 | -4 |
| 692 | Restore position | 1/1 | 4/4 | 2/4 | +2 |
| 693 | Restore offset | 1/1 | 5/3 | 2/3 | +1 |
| 695 | Decrement offset | 3/2 | 7/6 | 6/6 | +0 |
| 696 | Offset sign guard and selector address | 1/0 | 7/4 | 3/3 | +0 |
| 698 | Selector zero test | 0/0 | 8/5 | 3/3 | +0 |
| 715 | Position compound update | 2/2 | 5/4 | 3/3 | +0 |
| 717 | Selector comparison with four | 0/0 | 5/4 | 4/4 | +0 |
| 730 | Selector-to-state copy | 0/1 | 2/4 | 2/3 | +1 |
| 732 | Selector clear | 0/0 | 3/2 | 1/1 | +0 |

The reset's +1/-1 cancels. Early offset sharing contributes **-2 +1 -2 =
-3 words**: the title preheader loses a full address materialization, its
coordinate load gains a direct-address prefix, and the post-title setup loses
another full address materialization. This accounts for all twelve bytes.

The tail contributes **-4 +2 +1 +1 = 0 words**: the row exit stops preparing
two full addresses, position restoration creates one, offset restoration gains
a direct-address prefix, and the later selector-to-state copy now reloads the
selector. All other final line counts agree. In particular, fewer events at
the decrement and sign guard do not mean fewer final instructions there.

The complete totals are **-17 producer events, -1 draw and -3 final words**.
The converted offset read controls below show why restoring a count alone is
insufficient: each one-site control reaches target width with three additional
draws and different early sharing.

## Why the draw sequence changes, and what repairs it

At the offset decrement and following sign guard, baseline ugen takes three
and one draws respectively; attempt 21 takes two and zero. Its raw free-list
trace records `MOVE_END` on the reused offset values instead of the two omitted
load-result draws. The later selector-to-state copy adds one draw back. The
first unequal element of the complete draw-register sequence is index 312
(zero-based), in the transition tail. This attribution comes from actual
free-list events, not from interpreting a closed register cycle as FIFO order.

Attempt 7 exchanges only the adjacent offset and position restoration stores.
They refer to authenticated disjoint objects and cross no call or dependent
read. The offset decrement then reloads its global after the indirect position
store. That adds two final instructions and one draw. The full sequence now
matches all 720 baseline draws, while the deficit improves from twelve to four
bytes. The original reset remains at +0x1268; the single position address still
serves restoration, transition load and transition store.

The per-line draw census is **not** identical: the sign-guard draw remains
absent and the final selector-copy draw remains added. Equal complete register
order therefore does not establish an equal emission schedule. The reload is
a real compensating cost, not a shipping repair.

Compared with attempt 21, the aligned residual falls from 1,351 paired rows
to 762, with no change in the early windows through +0x1200. The +0x1400
window adds three paired rows while later windows improve. Its complete gaps
are candidate +0x14EC, +0x14F0 and +0x1644 against target +0x12E8,
+0x1348, +0x1358 and +0x1518. The three early missing words remain.

## Directed source controls

All rows below use untouched configured compiler output. Every frame is
0x138. Scores are differing words. Each source/object, hypothesis, full aligned
map, census, context check and named procedure receipt is preserved privately.

| Attempt | Intervention | Masked / raw | Byte delta | Draws / events | Original draw order |
| --- | --- | ---: | ---: | ---: | --- |
| 1 | Convert both early offset reads on attempt 21 | 3512 / 3517 | +12 | 723 / 5062 | no |
| 2 | Explicit early offset pointer through existing cursor | 2511 / 2579 | -12 | 721 / 5058 | no |
| 3 | Consistent converted offset restoration, decrement and test | 2470 / 2541 | -20 | 718 / 5057 | no |
| 4 | Use captured selector for final state copy too | 2475 / 2545 | -12 | 718 / 5054 | no |
| 5 | Use dead savedOffset for selector zero/four tests | 2458 / 2532 | -8 | 722 / 5058 | no |
| 6 | Direct position pointer from restoration through transition | 2347 / 2424 | +4 | 718 / 5066 | no |
| 7 | Exchange the two restoration stores | 2483 / 2553 | -4 | 720 / 5056 | yes |
| 8 | Restore position after the offset decrement | 2481 / 2552 | -4 | 720 / 5056 | no |
| 9 | Compose direct position pointer and exchanged stores | 2334 / 2415 | +8 | 718 / 5066 | no |
| 10 | Convert only the named position-pointer definition | 2483 / 2553 | -4 | 720 / 5057 | yes |
| 11 | Convert only title-coordinate offset read on attempt 7 | 1149 / 1260 | +0 | 723 / 5059 | no |
| 12 | Convert only saved-offset capture on attempt 7 | 1157 / 1269 | +0 | 723 / 5059 | no |
| 13 | Consistent converted offset tail with exchanged stores | 2471 / 2541 | -12 | 721 / 5061 | no |

The direct position pointer in attempt 6 restores the early shared offset
address, but the compiler reconstructs the position address later, losing the
required reuse. Combining it with the order witness does not restore the draw
order. Converting the named pointer only at its definition (attempt 10)
reproduces attempt 7's content sections, relocations and symbols exactly;
one extra producer directive does not change executable output.

The two single-read controls reach target width but add three draws: attempt
11 spends two extra at the title coordinate and one at saved-offset capture;
attempt 12 spends one and two. Both also change earlier case-2 emissions.
The final ordered, consistent-address composition still loses width and order.
No source control achieves the joint condition or improves the retained 169.

## Landscape and validation

The exhaustive same-kind landscape on attempt 21 contains 926 entries across
140 webs: 7 accepted target-width outcomes, 854 size outcomes and
65 other outcomes (including no-second-colour and declined receipts). The
best target-width single is 1,220 masked, far above 169. Its unforced baseline
has delta -12, so the tool's positional footprint differences compare unequal
geometry; no additive packing or forced floor is claimed. No force is adopted.

The assignment gate returned `base-only`. Fresh configured, private-copy,
prepared-input, indexed and instrumented builds reproduce the baseline and
attempt 21. The compiler-expanded baseline accepts self-context comparison;
every source control preserves that context and maps to procedure zero through
its named Ucode input and complete procedure index. Partial preflight retains
1,253 relocation records per side, 1,237 offset/type agreements and 673 stable
identity agreements, with 860 candidate identities resolved and 393 unresolved.
These are not exact relocation or C-linked ROM proofs.

## Preservation and next action

This is a scoped end to the requested composition experiment, with the fallback
line-by-line accounting delivered, not an unrestricted impossibility claim or
a claimed three-attempt information stall. Thirteen source controls give no
adoptable improvement. The known split-pressure and title-induction families
were not reopened. The final three controls all fail the required joint
width, sharing and order condition; their measurements remain available.

A further authorized packet should start from attempt 7, whose literal source
already recovers complete draw order and position reuse. Its concrete remaining
problem is restoring early offset sharing while removing the compensating
offset reload. Repeating the two converted-read subsets, direct-position-pointer
composition, or paired-address store order will reproduce measured failures.
Do not infer success from 720 draws or target width alone.

`finalize_plateau.py` preserves the guarded 169 body and concise handoff.
The long first summary exceeded the handoff shard's existing size limit;
shortening it fixed that failure without deleting history or changing a gate.
The detailed accounting lives here. `tools/gates.sh` passes verify, cleanroom,
check-docs, check-scoreboard and check-tooling. ROM verification exercises the
assembly fallback.

Private packet: `matching-evidence/wv-l-attempt21-20260914.tar.gz` under Git's
common directory. It preserves every new source, stock object, draw trace,
complete residual, command and validation receipt. Its inherited wv-k packet
is referenced separately rather than duplicated. No instruction listings,
raw traces, compiler captures or absolute workstation paths enter Git.
