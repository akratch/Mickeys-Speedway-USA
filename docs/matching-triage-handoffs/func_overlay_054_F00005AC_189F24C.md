<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:start -->
### `func_overlay_054_F00005AC_189F24C` plateau handoff

- source: `src/overlays/o054/overlay54TailA.c`
- score: 1415 differing words
- frame: 0x158
- relocations: 273
- first mismatch: +0x0
- summary: 1588/1594 words, 59 ordered calls; five flat ownership refinements. Next: trace peeled height-exit address sharing and shared state induction.
Final disposition: reconstruction plateau after five consecutive refinements
without new matching information. The guarded C is compiled, not object-exact
or ROM-exact. The owned range is overlay 54 text `+0x5AC..+0x1E94`, ROM
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

The reconstruction uses `ControlActor`, `ControlPlayer` and the project's
`OverlayOffsetRecord`, with typed reads for fields still inside the partial
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

The retained attempt 81 has 1,588 words against 1,594, a 24-byte deficit,
1,418 raw and 1,415 relocation-masked positional differences, first `+0`, and a `0x158`
frame against `0x138`. Its 273 relocations have 67 exact
position/type/identity tuples and 268 matching identity/type records when
position is ignored. One alternate-pointer LO record is replaced by an extra
HI record, and the two peeled height-loop exits each introduce an additional
HI/LO pair. All 59 calls remain in target order, and the first nine call sites
have exact offsets. The baseline size deficit is reduced by 96 bytes, from
120 to 24; this earns no matching credit. Attempt 61 has the best valid masked
positional score, 1,395 at 1,584 words, but lacks the later counter and accumulator
recovery. Its artifact is preserved. The heartbeat may still rank attempt 7
first because its ordering favors equal geometry; that superseded candidate
contains the narrowing defect described above and is not the retained result.

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
natural zero-based indices recovered those anchors. The metadata-only alias and target load order account for 268 of the 269
identity/type records; the later peeled recurrence adds four extra records. The intermediate artifacts remain
available; none is object-exact.

The original opening animation call was 136 bytes early. Ordinary counted
and guarded post-tested loops, a named damping step, an explicit prior-height
snapshot, and division by eight did not recover the target's peeled recurrence.
The initial explicit source batching was unrolled again into sixteen-update
blocks. Countdown induction retained a descending loop. Scalar, adjacent-float
aggregate, indexed state-tail aggregate and one-element array diagnostics did
not recover the missing peeling; the unproved layout diagnostics remain ignored.
A named scale constant left text unchanged but went into writable data under
the stock compiler, so the exact `0.66f` literal remains the source spelling.

Attempts 54 and 55 established a new CFG lever: an explicit backedge or a
structured loop with an internal latch exit avoids the unwanted second unroll.
Attempts 56 through 65 independently reconstruct the carried remainder and
four-update domains using that lever. The current ordinary C carries the prior
height and its damping step, uses a local remainder, and keeps the icon and
screen coordinates in separate source roles. For an enabled positive update
rate, the remainder performs exactly the low two-bit count, followed by the
remaining four-update batches. Every delayed step is consumed exactly once;
the counter stops before signed overflow. No padding update, dummy operation,
new memory effect or compiler-flag change is used.

Carrying only the product or the completed next height leaves the opening
short. Carrying both prior height and product recovers its geometry; the common
counter increment then aligns the animation and video-size calls exactly. The
compiler still emits ordinary branches and product copies where the target
uses branch-likely delay-slot adds, and it gives the final stores separate
absolute address pairs. A local height pointer forwards away intermediate
stores and regresses the opening by 224 bytes, so it is rejected. Distinct
coordinate roles restore the target's update-rate and timer saved-register
roles. Factoring the remainder exit, structured internal exits, and limiting
the float pair's lexical scope do not further improve the current shape.

A compact icon record plus termination pointer reduces the frame by eight
bytes but does not prove the original local extent; it remains an ignored
diagnostic. Guarding the whole player body on nonnull actors and reconstructing
coordinate defaults do not improve the relevant CFG. The retained C keeps the
ordinary early return and coordinate expressions. Explicit byte induction, separate typed state cursors,
inferred template interior-array grouping and sign-record view variants do not
recover the target's allocation and relocation structure. These negative
results are retained with their predictions; unchanged inputs are not new
work. Workbench still routes through `structure-mismatch`,
`structure-buckets` and `constant-audit`. The final proof still has a structural mismatch, so no promotion was attempted.

