<!-- plateau-handoff:overlay94UpdateController:start -->
### `overlay94UpdateController` plateau handoff

- source: `src/overlays/o094/overlay94UpdateController.c`
- score: 11 masked / 22 raw differing words
- frame: 0x70
- relocations: 36
- first mismatch: +0x1C
- summary: Authenticated integer-return correction leaves emitted code unchanged: 11 masked differences with exact size/frame and 36 aligned relocation identities.

#### Narrow return-type correction

One authorized correction against `3b03e801` changes the local declaration of
`func_8005ABA8` from `void` to `s32`, preserving its arguments, the complete
function body, and physical source lines. The callee's own 444-byte linked
range at `0x8005ABA8` equals the original ROM range at `0x5B7A8`. Its early
return explicitly returns zero; its normal return transfers a zero/one status
to the integer return register. Mickey's models TU already declares the same
integer return, and the independently proved O4 correction corroborates it.
The O94 runtime call at owned offset `+0x28C` resolves to
`resident:+0x5A758`, the same callee under the resident runtime base.

Fresh full-TU probes before and after the correction both report 275 words
(1,100 bytes), frame `0x70`, 11 relocation-masked differences, 22 raw
differences, and first divergent row 7 (`+0x1C`). All 36 runtime relocation
records align by offset, type, and stable/effective identity, with zero
unresolved candidate identities. The historical seven-word comparison is
not substituted for this fresh baseline. The opcode edit distance is zero
and structural edit distance is seven, but neither implies instruction exactness.

The corrected candidate's `.text`, `.rodata`, `.data`, and all 36 static
relocation tuples remain identical to its preserved baseline. The normal
fallback TU's same sections and all 12 static relocation tuples also remain
unchanged. Source/object/score/first-mismatch artifacts are retained in the
owning lane's ignored `build/function-probe/` directories.

This is a declaration-fidelity correction with no emitted-code improvement.
The successful O4 return-type lever is target-specific, not evidence of a
bulk matching improvement across other callers.
The `NON_MATCHING` fallback remains, and no matched bytes are credited.
No carrier, CFG, frame, flag, or permutation change was attempted. Further
manual work requires independently reviewed evidence; any standing sweep
must be separately leader-assigned rather than inferred from this correction.
<!-- plateau-handoff:overlay94UpdateController:end -->
