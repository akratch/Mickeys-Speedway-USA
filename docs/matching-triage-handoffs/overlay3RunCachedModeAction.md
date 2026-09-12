<!-- plateau-handoff:overlay3RunCachedModeAction:start -->
### `overlay3RunCachedModeAction` plateau handoff

- source: `src/overlays/o003/overlay3RunCachedModeAction.c`
- score: 34/113 words
- frame: 0x58
- relocations: 15
- first mismatch: +0x84
- summary: Inlining call-result carriers loses a frame quantum; intermediate-boolean deletion still misses the switch transfer.
#### 2026-09-12, lane p23-lastmile5: complete colour landscape and boolean shapes

Fresh baseline: 113 words, delta zero, 34 raw and masked differences, frame
0x58, 15 relocations, first masked mismatch +0x84. The older 79-word header
counts agreeing words. Aligned buckets are 91 exact, 11 naming, zero immediate,
ten paired structural rows, plus a candidate-only word at +0x98 and a
target-only word at +0xE0. No source was adopted.

The single-procedure TU's full same-save-kind scan covers nine coloured webs
and 82 accepted alternatives: 74 retain geometry and eight grow. The complete
winners list is `p1:w29=c2` at 29, `p1:w29=c3` at 31,
`p1:w36=c4`, `p1:w36=c5`, `p1:w36=c6` at 31 each,
`p1:w39=c3` at 30, and `p1:w79=c3` at 31. The coarse-window packing selects
only web 29 at colour 2. A 15-cell lattice takes that force, web 36 at colour
4, and the web 39/79 winners, one colour per web. All cells apply; its measured
floor is 25 with webs 29 and 39, which are additive despite sharing coarse
windows. This is the selected lattice's floor, not an exhaustive combination
bound over the other colours. Stock/instrumented section, symbol and relocation
fidelity passes before that lattice.

Eleven source forms are retained under `build/p23/overlay3RunCachedModeAction/`.
Keeping the angle in its shifted representation, using either signed or unsigned
shifts, scores 43 at delta zero. Its aligned structural bucket falls to five
paired rows, but naming grows to 30 and the surplus moves to +0xA0; the missing
+0xE0 word remains. A short angle local scores 83 with eight extra bytes.
Copying valid through each of the existing angle, encoded and random locals
before the mode switch is flat on both shapes: 34 on the retained shape and
43 on the shifted one. The combined short-circuit expression scores 79 on the
raw shape and 84 on the shifted shape, each twelve bytes too large.

The source question is the transition from the comparison/call-result boolean
to the mode-switch boolean carrier. Moving the angle shift earlier repairs
some structure but does not supply that final carrier transfer. Existing-local
copies normalize away; a complete logical chain changes the frame and code
geometry. Stop after those carrier copies stall and the final expression-shape
check excludes the direct chain. Retain the guarded 34-word candidate, with
both source shapes and their distinct aligned residuals preserved privately.

#### 2026-09-12, lane p24-recipe: carrier deletion does not supply the boolean transfer

The configured baseline reproduces 34 raw/masked differences at 113 words,
delta zero, frame 0x58 and fifteen relocations. Aligned buckets are 91 exact,
eleven naming, zero immediate and ten paired structural rows, plus the
+0x98 candidate-only and +0xE0 target-only pair. First mismatch remains +0x84.
The prior complete nine-web landscape and selected lattice floor 25 were
reused on this unchanged baseline.

Deleting the encoded-result declaration and inlining its call scores 43 at
delta zero. Deleting the random-result declaration and inlining its call also
scores 43. Both lose a frame quantum; their unchanged naming buckets show that
removing these names does not change the desired colour. Retaining the unused
encoded declaration as a frame diagnostic reproduces 34 and the original
aligned map. That diagnostic is not adopted. The inline-call variants would
also require argument/read-order review before any promotion.

Deleting the two intermediate bound booleans, initializing valid to zero and
assigning the path-check result only inside the range, scores 52 at delta zero.
It removes the four structural rows in the mode-action windows but adds naming
rows throughout and retains the missing boolean-transfer word. Computing one
combined bounds boolean first and then checking the path costs eight bytes
and scores 80. The generated call-result and bounds shapes establish no exact
transfer identity, and the last three forms do not improve the baseline. Stop
under ADR 0018 with the original guarded body.

The named source question remains how a generated comparison or call-result
boolean reaches the mode-switch carrier without losing a frame quantum or
adding the target's missing transfer as an extra instruction. Sources, objects,
scores and aligned per-window comparisons are preserved privately under
build/p24/overlay3RunCachedModeAction. No new bytes are counted as matched.

<!-- plateau-handoff:overlay3RunCachedModeAction:end -->
