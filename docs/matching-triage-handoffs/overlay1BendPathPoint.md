<!-- plateau-handoff:overlay1BendPathPoint:start -->
### `overlay1BendPathPoint` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 82/107 words
- frame: 0x30
- relocations: 6
- first mismatch: +0xC
- summary: exact geometry; parameter-home scheduling and three coupled integer/pointer register webs remain after the full flag lattice

#### tu2-o1tail: the u8 parameter's spill slot is the residual, and it is not source-reachable so far

Re-measured at the assigned base: 25 masked words, 107 of 107 instructions,
0x30 frame exact, first mismatch +0xC. Two mechanisms were isolated and both
resisted every form tried.

First, the spill slot. The target spills the `index` parameter to the byte of
its own incoming argument home (sp+59, the low byte of a2's slot at sp+56) and
reloads it there after the `overlay1GetPathReloc` call. The candidate's
`volatile u8 localIndex` hack produces the same store/reload pair, but into a
separate local at sp+25. Removing the hack so IDO spills the parameter itself
does not work: the instruction count drops to 106 and the residual explodes to
104 words. Also eliminated: `index = *(volatile u8 *)&index` after the call
(108 instructions, 105 words), a `volatile u8 index` parameter (108, 103), and
dereferencing the address of the volatile local (108, 106).

Second, the index-block colours. The target colours currentIndex v1,
previousIndex a0 and the path-count carrier a1; the candidate has previousIndex
v1, currentIndex a1, count a2. Eliminated: writing `currentIndex = index` in
the else branch where index is known zero, which does reproduce the target's
`move v1,a2` but costs 97 words overall; ordering the else branch with
currentIndex first (flat at 25); folding the decrement into
`previousIndex = path->count - 1` (26); and spelling the decrement as
`previousIndex = previousIndex - 1` (flat).

Three consecutive attempts on each mechanism produced no better residual and no
new identity, which is the stall. Next lever: the parameter-home spill is an
allocator decision, so it needs ugen ring or emit-order instrumentation rather
than another source form.
<!-- plateau-handoff:overlay1BendPathPoint:end -->
