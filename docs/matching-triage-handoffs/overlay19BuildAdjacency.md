<!-- plateau-handoff:overlay19BuildAdjacency:start -->
### `overlay19BuildAdjacency` plateau handoff

- source: `src/overlays/o019/overlay19BuildAdjacency.c`
- score: 36/123 words
- frame: 0x80
- relocations: 1
- first mismatch: +0x7C
- summary: Named fifth argument and redundant-narrowing removal improve 41 to 36. No colour winners remain; ring draws and one moved-load gap need a new source identity.

### Exhaustive landscape and retained improvement (2026-09-12, p23-lastmile5)

The fresh configured baseline is 123 words (492 bytes), frame `0x80`,
one candidate and one target ELF relocation, and 41 raw/masked positional
differences, first `+0x7C`. Its aligned split is 82 exact, 41 naming,
zero immediate and zero structural rows, without gaps.

`web_footprint.py --every-colour` on procedure zero sampled 17 coloured
webs: 110 rows, comprising 109 accepted same-save-kind alternative forces
at size delta zero and one singleton without another colour. The complete
winners list is `p1:w81=c7` at 39 words. The selected single-force lattice
reproduces 39; stock/instrumented section, symbol and relocation fidelity
passes. This is a diagnostic floor for the measured force set, not a lower
bound over all possible combinations or source shapes.

Five stock source probes crossed signed narrowing with naming the fifth call
argument. Naming alone scores 51; removing the narrowing alone scores 48;
combining both scores **36**. Removing the redundant mask scores 57, or 51
with the named argument. All retain size delta zero. The retained guarded
candidate uses the existing `edgeOffset` local for the fifth argument and
removes the signed narrowing before masking `frame.suppressed`. The preceding
`0x1080` mask already confines that value to the positive signed-halfword
range; this removal preserves its value. The argument expression has no call
or store, and naming it preserves the values passed to the sole call.

Fresh proof of the retained source gives 36 raw/masked differences, first
`+0x7C`, with the same 123 words, one relocation and `0x80` frame. Frame
census reports identical sixteen-slot home sets and no differing shared
slots. Aligned buckets are 96 exact, 25 naming, one immediate and zero paired
structural rows, plus one candidate-only word at `+0xD4` and one target-only
word at `+0x90`. This is a positional improvement with a moved-load gap, not
an entirely register-only residual or a match.

A second exhaustive scan of this actual retained shape sampled 18 coloured
webs: 115 rows, comprising 114 accepted alternative forces at size delta zero
and one singleton. Its complete winners list is empty; the measured
single-force floor is therefore 36. There are no improving forces to combine
in a winner lattice. Stock/instrumented fidelity passes again. Force requests
remain diagnostics and none enter the source.

Three closing stock probes split the two frame stores onto separate lines,
reversed their order, and made the output-offset home unsigned. All score 36
at size delta zero without a better residual or new identity. This is the
ADR 0018 stall evidence. The next source question is which temporary producer
and loop-head load lifetime yield the target ring phase without moving the
output-offset load; another isolated forced colour cannot answer it.

Evidence is preserved privately under `build/p23/overlay19BuildAdjacency/`,
including both landscapes, all source/object/result cells, the aligned map,
frame census and fidelity receipts. Commands: `residual_map.py`,
`web_footprint.py --every-colour`, `force_lattice.py`, configured stock source
probes, `frame_census.py`, `finalize_plateau.py`, and
`tools/gates.sh verify cleanroom check-docs`. The fallback remains canonical;
no executable bytes receive new matching credit.

<!-- plateau-handoff:overlay19BuildAdjacency:end -->
