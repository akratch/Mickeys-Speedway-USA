<!-- plateau-handoff:overlay1AdvancePath:start -->
### `overlay1AdvancePath` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 81 differing words
- frame: 0x58
- relocations: 22
- first mismatch: +0x10
- summary: Exact size and aligned opcode shape; 81 masked naming differences; initial three-draw deficit proved diagnostically.

### Historical source shape (superseded 2026-09-12)

The retained full-TU C remained 163 instructions against the 162-instruction
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
### Re-test and retained candidate (2026-09-12)

The inherited one-load-surplus closure falls. A union supplies array views of
the result while retaining the original struct member for the trace call.
The configured full-TU stock output now has exactly 162 instructions, the
`0x58` frame and the existing 22 relocation records. No callee declaration,
calling convention, per-file flag or matched callee body changed. The masked
positional residual improves from 121 to 81 words, first `+0x10`; aligned
buckets (exact/register/immediate/structural) move from 67/74/2/20 to
81/81/0/0. The fallback remains canonical and receives no new matching credit.

Both array load identity and comparison carriers matter. Reversing the
first/second-versus-base comparisons on the new array shape removes the last
non-naming differences. The old operand-order closure therefore does not
transfer to that shape. Reverting all other fields to named members restores
the extra endpoint reload; retaining a named y field alone is byte-flat, as
is an ordinary eight-halfword array instead of the union. The union retains
the callee's declared result layout explicitly. Widening local endpoints,
changing the call ABI, or invoking through a cast is not the lever.

The register census is incoherent across the whole function: multiple regions
have different mappings. Instrumented full-TU output passes the section,
relocation and symbol fidelity gates against the same configured stock input.
Its temporary-allocation trace locates the first discrepancy at the initial
flag expression. A private compiler diagnostic rotates the available temporary
list by three at that allocation, with acceptance recorded and the forced
object scored directly. It makes the initial region exact through `+0xD0`
and reduces the diagnostic residual to 54 naming differences, first `+0xD4`.
That is evidence for the initial draw deficit, not a source match. It does not
prove a uniform function-wide ring shift or a forceable colour defect. No
instrumented output is retained as canonical source or earns matching credit.

Natural SDK bit extraction and packed-bitfield reads leave the retained shape
flat; packed count extraction adds instructions, and preserving the whole
packed flag value removes instructions through commoning. Bitfield writes
change the later draw sequence without closing it. The closing anchor-order,
named-y and plain-array controls do not improve the residual; extending the
local ring rotation to later flag sites also produces no gain. The natural
source hypotheses have stalled under ADR 0018. Resume with a source-authentic
explanation for the initial three extra draws and independent comparison-carrier
evidence, then re-map the later regions. A single global colour permutation
or another flag lattice does not address the measured mechanism.

The final workbench comparison calls this `allocation-mismatch`; its detailed
playbook reports a mixed register/schedule/structural residual and recommends
fixing structure before register classes. Its raw full-TU relocation comparison
also reports 22 site/name discrepancies against the zero-based target object.
The aligned 81-word count is therefore explicitly relocation-masked. Canonical
binding and linked owned-range proof remain required before any promotion;
the existing relocation count alone is not identity proof.

Validation: configured stock full-TU probes with the per-file multiplication
flag retained; aligned, residual, register and frame censuses; instrumented
full-TU fidelity and direct diagnostic-object scoring; workbench diagnosis;
full ROM verification through lane gates with the guarded fallback retained.
Private source, objects, decision records and rejected alternatives remain
available in the lane's scratch evidence. No forced compiler or object-byte
changes are part of this candidate.

<!-- plateau-handoff:overlay1AdvancePath:end -->
