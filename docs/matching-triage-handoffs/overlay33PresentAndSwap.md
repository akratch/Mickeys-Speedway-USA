<!-- plateau-handoff:overlay33PresentAndSwap:start -->
### `overlay33PresentAndSwap` plateau handoff

- source: `src/overlays/o033/overlay33PresentAndSwap.c`
- score: 18/39 words
- frame: 0x18
- relocations: 18
- first mismatch: +0x8
- summary: Candidate trace proves seven temp births. rcpFast3d fixes all 18 relocation identities; 21 masked differences remain without target temp events.

#### 2026-09-11, lane `f9-small`: L94 opens the first address web; the second region needs the stored value forwarded

With `index = gOverlay33BufferIndex` for the first region and
`gOverlay33BufferIndex ^= 1; gOverlay33ActiveBuffer =
gOverlay33BufferRefs[gOverlay33BufferIndex]` for the second, the first
region is exact (the global's address becomes a uopt web coloured `$t0`, as
the target has, instead of ring temp `$t6`) and only the second region
differs: the candidate re-reads the global after the store (`lw $t1,0($t0)`,
+4 bytes) where the target uses the xor result (`sll $t2,$t9,2`). The
retained `index = g ^ 1; g = index; refs[index]` forwards the value but
loses the address web (21). Reading the global directly in any of the three
first-region call arguments regresses to +4..+16 bytes. Decision variable: a
second-region spelling that both keeps two accesses of the global in the
region (address web) and reads the toggled value from the register.
<!-- plateau-handoff:overlay33PresentAndSwap:end -->