Attempt 66 recovers the centisecond accumulator as two successive updates,
matching the target's actual def-use sequence and removing the extra temporary
from the combined draft expression. The workbench normalized distance falls
from 1,151 to 426; this is supporting structural evidence, not a positional
match score. Subsequent shared-byte-index variants add unwanted state-table
address pairs. A separate digit-copy counter does not recover the target's
retained initialization. Removing the explicit address casts from the first
spacing bound regresses both its geometry and residual.

The opening refinements establish a specific remaining tradeoff. Independently
scoped float pairs recover the target's constant-register roles but enlarge the
frame. Using the global height as the previous-value carrier removes the four
extra height records, while the animation calls move 24 bytes early; that
variant has 1,582 words, 1,514 masked differences and 269 relocations. A completed
height accumulator has the same short geometry. A third named result leaves a
copy at the loop merge; factoring a common consuming store collapses the peeled
shape. These variants remain ignored evidence, not improvements to bank by
cancelling unrelated region sizes.

Attempt 76 identifies the shared record type through the exact producer and
copy callees (Tier B): `overlay54PatchIndices` casts resolved resource pointers
into signed words, and `overlay54CopyOffsetRecords` uses `OverlayOffsetRecord`.
The retained source uses that type directly and follows the producer's explicit
pointer-to-word conversion. Its field layout is unchanged. The first sign/digit
buffer has ten records including its terminator, and the per-player row and
state arrays have four entries, based on the owned storage extents and their
initialization traversal. Template cursors are read-only views; the templates
remain writable by their producer. Per-player pointer declarations now express
their actual iteration lifetime. These types do not claim the original typedef
spelling or repair the compiler's remaining allocation choices.

Stopping evidence: attempts 77 through 81 are five consecutive refinements of
the already tested type-ownership explanation: complete the first array extent,
complete per-player extents, qualify the template cursor, scope HUD pointers,
and scope actor/player/level pointers. All retain 1,588 words, 1,415 masked
and 1,418 raw differences, the `0x158` frame, 273 relocations, 67 exact sites,
268 identity/type records and the normalized distance of 426. Attempts 77 through
80 have identical executable text to 76. Attempt 81 changes only stack-home
displacements, without improving any of those measurements or recovering the
missing target lifetime. They prove no new identity or eliminated mismatch
mechanism. The last new type evidence is 76; the last substantive body recovery
is the accumulator at 66, following the opening anchors at 65. This is the
assignment's five-attempt stall, not an attempt cap or deadline exhaustion.

Concrete next lever: pin the retained configured C/object and use a separately
authorized compiler-causal packet to trace where the peeled height exits lose
the shared address, and where the related mode/alpha accesses lose the target's
common four-byte induction. Establish the producing compiler phase and require
an unchanged trace-off baseline before proposing another source relationship.
The remaining alternate-pointer HI/LO discrepancy must also be resolved before
any exact claim. Declaration permutation, a flag lattice and a permuter are not
substitutes for this missing causal evidence and were not used in this packet.

Every source/object/score and the runtime identity audit are retained in ignored
`build/whale5/`. The raw m2c output remains ignored and was used only as a draft.
The generated all-header context could not parse unrelated incomplete types;
a narrowed project-header context produced the draft. The actual stock-
preprocessed baseline passed the context comparator against itself.

Validation commands: `tools/wb_compare.sh --summary-json` for the owned symbol;
`decomp-workbench diagnose` and the `structure-buckets`/`constant-audit` guides;
the runtime relocation/call census; `gmake verify`;
`gmake check-nonmatching-builds`; `gmake check-docs`; and `gmake cleanroom`.
`tools/finalize_plateau.py` guards and records this final disposition.
The configured NON_MATCHING full TU compiles. The rebuilt canonical
fallback has unchanged text bytes and named relocations compared with the base.
`gmake verify` fails both before and after this work because the base overlay 94
controller fallback retains unresolved resident call names and truncated call
relocations; logs are retained in the ignored evidence directory. This packet
does not alter that separately owned source or claim a passing full-ROM gate.
All 243 guarded TUs pass `gmake check-nonmatching-builds`. The source-only
clean-room and documentation gates pass after `tools/finalize_plateau.py`
records the plateau. The final call census again proves all 59 identities in
order. The full-ROM gate remains blocked by the unchanged base overlay 94
link failure; no candidate linked-ROM proof or new matched bytes are claimed.

Files changed: only `src/overlays/o054/overlay54TailA.c` and this symbol-owned
handoff shard. Earlier reconstruction milestones are `18ce6969`, `4fb1d5c2`,
`16242945`, and `ee5b33de`; the final function-sized commit banks the accumulator,
shared types and measured plateau. No symbol-table or atlas match status changes.

<!-- plateau-handoff:func_overlay_054_F00005AC_189F24C:end -->
