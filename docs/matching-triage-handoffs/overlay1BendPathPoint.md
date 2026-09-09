<!-- plateau-handoff:overlay1BendPathPoint:start -->
### `overlay1BendPathPoint` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 86/107 words
- frame: 0x30
- relocations: 6
- first mismatch: +0xC
- summary: previous/current/path/next is the declaration order that lands both spilled stack homes (25 words to 21); the rest is the prologue byte spill and the index block's pool colours

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

#### lm-o1tail: the parameter-home spill is source-reachable after all

The previous record called the spill slot an allocator decision needing ugen
instrumentation. It is not: `*(u8 *)&index = index;` written before the
`overlay1GetPathReloc` call reproduces it. Taking the parameter's address makes
IDO keep `index` in memory, and the redundant store lands on the byte of the
parameter's own incoming argument home -- the exact slot the target uses -- with
107 instructions and the 0x30 frame both held. It also puts the third argument's
save into the target's out-of-order position, second rather than third, so the
prologue's first four words become exact.

Two things it does not yet buy. The store is scheduled before the call rather
than into its delay slot, where the target puts it and where this form puts the
selector mask instead; and the reload lands in a ugen ring temp rather than the
argument register the target keeps it in, which shifts the index-block colours
downstream. Positionally that form scores 48 words against the retained hack's
25, so the `volatile u8` local stays as the candidate -- but the hack cannot
reach the target's slot at all, because its local is allocated in the local area
(sp+25) and only the parameter itself lives in the argument area. The address
form is the structurally correct route and the hack is not.

Measured and eliminated from this base, do not repeat: `volatile u8 index` as
the parameter, with the value read directly, read once into an `s32`, read once
into a `u8`, and read before the call (108 to 110 instructions); a plain
self-assignment and `*&index = index`, both eliminated before codegen (106);
`*(volatile u8 *)&index = index` before, after, and on both sides of the call
(109 to 111); a `u8 *` pointer local carrying the address (107, same 48-word
basin as the direct address form); reading back through `*(u8 *)&index` or a
named `u8`/`s32` local after the store; the volatile local declared first and
last in the list (frame 0x38 and 0x40 -- its position moves the frame); and the
plain no-hack form, which is 106 instructions because it reloads from the
argument home without ever storing to it.

Next lever: the delay-slot choice between the parameter store and the selector
mask, and then the reload's carrier. Both sit on the same three instructions.
<!-- plateau-handoff:overlay1BendPathPoint:end -->
