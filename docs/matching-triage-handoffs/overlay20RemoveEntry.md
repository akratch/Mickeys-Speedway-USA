<!-- plateau-handoff:overlay20RemoveEntry:start -->
### `overlay20RemoveEntry` plateau handoff

- source: `src/overlays/o020/overlay20RemoveEntry.c`
- score: 51/53 words
- frame: frameless
- relocations: 10
- first mismatch: +0x6C
- summary: the limit temp is uopt pool web 42, blocked from v0 by invisible web 8; CDX_FORCE p2:w42=c1 is declined, so reordering that leaves web 8 cannot reach it

#### 2026-09-09, lane fin-misc: the tie is between two dead colours

Measured on a 64 ms direct-`tools/ido/cc` loop against the real object, scored
reloc-blind (the overlay post-process consumes most of the relocation entries,
so a name-comparing score is not the faithful one). About 250 candidates, every
one 2/53 at the same two words.

The colour order reads straight out of the object. The compaction bound is a
pool web, defined at +0x6C and dead at +0x7C. At that point both `entry`'s
colour (v0, last used at +0x2C) and the `&gOverlay20EntryCount` address web's
colour (a2, last used at +0x58) are free. The target reuses v0 -- the
earlier-freed of the two -- and IDO reuses a2, the more recently freed one.
Nothing in the emitted code keeps v0 live across the bound, which is why the
2026-09-08 proc-0 trace had to call the blocker an *invisible* web.

Newly eliminated this pass, each measured:

- All six declaration orders of the three locals, plus four extra-local shapes
  (a leading, middle and trailing dummy, and a `void **` cursor). Flat, which
  is the field guide's rule that declaration order does not drive colouring.
- A 63-cell colour-reserving dead-store sweep: `dead = 0`, `= i`, `= owner`,
  `= entry`, `= NULL`, `= gOverlay20EntryCount` and `= gOverlay20ShiftEntries`
  inserted at each of the nine statement positions. None of them reserves a
  colour here, so the `value = 0` reservation rule does not apply to this site.
- Eight compaction-loop spellings: re-reading the bound from the global (6
  words), `!=` and reversed conditions, unsigned bounds (14), a separately
  named end pointer (15), `++i` in the condition, and hoisting the bound above
  the guard (6).
- Five search-loop shapes and five marker-loop shapes, including the reversed
  entry comparison and a `for` marker loop (10 words, one instruction fewer).
- Use-side lock breaks on the bound, on `i`, on the store index and on the
  entry comparison -- the lever that closed `func_8003A2C8` the same day.
- Retyping the bound carrier as `void *` or `void **`, and folding it back onto
  `entry` so the two share one variable: 22 to 29 words, because the entry
  comparison's type changes with it.

The verdict is unchanged and now much better supported: no spelling of this
function reaches the target colour. The instrumented uopt capture -- a print in
the recompiled `globalcolor` showing the free list at the bound's web -- is the
only remaining lever.
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
