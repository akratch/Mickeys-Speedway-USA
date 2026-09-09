<!-- plateau-handoff:overlay97InitScale:start -->
### `overlay97InitScale` plateau handoff

- source: `src/overlays/o097/overlay97InitScale.c`
- score: 143/144 words
- frame: frameless
- relocations: 0
- first mismatch: +0xD0
- summary: Verdict register-permutation; lever unreachable. P_STUCK_FLAT and p2 force grid stayed nonexact. Next: source-semantic uopt address-fold attribution.

#### 2026-09-09, lane fin-near: the word is an address fold, and it is bounded

Remeasured in a direct-compile loop that is `.text`-identical to the
configured NON_MATCHING object: 144 words, frameless, one word at `+0xD0`.
The target derives the values cursor from the live bounds carrier
(`a2 = a3 + 2`); the candidate derives it from the model pointer
(`a2 = a1 + 0x3E`). Measured this pass, each against the real object:

- `volatile` on `bounds` is irrelevant: the plain `register` form is the same
  1 word. What decides the shape is the *base* each pointer is spelled from.
  `values` from the re-read `instance->model` (a CSE temp) keeps the single
  a2 chain; `values` from the named local `model` splits the chain into two
  registers (a3 = initial, a2 = first increment) and costs 68 words.
- Deriving `values` from `bounds` in any spelling (`(u8 *)bounds + 2`,
  `&bounds->bounds[1]`, `bounds + 0` then `++`, before or after the first
  read, with either base) makes uopt fold `bounds` into the first load
  (`lh 60(a1)`) and costs 3 to 93 words. Once `bounds` has a second use it is
  propagated; with the load as its only use it is materialised. That is the
  opposite of the usual single-use rule and is the wall.
- A single six-read pointer chain starting at `model + 0x3C` (no `bounds`
  variable, `values++` after every block) reproduces the target's whole
  register structure -- a3 for the initial value, `a2 = a3 + 2`, the folded
  second increment and the four `addiu` -- and is 2 words: the first load
  folds to `60(a1)` where the target reads `0(a3)`. Twenty spellings of that
  first read (subscript, deref, member, `s16 *` and struct pointer, with and
  without `volatile` and `register`) are all flat at 2.
- The closest structural form (bounds from the re-read, `values` a cast copy
  of it, `++` inside block 1) is 3 words: the sum reuses a1 instead of
  taking a3, and the load folds. The target's a1 is dead after the sum, so
  the fresh colour is not a liveness effect source can buy.

The 1-word baseline is retained. What is left is uopt's decision not to fold
`model + 0x3C` into the first load when the same sum also seeds the chain; no
C spelling found makes it both a materialised register and the load's base.
<!-- plateau-handoff:overlay97InitScale:end -->
