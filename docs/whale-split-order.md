# The identity, itemised: lane wv-u

**Retained: 18 masked / 195 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x12E4, 1,253 owned text relocations.**
Unchanged from wv-s. The C remains guarded `NON_MATCHING`. This file is a
checkpoint written mid-lane; the cycle ledger at the end is the part to
trust, and it will be extended.

## What this lane adds so far

wv-t read the two case-12 fragments as one growth identity and named the
next cycle: a web coloured `v1` before the `&D_o058_5E9C` remainder splits
at save 3.60. This lane itemised both sides of that identity from the
banked neighbour captures (`allocator18-detail-w1139.log`, both of W's
decisions, and `allocator18-detail-w1100.log`) and found the identity is
tighter than the dispatch assumed:

- **The two pieces have the same window.** In {183, 184, 185, 190} the
  183-seeded `&D_o058_5E9C` piece counts 24 interferers at its decision and
  W counts 23 at its own; the one extra is W's *own* lineage remainder
  (`w1096`), which never counts for W. Every other member is shared: the
  lineage remainders are renumbered between the two decisions (931 -> 1118,
  926 -> 1187, 964 -> 1191, 38 -> 1178, 0 -> 1179) but none leaves the
  window, and no piece dropped between the two decisions was a member of
  either list. There is no transient interferer to exploit.
- **The held colours differ by `v1`, `a0`, `a1` only**, all three decided
  between the two (w40 at 137, w251 at 172, w222 at 188). `a0`/`a1` are
  folded by the call block at 185 for both pieces, so `L1 = L2 - 1` exactly
  as wv-t wrote, and the margins are what they are: on the 18 body W fails
  185 by one colour; on a carrier-less body (48, ptr, fresh `k`) the piece
  passes 191 or 202 by one and W has margin 0 at 202.
- **Consequence: the target cannot carry a dead carrier web at 183.** With
  `L1 = 11` W fails 202 even at `N1 = 22`. So the target is the 48
  configuration on the allocator side, and the open question is the source
  shape that puts the reset in call 1's delay slot with a constant cursor
  init and *no* extra interferer at 184: the pointer form's `&D_o058_5C98`
  remainder (`w1175`) is still in W's list at W's decision, so it is not a
  transient either.
- **The one remaining lever inside the identity is the order of `w40`
  (3.0) against the remainder's 183 step (3.60 / 3.32).** With `w40`
  decided first, the piece sees `v1` held and rejects 202 on a
  carrier-less body while W is unchanged. Measured both ways: the head
  probes (`if (n * 2);`) do raise `w40`'s save (3.5 with one, 4.0 with
  two) but each inserts two basic blocks before the preheader, which
  lowers W's own save (`nocs` 3, 0.67, decided at 302) and on the two-probe
  cell rematerialises W's head read outright; and lowering the remainder's
  save by moving `D_o058_5E9C` reads onto the adjacent `&D_o058_5E98`
  lineage (`(&D_o058_5E98)[1]`, same bytes after link) either does not
  reach 3.0 (case 2's loop: 3.20, and 39 rows of case-2 colour), splits
  the `D_o058_5E9C` value web (the head read: `w251`'s `a0` becomes a ring
  temp, +4 on every body), or poisons alias analysis (the row-loop negate:
  +969 aligned rows everywhere, though the order did flip and the piece
  then failed 185 by the same one colour).

## Instruments added (scratch, banked under `cells-wv-u/`)

- `readW.py LOG`: finds W by shape (the coloured `&D_o058_5EA0` piece whose
  refs are the head, the save block and the restore block), rebases block
  numbers on where 183 landed, and prints the growth of every piece seeded
  there plus the coloured webs in the window in decision order. Needed
  because probe cells shift the block map.
- `intfdiff.py LOG WEB WIN`: the interferer list at *each* decision of a
  `CDX_DETAIL_WEB` capture, restricted to a window, and the set difference
  between consecutive decisions. This is what itemised the identity.
- `peels.py LOG SYM`: the peel history of one lineage (seed, save, nocs,
  totalsave per decision).
- `cellcc.py` now gives `nm_ranking` a private work directory per process;
  parallel cells sharing `build/nm_ranking` raced on `target.o`.

## Cycle ledger

     1   carriers already live at 183 (savedPosition, savedOffset, rowY)   3 cells: 2417/2417/2282, all +4; memory-class carriers keep the reset as a store
     2   fresh carrier k + n*2 probes after columnStep (6 forms)            6 cells: probes insert blocks; w40 3.5/4.0 but W's save falls (0.67) or W is rematerialised
     3   the same probes above the head statements                         3 cells: 2282 (+4), probe blocks land before 183; w40 2.67, nocs 3
     4   alias D_o058_5E9C through &D_o058_5E98 in case 2 / case 3 loops    4 cells: 2321 (+4, 3.20 not < 3.0, 39 case-2 rows), 2706 (-16 folded), 2330 control
     5   alias the head read at 183 only                                    3 cells: 2435 (+4) on k, ptr and 18: splits the a0 value web
     6   alias the row-loop negate at 192 (+194)                            4 cells: ~3000, delta -4/-8/+12: alias analysis poisoned; order did flip on the control

## Reproduction

As wv-t: cells compiled from copies of the TU in private scratch with the
tree's `tools/ido/cc` and configured flags, then with the instrumented `cc`
under `CDX_LOG=1 CDX_PROC=0 CDX_OUT=... CDX_DETAIL_WEB=all
CDX_LINEAGE_TABLES=all`; every pair passed the `.text` identity gate
(`text_identical: true` in each `result-*.json`). Scores from
`nm_ranking.process_item` on the stock object. Banked at
`whale-resources/cells-wv-u/` in Git's common directory: every cell's
source, `result-*.json`, `allocator-*.log`, the harness, and the two
neighbour captures `allocator-cell-ptr-dW.log` / `allocator-cell-48-dW.log`.
The tree source was never written.
