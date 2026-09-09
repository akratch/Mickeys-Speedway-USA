<!-- plateau-handoff:overlay20RemoveEntry:start -->
### `overlay20RemoveEntry` plateau handoff

- source: `src/overlays/o020/overlay20RemoveEntry.c`
- score: 51/53 words
- frame: frameless
- relocations: 10
- first mismatch: +0x6C
- summary: the limit temp is uopt pool web 42, blocked from v0 by invisible web 8; CDX_FORCE p2:w42=c1 is declined, so reordering that leaves web 8 cannot reach it

#### 2026-09-09 (second pass): the blocking web is named

The two words are still `addu v0,t0,t9` / `sltu at,a1,v0` against the
candidate's `a2`: the compaction loop's limit pointer. The instrumented uopt
identifies it as pool web 42. When web 42 is coloured its `forbidden0` is
`0x78000000` -- v0, v1, a0 and a1 are all taken by interfering, lower-numbered
webs -- and a2 is simply the lowest colour left. Its four interference partners
are web 37 (a1, the cursor), web 34 (a0), web 10 (v1, `i`) and web 8 (v0).

Web 8 is invisible. Forcing it to a3 changes no instruction anywhere outside
the compaction loop, so it holds v0 without emitting a word.
`CDX_FORCE=p2:w42=c1` is declined at both the decision and the colour site,
which proves the interference is real rather than a priority choice: no source
reordering that leaves web 8 where it is can reach the target. Freeing v0 by
forcing web 8 elsewhere is not the route either -- web 37 is coloured first,
takes v0, the limit takes a1, and the residual grows to six words. The target
needs web 8 not to *interfere* with web 42, not merely not to hold v0.

Newly eliminated, all byte-flat at two words: every dead-store colour
reservation on a spare `s32`/`u32`/pointer local at four placements and on
`entry` itself; a bare extra declaration of each type; `entry` typed as `s32`,
`u32`, `u8 *` and `void **`; a 120-row lattice of four declaration orders by
three search-loop spellings by four compaction spellings by two tail orders, in
which the limit register is only ever a1 or a2; the `owner`, `new_var` and
global-re-read bound carriers; and nesting the whole body instead of the early
returns. Deliberate extra pressure (keeping `entry` or `owner` live past the
loop) pushes the limit *up* to a3 and never down to v0, the same
one-directional signature.

Next: identify web 8's source construct (a `CDX_DETAIL_WEB=8` capture gives its
`bb`/`line` and neighbours) and remove its live range across the compaction
loop; that is the only remaining degree of freedom.

<!-- plateau-handoff:overlay20RemoveEntry:end -->
