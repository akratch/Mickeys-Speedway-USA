<!-- plateau-handoff:func_overlay_007_F0000324_185C1AC:start -->
### `func_overlay_007_F0000324_185C1AC` plateau handoff

- source: `src/overlays/o007/func_overlay_007_F0000324_185C1AC.c`
- score: 129/348 words
- frame: 0x78
- relocations: 61
- first mismatch: +0x7C
- summary: Frame closed at 0x78; every stack slot now agrees. Residual is three missing words and 30 naming rows.

## 2026-09-11 frame closed (lane `lane/o11-frames`)

The frame identity, solved from both objects rather than guessed.  Argument
build, the five saved-register words and the `ra` slot sit at identical
absolute offsets on the two sides, so the whole 8-byte gap was the declaration
block: this candidate's was 56 bytes for fourteen locals, the target's is 44 for
eleven.  What pins the count at eleven rather than twelve is the compiler
temporary that carries the first measurement across the second call -- it is the
*second* temp below the block on both sides, so an eleven-cell block puts it at
-52 and a twelve-cell block at -56.  The twelve-declaration control, with a
separate `scale`, was built and measured: frame back to 0x80, immediate-only
bucket back to 24.

Three declarations came out, none of them by merging into a live carrier:

- `handleObject`: deleted, the call reading `overlay7RuntimeLastObjectReloc->x/y/z`
  directly.  uopt commons the global load into one `v0` temp, which is what the
  target does.  Byte-identical code, one declaration fewer.
- `cursor`: deleted, the loops indexing `objects[remaining]`.  Strength reduction
  rebuilds the identical walking pointer in `s1`.
- the nested `scale`: deleted, its hoisted loop invariant reusing the `difference`
  carrier, whose live range is the first object loop and therefore disjoint.

Then declaration order, which is free: `objects` 6th, `object` 7th, `difference`
11th.  That reproduces the target's stack ladder exactly -- all fourteen slots,
the compiler temp included.  The other eight positions are byte-inert; four
random permutations of them scored identically.

Measured, aligner buckets before and after:

- before: 238 masked, frame 0x80 against 0x78, 229 byte-exact, 45 naming,
  28 immediate-only, 50 really-different, first mismatch +0x0
- after: 129 masked, frame 0x78, 280 byte-exact, 30 naming, 5 immediate-only,
  33 really-different, first mismatch +0x7C

The immediate-only bucket is the one that tracked the frame: 28 to 5.

One further structural edit is folded in: `entry` is assigned inside the
`D_290->nested != NULL` arm rather than above the test, which is what makes the
target's `move` of the commoned load into the `entry` carrier appear.

### What is left: three words, both named

The candidate is now twelve bytes short, and the alignment names all three
missing words.

1. The `D_844` scan.  The target counts a register down from 43 and walks the
   table pointer; this candidate's `index` dies at strength reduction and the
   loop compares the pointer against a precomputed end.  The target also copies
   its counter to `v0` every iteration, dead on the path that takes the call, so
   its counter is live across the call in uopt's view and this one's is not.
   That accounts for one word.
2. The sixth argument of `overlay7CreateRuntimeOwnerReloc`.  This candidate
   passes `NULL` and stores `zero`; the target materialises a relocated address
   into a register and stores that, which is two words.  The argument is not
   null, and identifying what it points at is reconstruction, not allocation.

Thirty naming rows remain after that.  They are not a frame problem any more.
<!-- plateau-handoff:func_overlay_007_F0000324_185C1AC:end -->
