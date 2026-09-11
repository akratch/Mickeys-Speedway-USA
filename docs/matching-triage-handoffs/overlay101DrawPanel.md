<!-- plateau-handoff:overlay101DrawPanel:start -->
### `overlay101DrawPanel` plateau handoff

- source: `src/overlays/o101/overlay101DrawPanel.c`
- score: 36 differing words
- frame: 0x178
- relocations: 18
- first mismatch: +0xB4
- summary: Exact 268 words and 0x178 frame; two uopt region boundaries and one order-only move took 79 masked words to 36. The residue is NOT colouring: 240 single-web globalcolor forces leave it at 36 or worse. 2026-09-11, lane p2-close, measured and it inverts the older reading: the shipped frame's declared block ENDS at +0x64 and is 276 bytes where this candidate's ends at +0x58 and is 288, so the target declares three FEWER homed scalars, not a seventh more, and its compiler-temp pool is eleven cells against this candidate's eight. The eight immediate-only words are exactly the three spill displacements that follow from that, and the t1 census of 4 against 12 is the same fact seen from ugen: globalcolor here holds t0 through t5, leaving a four-register expression ring, while the shipped code spends t1 as an expression temp, so it carries one p1 web fewer. Decision variable: the declared-symbol count. Reopen with a source form that drops one declared scalar without adding a reload; naive inlining costs 48 to 436 bytes because the work struct's address escapes.

#### 2026-09-11, lane p2-close: the block is SMALLER, not larger, and the older reading inverts

Baseline reproduces: 1,072 bytes, 268 of 268 words, delta 0, masked 36, first
mismatch +0xB4. Aligner: 234 byte-exact, 16 register naming, 8 immediate-only,
13 really different.

**The eight immediate-only words are three spill displacements, and naming them
settles the direction.** `frame_census` puts the target's spills at +0x48 with
one store and three loads, and at +0x5C and +0x60 with one store and one load
each; this candidate's are at +0x5C with one store and three loads, and at
+0x50 and +0x54. Read against the object they are the same three values in the
same pattern: `x + work.width` carries the three loads, `y + 2` and `x + 14`
carry one each. Only the displacements differ, which is exactly eight words.

**Therefore the target's declared block ends at +0x64 and is 276 bytes, where
this candidate's ends at +0x58 and is 288.** Compiler temps sit immediately
below the declared block -- confirmed here by adding padding declarations and
watching the temp slots move with the block bottom -- so the target touching
+0x60 puts its block bottom no lower than +0x64. `work` is pinned to +0x64 on
both sides by the four addresses passed to BuildIntensityColors, so the target
declares three fewer four-byte objects than this candidate, and by
`frame = round8(56 + temps + block)` its compiler-temp pool is eleven cells
against this candidate's eight.

**That inverts the older record.** "A seventh declared local would supply the
missing ring web but moves the frame" is measuring the right symptom in the
wrong direction: the shipped source declares fewer homed objects, not more, and
three of this candidate's declarations are compiler temporaries there.

**The ring census says the same thing from the other end.** The instrumented
ugen's caller-saved free list for this procedure is only t6, t7, t8 and t9:
globalcolor takes v0, v1 and t0 through t5, and ugen strikes those from the
pool at procedure entry. The shipped code spends t1 as an expression temporary
in the mode-1/3 arm, so it runs a wider ring, which means one fewer p1 web.
One fewer p1 web and three fewer declared scalars are the same fact. The
earlier "t1 twelve times against four" census was right and its reading --
"one more web in the expression ring" -- was backwards.

**Measured this pass, every cell at delta 0 unless stated.** One through five
unused pointer declarations after `bottom` give frames 0x180, 0x180, 0x188,
0x188 and 0x190, so the temp pool is rigid at eight cells for this declaration
set and no padding edit reaches the target's layout. Deleting any of `x`, `y`,
`right` or `bottom` and inlining its expression costs 48 to 436 bytes, because
`&work.full` escapes and `work.width`/`work.height` therefore reload after
every call -- so L120's "a commoned load deletes at zero cost" does not apply
to this struct. Splitting `width` and `height` out of the struct into ordinary
locals and dropping the `right` and `bottom` declarations keeps size and frame
exactly, at 87 masked, and is the only form found that does. A full
reconstruction as nine separate scalars above a trailing `records[20]` array
reproduces the target's spill pattern exactly -- one three-load slot and two
one-load slots, in that order -- but lands eight bytes low, because uopt then
makes nine temporaries where the target makes eleven. Reusing `x` and `y` for
the mode-1/3 argument expressions is byte-inert. Peephole-removable no-op masks
on `panel->mode`, `panel->intensity` and `panel->alpha` all move the ring but
all regress, to 91 through 252.

**Reopen condition.** A source form that removes one declared scalar without
adding a reload, or that makes uopt create two more temporaries against a
276-byte block. The decision variable is the declared-symbol count, not a
colour and not a spelling.

<!-- plateau-handoff:overlay101DrawPanel:end -->
