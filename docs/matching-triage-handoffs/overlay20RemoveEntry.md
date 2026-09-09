<!-- plateau-handoff:overlay20RemoveEntry:start -->
### `overlay20RemoveEntry` plateau handoff

- source: `src/overlays/o020/overlay20RemoveEntry.c`
- score: 51/53 words
- frame: frameless
- relocations: 10
- first mismatch: +0x6C
- summary: The v0-a2 tie is a free-list choice between two dead colours; ~250 further candidates across declarations, dead stores, loop shapes and lock breaks are all 2/53, so no source spelling reaches it and the instrumented uopt oracle is the only lever left.

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
<!-- plateau-handoff:overlay20RemoveEntry:end -->
