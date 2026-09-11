<!-- plateau-handoff:overlay98RenderReflections:start -->
### `overlay98RenderReflections` plateau handoff

- source: `src/overlays/o098/overlay98RenderReflections.c`
- score: 57/389 words
- frame: 0x190
- relocations: 36
- first mismatch: +0x0
- summary: Exact-size V0 has a 56-byte non-save frame deficit and 21/36 relocation tuple alignment; prior natural mechanisms are exhausted.

## 2026-09-11 the frame runs the other way, and the 56 bytes are solved (lane `lane/o11-frames`)

No source change adopted.  The frame arithmetic is now closed as arithmetic, and
the placement is measured rather than argued.

**The identity.**  Both objects put the argument build, the ten saved-register
words and the single saved double at identical absolute offsets, and both leave
the same 80 bytes below the declaration block.  So the target's larger frame is
entirely block: 376 bytes against this candidate's 320.  **This candidate is 56
bytes of declaration short**, which is the opposite of every other frame residual
on the queue and is why `L112` is the law that reaches it -- the free parameter
here is how many cells the target declares, not how many this one can shed.

**Where the 56 bytes go.**  The three 64-byte matrices are visible on both sides
as the only address-taken slots, and their spacing is 64 on both, so they can be
used as rulers.  Measured from each frame top:

- this candidate: matrixC at -64, so the matrices are the first declarations
- the target: matrixC at -92, so **28 bytes -- seven cells -- are declared before
  the matrices**
- between matrixA and `transform`: two cells here, three in the target
- after `inverse`: eighteen cells here, twenty-four in the target

7 + 1 + 6 = fourteen cells = 56 bytes.

**Two of the seven pre-matrix cells are identified.**  The target homes -24 with
one load and two stores and -28 with two loads and one store.  Those are exactly
the traffic signatures this candidate's `gfx` and `cursor` carry at the bottom of
its own block.  Declaring `gfx` sixth and `cursor` seventh, ahead of the
matrices, is therefore the reconstruction, not a padding choice.

**Verified by construction.**  Filling the three regions with five, one and eight
unused `s32` cells and moving `gfx` and `cursor` to positions six and seven:

- frame 0x190 to **0x1C8**, the target's, at delta 0
- first mismatch +0x0 to **+0x68**
- immediate-only 17 to **9**; masked 332 to 327; byte-exact 96 to 102
- **24 of the 37 stack slots land on the target's exact offsets**, up from the
  14 saved-register and argument slots that agreed before: all three matrices,
  every field of `transform` and `inverse`, `cursor`, and `savedStateIndex`

The padding is a measurement, not a candidate, and is not committed.  What it
establishes is that the target's block is solvable and that the placement above
is right within two cells.

**The two cells still wrong.**  After that fill, the candidate homes -368 and
-372 (`savedReferenceY`, `savedDistance`) where the target homes neither, and
misses the target's home at -296.  Both are `L118` shaped: the target carries
those two volatiles in registers and homes something this candidate does not
declare at all.  Deleting the two volatile declarations frees two of the eight
trailing cells for whatever the target's -296 local is.

**Not the lever:** declaration order alone.  Moving `gfx` and `cursor` to the
front without the fourteen cells leaves the frame at 0x190 and the score at 332.
<!-- plateau-handoff:overlay98RenderReflections:end -->
