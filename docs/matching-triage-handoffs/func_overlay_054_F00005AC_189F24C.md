<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:start -->
### `func_overlay_054_F00005AC_189F24C` plateau handoff

- source: `src/overlays/o054/overlay54TailA.c`
- score: 1503 differing words
- frame: 0x158
- relocations: 269
- first mismatch: +0x0
- summary: Active reconstruction checkpoint: 1556/1594 words; 59 ordered calls; 268/269 relocation identity/type records, 22 exact sites. No match credit.
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

Attempt 44 has 1,556 words against 1,594, a 152-byte deficit, 1,504 raw and
1,503 relocation-masked positional differences, first `+0`, and a `0x158`
frame against `0x138`. Its 269 relocations have 22 exact
position/type/identity tuples and 268 matching identity/type records when
position is ignored. One alternate-pointer LO record is replaced by an extra
HI record. Equal counts are not a complete relocation match. All 59 calls
remain in target order. The candidate is retained for its reconstructed
interfaces and field identities, rather than its positional score alone.

The actor-list result and walking cursor now have distinct source roles, as
proved by the target's preserved returned base and later cursor initialization.
Per-player HUD arrays use their actual two-, three- and ten-record row strides.
The indexed eight-record resource copies produce the target's four-record
kernel. Independent pointer bounds introduced remainder paths; same-array
pointer comparisons stayed scalar. The signed time-delta and reset coordinate
remain full-width values. A separate, authenticated relocation view of the
first record's metadata recovers its direct global stores. The delta loads now
precede the pointer stores as the target does. The resident interface audit
corrects the button query to `u16`, the sound arguments to `u16` and `void **`,
and the generic pointer and video-size output signatures; explicit caller
casts retain the recovered object views and unsigned screen-size arithmetic.

Attempt 24 reduced the aligned structural residual to 135 through the row
and cursor reconstruction. Attempt 35 accounted for every one of the 269
target identity/type records, with two additional HI records; its independent
alternate-pointer alias cannot share the target's address load. Attempt 36
had a lower positional residual of 1,477 masked words, but shifted digit-loop
address anchors and lost identity agreement. Restoring the digit view's
natural zero-based indices recovered those anchors. The current metadata-only
alias and target load order retain the exact total relocation count while
leaving one type/identity discrepancy. The intermediate artifacts remain
available; none is object-exact.

The opening animation call is still 136 bytes early. Ordinary counted and
guarded post-tested loops, a named damping step, an explicit prior-height
snapshot, and division by eight do not recover the target's peeled recurrence.
A carried-step loop instead uses a rate-minus-one remainder. Explicit source
batching is unrolled again into sixteen-update blocks. Countdown induction
retains a descending loop. Scalar, adjacent-float aggregate and one-element
array declaration diagnostics do not recover the missing peeling. The
aggregate and array diagnostics remain ignored; no unproved layout is adopted.
A named scale constant leaves text unchanged but goes into writable data under
the stock compiler, so the exact `0.66f` literal remains the source spelling.

A compact icon record plus termination pointer reduces the frame by eight
bytes but does not prove the original local extent; it remains an ignored
diagnostic. Guarding the whole player body on nonnull actors and reconstructing
coordinate defaults do not improve the relevant CFG. The retained C keeps the
ordinary early return and coordinate expressions. Explicit byte induction,
inferred template interior-array grouping and sign-record view variants do not
recover the target's allocation and relocation structure. These negative
results are retained with their predictions; unchanged inputs are not new
work. Workbench still routes through `structure-mismatch`,
`structure-buckets` and `constant-audit`. This is an active packet checkpoint,
not a final five-attempt stall disposition.

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
