<!-- plateau-handoff:overlay14CreateValue:start -->
### `overlay14CreateValue` plateau handoff

- source: `src/overlays/o014/overlay14CreateValue.c`
- score: 82/96 words
- frame: 0x28
- relocations: 15
- first mismatch: +0x54
- summary: Resident selector identity authenticated; all 15 relocations align, with 14 positional words remaining.
- base: `3169297845d9e4b3843c03be16cfe6d51358d280`
- boundary: overlay 14 `+0x6FC..+0x87C`, 384 bytes / 96 words, no
  padding or export; two local callers at `+0x3C0` and `+0x40C`
- configured result: exact geometry and frame `0x28`, 82/96
  relocation-aware positional words, 14 differing words, first workbench
  mismatch `+0x54`
- scheduler evidence: a native IDO 5.3 `as1 -R` capture was trace-off
  identical in `.text`, `.data`, `.rodata`, and relocation tuples. The first
  block contains independent slot-base and end-pointer HI16/LO16 chains. Their
  low halves are simultaneously ready and the earlier logical source line wins.
- strict gain: keeping `slot = gOverlay14Slots28; scan_loop:` on one physical
  line preserves C tokens and reverses the tied low-half choice, closing
  `+0x18/+0x1C` and improving the reproduced plateau from 80/96 to 82/96
- relocation proof: fresh configured full-TU preflight is complete. Target and
  candidate each carry 15 records; all 15 offsets/types and stable/effective
  runtime identities align, with zero unresolved identities. The selector at
  `+0xAC` is the existing `s32 frontGetLanguage(void)`: shipped runtime identity
  `resident:+0x39DFC` plus the resident base selects its existing linked symbol
  at `0x8003A24C`. Its 20 linked bytes equal the original ROM range. Replacing
  the unauthenticated extern/call spelling preserves physical source lines and
  leaves candidate text bytes, frame, geometry, and positional score unchanged.
  No alias, symbol policy, or post-compile instruction edit was introduced.
- diagnostic limits: workbench's synthetic object comparison still reports
  import-fix with raw symbol-label differences; the full preflight authenticates
  runtime identities separately. Geometry edit distance is 12, opcode distance
  is 2, and the owned extent delta is zero. This is not an exact C result or a
  linked-trial recommendation; the NON_MATCHING fallback remains in place.
- residual: the `+0x54` workbench row is the already-authenticated active-slot
  data addend in an unlinked candidate. The first source/codegen residual is
  the chosen-pointer web around `+0xA4`: target uses pool register `v1` and one
  `sp+0x18` home, while the candidate introduces `t3` and later spills the
  copied `slot` web at `sp+0x24`. The final value/count block also carries a
  two-word temporary-ring shift.
- exhausted controls: the prior 119-configuration flag lattice remains closed.
  A call/slot/switch physical-line tie was byte-flat. Direct one-pointer and
  nonvolatile-pointer forms grew the owner by four bytes. Assigning `slot`
  before the selector regressed to 11/96 and displaced nine relocation sites;
  an active-pointer carrier regressed to 76/96, and a scan-key carrier to
  74/96.
- bounded permutation: one ten-minute, two-thread, relocation-annotated batch
  improved its scratch score from 345 to 210 but found no zero. Its best forms
  used forbidden inert labels/guards or changed case semantics; the natural
  carrier results regressed in the real TU and were rejected.
- next lever: retain this source. The operational selector-identity correction
  does not reopen manual source permutation: 14 differing words exceed the
  ADR 0016 manual gate. Await the reviewed transaction-tool integration and an
  explicitly bounded standing batch; a new manual attempt still requires its
  own admissible evidence. Do not repeat flags, physical-line ties,
  pointer-direct, assignment-order, or carrier forms.

#### Re-open under laws L90 / L94 (2026-09-10, lane/c3-reopen2)

- lever set the closure actually searched: the 119-row flag lattice, a
  call/slot/switch physical-line tie, direct one-pointer and non-volatile
  pointer forms, assignment order, three carrier forms, and one bounded
  ten-minute permutation batch. All of them are spellings of the pointer
  carriers; none of them changed which *object* the slot reads belong to.
- **L94's mechanism is present here, and it confirms the closure rather than
  reopening it.** The target object names three separate address
  materialisations at one and the same overlay data address, so the natural
  reading is that the three slot externs are one C array. They are not.
  Merging the two that share a region makes uopt open a single address web and
  lose 4 bytes: the entry array merged with the free-scan array is -4 bytes
  and 95 masked words, the entry array merged with the chosen array is
  -4 bytes and 94, and merging all three is -8 bytes and 80. The one pair that
  is *not* co-region -- the free-scan and chosen bases, separated by the
  bounded do-while -- merges at delta 0, and still scores 17 against the base
  14. So L94's co-region trigger is exactly what proves the three separate
  externs are required by geometry: any co-region merge is paid for in bytes
  the target does not spend.
- **L90 has nothing to reach.** Both loops are already byte-exact. The scan
  loop lies inside the exact prefix, and the bounded counter test that follows
  it matches instruction for instruction. There is no exit test left to
  rewrite.
- the deciding variable, named: a frame-cell one. The target spills one pointer
  web at one home and reloads it around all three calls. The candidate carries
  the volatile chosen pointer at that same home *and* spills a second copied
  web three cells higher, which is the whole of the constant class in the
  residual and the extra block-local temporary that follows it. A uopt cell
  census confirms the candidate declares six memory cells for five source
  locals.
- what was tried against that variable and failed: every form that removes a
  pointer costs +4 bytes -- the one-pointer form scores 76, the non-volatile
  carrier 76, dropping the copy and using the volatile carrier directly 69,
  and making the scan pointer volatile 104. Moving the carrier's declaration
  to the front or the back of the block regresses to 19 and 23.
- **verdict: the closure was correct and remains correct under L90 and L94.**
  Resume only with a form that removes one *cell* without removing a pointer.
<!-- plateau-handoff:overlay14CreateValue:end -->
