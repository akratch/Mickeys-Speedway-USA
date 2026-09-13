# Entry cycle and temporary FIFO: lane wv-e, 2026-09-13

**Retained source: 187 masked / 358 raw differing words, 14,456 bytes,
delta zero, frame 0x138, first masked mismatch +0x50.** The entry window
still has 34 naming and two structural rows. No original entry naming row
is repaired by any target-width probe, and no executable bytes are promoted.
The body and the split-axis source at lines 632-665 remain unchanged.

### The dispatch premise fails its own draw-order test

The supplied census contains 720 draws, all from the nine integer temporary
registers. None draws s0, s1, s2 or s3. Its first draw is at source line 338,
after the initial switch; the first mismatch precedes that draw. The saved
switch copy already agrees at s2. The wrong incoming switch value is a2
against a0, a pool allocation recorded for web 9, not a temporary FIFO draw.

The 0x400-byte window is not one basic block. Its aggregate substitutions
combine the initial switch, the copy-loop cursors and the first drawing case.
Before the copy loop there are five a2-to-a0 operand substitutions; the copy
region has six v1-to-s3, six a0-to-s0 and two a1-to-v1 substitutions; the
second dispatch adds the sixth a2-to-a0 substitution. The saved-register cycle
belongs to the later case-1 portion of the window. A dominant mapping over
those different values is a graph cycle, not evidence of one FIFO schedule.

The existing unforced allocator trace records copy-cursor webs 27 and 26
coloured v1 and a0, respectively, and the loop endpoint web 37 coloured a1.
Their allocation therefore cannot be inferred from the temporary draw list.
No allocator or temporary census was needed to rediscover those supplied rows.
The private-copy control additionally checks that the comparison harness
reproduces the supplied profile and full object surface.

The unchanged-source landscape report passes its freshness check. It also
contradicts the claim that w27 is its only winner touching this window:
w75=c16 has signed positional improvements totalling twelve words in four
entry subwindows, within its fifteen-word whole-function gain. w75=c15 and
three w962 colours also touch entry. These are the supplied force diagnostics,
not newly compiled forces or adoptable source. No colour landscape is rerun.

### Source probes, each compiled with stock and traced fidelity

All probes preserve physical source lines, original call order and the
split-axis region. Their complete sources, stock and traced objects, commands,
first mismatches, per-line census and aligned deltas remain private.

- Attempt 1 normalizes the initial positive-count guard through an inverted
  relation compared with zero. It adds one draw at line 338 and changes the
  complete draw sequence: 721 draws, 5,073 emission records. Width stays exact,
  but the score becomes 1,494 masked / 1,575 raw, with 118 entry naming rows.
- Attempt 2 applies the same boolean normalization at the copy-loop exit.
  It adds one draw at line 342: 721 draws, 5,072 emission records and a changed
  sequence. At target width it scores 1,489 / 1,570, with 114 entry naming rows.
- Attempt 3 gives the first switch an explicit existing rowBase carrier,
  dead before its later initialization. It reproduces 187 / 358 and the entire
  720-draw, 5,072-emission profile.
- Attempt 4 reuses i only as the initial copy-loop induction; the later
  consuming cases initialize it before use. It also reproduces 187 / 358 and
  the complete baseline draw profile.
- Attempt 5 uses a separate block-local signed copy index. It scores 195 / 366
  at target width, keeping all 34 entry naming rows and the entire baseline
  draw profile. The eight added naming rows are near the function tail.
- Attempt 6 makes only the nonnegative case-1 entry-array subscript unsigned,
  testing the supplied w75 nomination through source type partition. It scores
  190 / 361 at target width, retaining all entry rows and the complete baseline
  draw profile; three naming rows worsen in case 2.
- Attempt 7 expresses both copy cursors as typed pointers and terminates on
  the source-array endpoint. It changes allocation and draw order but costs
  24 bytes: 3,562 / 3,564, 722 draws and 5,061 emission records.
- Attempt 8 keeps both explicit pointers and restores the original signed
  count termination. It costs 20 bytes: 3,567 / 3,570, 720 draws and 5,062
  emission records, with changed draw order despite the unchanged total.
- Attempt 9 keeps only the explicit source pointer. It restores target width
  and the baseline draw sequence, scoring 196 / 367 with 5,066 emission records.
  Its apparent entry improvement from 34 to 33 naming rows is category
  migration: two naming rows become structural and another naming row appears.
  Entry structural rows rise from two to four; no original naming row heals.
- Attempt 10 keeps only the explicit destination pointer. It reproduces the
  two-pointer counted form's 20-byte surplus and 3,567 / 3,570 score, with
  720 draws, 5,066 emission records and changed draw order.

Every frame remains 0x138. Target-width cells retain 1,253 candidate relocation
records; the three oversized pointer cells have 1,257. All target-width probes
retain first masked mismatch +0x50. The oversized cells move it to +0x54,
which is not an accepted first-mismatch improvement at the wrong extent.

The two phase interventions each repair **zero of the original 34 entry
naming rows**. They establish that the FIFO is source-movable at zero byte
cost, while falsifying the proposed shared entry-phase repair in those forms.
The pointer controls change pool allocation, but the last three follow-ups
supply neither a better admissible residual nor a new repair mechanism:
restored scalar termination remains oversized, the source-only cursor merely
changes row categories, and the destination-only cursor reproduces the bad
counted-pointer output. This is the stopping evidence, not a ten-attempt cap
or an assertion that all C source is exhausted.

### Preservation, validation and next action

The ADR 0011 assignment gate is base-only. The configured full-TU baseline,
private source-copy baseline and supplied instrumented object pass section,
relocation and symbol fidelity. The actual compiler-output self-comparison is
function-exact. Fresh preflight reports 1,253 runtime/static records per side,
1,236 offset/type agreements and 671 stable identities; 395 candidate
identities remain unresolved. Its +0x2C first mismatch is the unmasked assembly
fallback representation, distinct from the +0x50 masked metric used here.
The guarded C is compiled nonexact evidence; canonical ROM verification uses
the assembly fallback.

The best source and all ten attempts are preserved in
`matching-evidence/wv-e-entry-phase-20260913.tar.gz` under Git's common directory.
The live ignored packet is `build/wv-e/`. tools/finalize_plateau.py refreshes
only EOF handoff metadata and this shard; tools/gates.sh supplies the closing
gate verdicts. No merge or push is performed.

Next action: use the banked entry pool-web and interference evidence before
assigning any further source experiment. Require a source hypothesis that
changes the relevant pool allocation at target width; a graph cycle or changed
FIFO sequence alone is not that evidence. The split-axis packet is unaffected.
