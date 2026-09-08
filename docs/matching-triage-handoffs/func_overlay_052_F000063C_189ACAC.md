<!-- plateau-handoff:func_overlay_052_F000063C_189ACAC:start -->
### `func_overlay_052_F000063C_189ACAC` plateau handoff

- source: `src/overlays/o052/overlay52TailB.c`
- score: 1597 differing words
- frame: 0x118
- relocations: 310
- first mismatch: +0x130
- summary: 6736/6748 bytes; 64 ordered calls; exact frame. Grouped recurrence recovered; next: prove remaining branch-latch and address-carrier differences.

The September 8 decompiler-assisted packet retains attempt 39 as ordinary,
guarded C. This is a compiled structural plateau, not an object or ROM match.
No symbol-table, atlas, scoreboard, or matching-credit change is claimed.
The source's GLOBAL_ASM fallback remains canonical.

**Owned extent and measured result**

- Identity: overlay 52, text offsets 0x63C through 0x2098, ROM offsets
  0x189ACAC through 0x189C708, exclusive ends. The owned executable extent
  is 6748 bytes, or 1687 words; none of the deficit is padding.
- Re-derived configured baseline: 6272 bytes, 1568 words, 476 bytes short;
  1672 raw and relocation-masked positional differing words, first +0x30,
  frame 0x118, and 208 candidate relocation records against 314 runtime
  target records. The inherited 314 figure described the target surface.
- Retained result: 6736 bytes, 1684 words, 12 bytes short. It recovers
  116 words, or 464 bytes, of the baseline size deficit. The diagnostic
  comparison has 1597 raw and 1595 relocation-masked differing words,
  first +0x130. Frame 0x118 agrees; 310 candidate relocation records all
  resolve, but only 27 sites agree in offset, type, and identity with the
  314-record runtime target. Relocation exactness fails.
- All 64 direct calls, covering 31 callees, agree in identity and order.
  This is stronger than a call multiset, and weaker than matching the call
  offsets or the linked executable bytes.
- The old scalar minimum is preserved at attempt 5: 1674 words, 1451
  positional differing words, frame 0x120. It has duplicate time divisions
  whose extra words cancel missing recurrence words. Attempt 40 improves
  the retained candidate's positional count by one word but substitutes
  duplicated stores for the target addition edges. Neither displaces the
  better-understood operation graph of attempt 39.

**Recovered evidence and source**

The raw extracted assembly collapses external calls and section-zero globals.
Decoding Mickey's runtime relocation tables recovers their real overlay,
section, and symbol identities before drafting C. The stock workbench target
contains only 162 relocation records, so it cannot prove this function's full
runtime relocation surface. A section-qualified assembly adapter was checked
against the owned ROM slice: 1687 words with zero differences and all 314
relocation tuples exact. That is an importer self-check, not a C match.

The full generated context did not parse in m2c because of unrelated header
types. A narrow context using this TU's authenticated fields and call names
produced a typed draft. Raw drafts, resolved assembly, and all comparison
artifacts remain ignored. No draft or instruction listing is tracked.
The actual asm-processor output was preprocessed with the configured compiler
and accepted by the context comparator when compared with itself. This audit
was completed after the initial reconstruction, not before the baseline;
it does not retroactively establish the required preparation order.

Recovered source includes the 16-byte texture entry layout, the racer's
accessed widths and field offsets, the menu object's float fields, and the
real descriptor/time-split interfaces. The template extent includes its
ninth, terminating entry. The remaining helper return and argument widths
were checked against Mickey callee evidence. Array indexing restores shared
time quotient/remainder computation and the target texture-copy loops.
Icon coordinates are sampled before the three count draws. The clock-hand
calculation now explicitly performs its 32-bit wrap before signed division.

The opening recurrence now advances the remainder and groups of four with
an explicit exit. A counted spelling of the already expanded draft had been
unrolled a second time; the explicit exit avoids that expansion. Its opening
call interval grows from 76 to 106 words against the target's 108, recovering
30 words in the intended region. Each positive update count still performs
exactly that many f32 subtraction/multiplication/addition steps and global
stores; nonpositive counts perform none. There is no extra final floating
operation, invented guard, or padding. This reconstruction does not prove
that the original source manually grouped the updates.

