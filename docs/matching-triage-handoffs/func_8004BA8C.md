<!-- plateau-handoff:func_8004BA8C:start -->
### `func_8004BA8C` plateau handoff

- source: `src/main/font.c`
- score: 6 differing words
- frame: 0x30
- relocations: 9
- first mismatch: +0x30
- summary: Frame and both spill homes now exact via the declaration-order home rule; residual is register-only, one web (fontData a3 vs v0).

#### 2026-09-10, lane `o7-mid`: ten words to six; the frame and both spill homes are now exact

Measured with `tools/score_symbol.py`: **6 relocation-masked words, size delta
0, category `register-only`**, first mismatch `+0x30`. The prior record was 10
words with a `0x20` frame against the target's `0x30`.

**The rule that moved it.** IDO gives a four-byte frame home only to a local it
leaves **memory-class**, and assigns those homes **descending from the top of
the local block, in declaration order**. A local uopt colours owns no slot at
all. Measured here: with the six original locals only `fontData` and `spacing`
are memory-class, so the block is two words (frame `0x20`) at `sp+0x1C` and
`sp+0x18`, in declaration order. Each further memory-class local adds one word,
the block rounds to 8, and every home moves with its declaration position.

The target's block is six words (frame `0x30`, slots `sp+0x2C`..`sp+0x18`) with
`spacing` at `sp+0x20` (fourth slot) and `fontData` at `sp+0x18` (sixth). So
the target's source carries four further memory-class locals: three declared
before `spacing`, one between `spacing` and `fontData`. Reproducing that
arrangement closes the frame word, the epilogue word and both spill-home pairs
at once — four words.

**This corrects the earlier "the rejected stackPad form is not a source lever".**
Padding alone is not one: three pads in any position reach the `0x30` frame and
still miss both homes (8 words, measured). The lever is the declaration
*position* of the two memory-class locals inside a six-slot block, which is a
different edit and had not been tried.

**The residual is one web.** `fontData` takes `a3` where the target takes `v0`
(`+0x30`, `+0x3C`, `+0x4C`, `+0x60`); the `+0x90` `0xF` compare and the `+0x94`
spacing-index add are the two commutative operand orders the previous pass
correctly predicted would flip only with the colouring.

Falsified at this base — all flat at six, all `register-only`, all delta 0:

- every `(spacing, fontData)` slot pair other than (4th, 6th), in five-, six-
  and seven-slot blocks (92 points);
- `fontData = D_800D60E4 + font` instead of `&D_800D60E4[font]`;
- `spacing[current]`, `current[spacing]`, `*(spacing + current)`,
  `*(current + spacing)`;
- both orders and both constant-first spellings of the `0`/`0xF` tests;
- `s32` rather than `u8` `defaultWidth`; pointer- and `char *`-typed slot
  locals;
- a named `0xF` and a named `0x80` initialised before the call, in every slot
  position — uopt re-materialises both ([L102]), so neither manufactures a web
  that could take `a3` first.

Changing the instruction geometry instead (and therefore not levers): moving
either assignment past the `if` block, inlining either global subscript, and
reading `characterWidth` before or inside the loop.

**Next lever.** `fontData` -> `v0`. Its web spans the conversion call in the
target too and is spilled to the same `sp+0x18` home on both sides, so L101's
call-result exclusion is *not* what holds `v0` back here. The open question is
p1/p2's visit order between this web and the inner `current` web, which also
holds `v0` and does not interfere with it. Run a CDX force sweep to establish
whether `v0` is on this web's candidate list at all before trying another
spelling — a silent decline is L101's signature and would mean no colouring
lever reaches it.

**The four reconstruction locals are committed as `frameSlot0..3`** with a
comment saying they are a measured reconstruction, not recovered source.
Replace them with the real locals if those are ever recovered; the object must
not change.
<!-- plateau-handoff:func_8004BA8C:end -->
