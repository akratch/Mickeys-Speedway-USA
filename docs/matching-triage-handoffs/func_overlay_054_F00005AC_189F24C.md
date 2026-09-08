<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:start -->
### `func_overlay_054_F00005AC_189F24C` plateau handoff

- source: `src/overlays/o054/overlay54TailA.c`
- score: 1517 differing words
- frame: 0x150
- relocations: 263
- first mismatch: +0x0
- summary: Active packet checkpoint: 1553/1594 words, 59 ordered calls, 21/269 exact relocation sites. Indexed copy kernel and actor cursor recovered.

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

The reconstruction uses `ControlActor`, `ControlPlayer` and a runtime HUD
record view, with typed reads for fields still inside the partial
player header's gaps. It restores the timer correction and level queries,
eight-record digit spacing, the signed time-delta branch and its texture
snapshots, the unsigned offscreen easing, the null-actor return, the separate
mode 3/4 transition arms, and the proved `0.66f` icon scale. The old volatile
stack-padding write and invented per-frame display toggle are removed.

Attempt 7 reached the exact 1,594-word extent with 1,400 masked positional
differences, but that equality combined a short opening with an overlong copy
loop. Further source review found two unnecessarily narrow temporaries: the
time-delta magnitude and reset X coordinate are 32-bit values in the target.
In particular, narrowing the negated minimum halfword loses its positive
magnitude. Those corrections are retained even though the old extent was exact.

Attempt 21 has 1,553 words against 1,594, a 164-byte deficit, 1,517 raw and
masked positional differences, first `+0`, and a `0x150` frame against `0x138`.
Its 263 relocations have 21 exact offset/type/identity tuples and 263 matching
identity/type records when position is ignored. All 59 calls remain in target
order. The four-record unrolled resource-copy kernel is recovered from an
indexed eight-record loop; independent pointer bounds introduced remainder
paths, while same-array pointer comparisons stayed scalar. The actor list now
advances through a cursor as the target does, reducing aligned register
residuals to 475 and structural residuals to 153. Those aligned counts are
mechanism evidence, not match credit.

The runtime HUD view treats the first two record words as resolved texture
pointers and preserves the signed metadata field. Explicit pointer snapshots
are necessary for the single-read copy pattern; copying through the first
record's fields introduces reloads. Separating the sign and digit views, or
making the sign a scalar aggregate, did not recover the remaining direct field
references. An explicit guarded do-loop and a structured outer for-loop were
unchanged; those spellings alone are ruled out. The workbench constant audit
found no aligned non-stack differing immediate in attempt 14. Workbench still
reports `structure-mismatch`; its structure-buckets and constant-audit levers
remain the routing. The opening recurrence still lacks the target's peeling,
and stack/first-record reference structure remains unresolved. The packet is
active and has not reached its five-no-information stopping condition.

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
All 243 guarded TUs pass `gmake check-nonmatching-builds`. The source-only
clean-room and documentation gates pass after `tools/finalize_plateau.py`
records the checkpoint.

<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:end -->