**Remaining deficit and stopping evidence**

Call-delimited extent differences are now confined to the opening recurrence
interval (two words short), item replacement (one short), item restoration
(two short), the main icon draw (one extra), and the final count draw
(one extra). Equal extent elsewhere is not byte identity.

The recurrence still has two ordinary backedges where the target uses likely
backedges with duplicated addition edges. Its FP carrier graph differs.
The initial paired halfword stores also form separate high-address loads,
where the target shares them. Other relocation-count differences concern
address formation for fields of the same authenticated entry arrays; there
are no unresolved callee or global identities. Remaining icon and item
intervals include different address/value lifetimes and spill placement.
No wrong arithmetic literal was found in the workbench constant bucket;
its aligned constant findings were stack-displacement differences.

Attempt 39 was the last accepted structural gain. After the single-word
positional alternative at attempt 40, five consecutive refinements left the
retained residual and the remaining causal explanation unchanged:

- 41: separate old-value and delta locals add copies, six words, and a
  larger frame; additional relocation sites are not the target sites.
- 42: one accumulator for delta and result retains the wrong latch and
  does not improve the residual.
- 43: a forward completion exit changes scheduling but leaves the same
  opening extent and relocation deficit.
- 44: a typed float cursor removes target loads/stores and grows the frame.
  Its lower positional count is a regression in the structural comparison.
- 45: counter initialization on the nonpositive path is byte-flat.

These refinements recover no new call/global identity and do not resolve a
new mismatch mechanism. Stop on that stall, not on the total attempt count.
Earlier named-step, simple loop-condition, countdown, and pointer-hoist forms
are preserved as exhausted inputs. The remaining recurrence issue is not a
missing update operation. Do not restart those spellings without changed
evidence. No flag lattice, permuter, instrumented compiler, invented volatile
access, global regrouping, or post-compile instruction editing was used.

The concrete next lever is a faithful comparison of the producing compiler
phases for the retained recurrence and a Mickey-proved sibling with the same
branch-latch shape, plus an address-context check for the paired fields.
First establish whether source def/use structure or compiler context owns
those decisions. This requires a separately authorized mechanism; it is not
permission to change shared flags, symbols, alignment, or data placement.
The workbench verdict remains structure-mismatch, routed through
structure-buckets: context parity and the spill-slot census, after the
completed constant and call/global identity audits.

**Artifacts, commits, and validation**

The packet base is cbd755d9b2e828b5f4c05a4efe81d70353d205cb.
Progress commits are 0f5059c5a702933b4a3c3faf6fdc1fdfa8b61d17,
c45403aa, and 0f914be9; the commit containing this handoff banks the grouped
recurrence and final interface corrections. Only the owned source and this
shard change in that final source commit.

Ignored evidence is under build/whale4/: baseline source/object and reports,
attempt snapshots through a045, the retained a039 source/object/diagnostic
ELF, per-attempt rationale and measurements, runtime identities, the target
adapter self-check, compiler-input self-comparison, and recurrence review.
The standalone link is a stored-addend diagnostic, not a synthetic-address
promotion or a canonical linked-ROM proof. Every meaningful alternative is
preserved, including the scalar minima.

Commands used include generate_ctx.sh, mips_to_c.sh with the typed local
context, tools/wb_compare.sh with the configured full-TU candidate,
decomp-workbench diagnose/compare/guide structure-buckets,
tools/candidate_context.py, and the ignored measure.py/call_buckets.py
adapters. tools/finalize_plateau.py generated the guarded plateau metadata
and this shard's metric header; the richer evidence is recorded here.

Source gates and all 243 NON_MATCHING TU builds passed at each banked
checkpoint. gmake verify repeatedly fails in unchanged overlay94UpdateController
at the assigned base: undefined func_800254FC, func_8002565C, func_8002A878,
and func_8002A910, plus resident R_MIPS_26 overflows for func_8005ABA8,
func_8005AF14, func_80019AB8, and func_8002B040. There is no canonical linked
ELF available for an owned-range extraction. This remains outside this
lane's ownership. Closing gate receipts are retained alongside the packet;
no full-ROM success or newly matched bytes are claimed.

<!-- plateau-handoff:func_overlay_052_F000063C_189ACAC:end -->
