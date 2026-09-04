<!-- plateau-handoff:overlay1AdvancePath:start -->
### `overlay1AdvancePath` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 121/162 words
- frame: 0x58
- relocations: 22
- first mismatch: +0x10
- summary: Size-near pass confirms one extra result.x reload; ten carrier/CFG/line forms are nonexact and the callee proves the s16 ABI

The retained full-TU C remains 163 instructions against the 162-instruction
target, with the exact `0x58` frame, 121 positional differences, and first
mismatch `+0x10`. Earlier guarded functions shift the candidate by `+0xFC`,
so the fail-closed preflight rejects static relocation ownership; the target
fallback and candidate were compared as isolated diagnostic objects only.

The one-instruction size surplus is localized after `overlay1GetEntry`.
Candidate C loads `result.x` for the endpoint comparison, then reloads it into
`a1` for `overlay1AppendPathPoint`; the target assigns the first load to `a1`
and carries it directly into the call. The promotion trial reports the tail TU
as `text-size-differs (+12 bytes)`. Its zero in/out counters are defaults from
the ROM-length failure, not an in-range equality proof.

Ten bounded source forms did not remove that reload. `register` and plain
`s16` carriers both grow to 165 instructions and 129 positional differences;
the historical `register s32` carrier also grows to 165 and 124 differences.
Reusing `currentX`, grouping the entry/condition or condition/call source
lines, and an explicit equality/goto shape are byte-identical to the retained
candidate. Reversing one or both comparisons leaves the same size and 121
positional differences. A conditional-expression form grows to 166
instructions and 126 differences.

The callee's own target prologue stores both argument registers and explicitly
sign-extends them from 16 bits, proving that the shared `s16` prototype is
authentic. A coordinated `s32` prototype would remove the caller symptom by
falsifying the callee ABI and is not admissible. Preserve the retained source
and fallback. Reopen only with evidence that this caller originally compiled
against a no-prototype declaration across a real TU boundary, or with a new
allocator/web mechanism that keeps the `s16` load in `a1`; do not repeat these
ten forms, the flag lattice, an indirect cast, or a shared wide prototype.
<!-- plateau-handoff:overlay1AdvancePath:end -->
