<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:start -->
### `func_overlay_054_F00005AC_189F24C` plateau handoff

- source: `src/overlays/o054/overlay54TailA.c`
- score: 1400 differing words
- frame: 0x158
- relocations: 281
- first mismatch: +0x0
- summary: Checkpoint; packet continues. Exact size and 59 ordered calls. Target frame 0x138; 71/269 exact relocation sites. Easing/copy regions offset.

This is a committed reconstruction milestone in the active structural packet,
not a final stall disposition. The guarded C is compiled, not object-exact or
ROM-exact. The owned range is overlay 54 text `+0x5AC..+0x1E94`, ROM
`0x189F24C..0x18A0B34`: 6,376 executable bytes, with no padding credit.

The configured baseline reproduced 1,564 candidate words versus 1,594 target
words, a 120-byte deficit, 1,545 raw and 1,544 relocation-masked differences,
first `+0x10`, and equal `0x138` frames. Its candidate object has 240 static
relocations and 48 call sites; the runtime target has 269 records and 59 calls.
The old shard's 269 count described the target, not the candidate object.

Mickey-only runtime records now distinguish all outbound call identities and
the initialized data, BSS, resident-global and local constant surfaces. The
compiled candidate has the target's complete 59-call sequence, including the
four calls to `overlay54GetOffsets`. The overlay 45 object and atlas prove the
release call at `+0x270` and `overlay45SetMode` at `+0x1BE0`; the overlay 56
object proves `overlay56SplitTime` at `+0xB8`. Resident callee definitions
supply the argument types. No donor body or external reference source was
adopted; the near-match oracle found only weak similarities.

The reconstruction uses `ControlActor`, `ControlPlayer` and
`OverlayOffsetRecord`, with typed reads for fields still inside the partial
player header's gaps. It restores the timer correction and level queries,
eight-record digit spacing, the signed time-delta branch and its texture
snapshots, the unsigned offscreen easing, the null-actor return, the separate
mode 3/4 transition arms, and the proved `0.66f` icon scale. The old volatile
stack-padding write and invented per-frame display toggle are removed.

Attempt 7 retains the exact 1,594-word extent with 1,402 raw and 1,400 masked
positional differences, first `+0`, and a `0x158` frame. Its 281 relocations
include 71 tuples with exact target offset, type and runtime identity; 266
records overlap when position is ignored. The final 18 calls have exact target
offsets. This does not prove the interior: the first easing region is short
and the late copying region is long. Workbench still reports
`structure-mismatch`; its constant-audit and structure-buckets levers apply.
A named easing-step local did not recover the opening region's size, so that
spelling alone is ruled out. Continue with the missing source-level loop and
aggregate relationships rather than a flag or permutation search.

Every source/object/score and the runtime identity audit are retained in ignored
`build/whale5/`. The raw m2c output remains ignored and was used only as a draft.
The generated all-header context could not parse unrelated incomplete types;
a narrowed project-header context produced the draft. The actual stock-
preprocessed baseline passed the context comparator against itself.

Validation: the configured NON_MATCHING full TU compiles. The rebuilt canonical
fallback has unchanged text bytes and named relocations compared with the base.
`gmake verify` fails both before and after this work because the base overlay 94
controller fallback retains unresolved resident call names and truncated call
relocations; logs are retained in the ignored evidence directory. This packet
does not alter that separately owned source or claim a passing full-ROM gate.
The source-only clean-room and documentation gates run through
`tools/finalize_plateau.py` before the checkpoint commit.

<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:end -->
