<!-- plateau-handoff:overlay83BuildBatch:start -->
### `overlay83BuildBatch` plateau handoff

- source: `src/overlays/o083/overlay83BuildBatch.c`
- score: 71/168 words
- frame: 0x90
- relocations: 5
- first mismatch: +0x0
- summary: Initializer scope cuts frame 0x98 to 0x90 (target 0x78); count/scale allocation remains; relocs 3/5 offset/type, 0/5 static identity.
- baseline: Exact 672-byte size and 168-word body; 71 words align and 97 differ. The original candidate frame was 0x98 against the target 0x78.
- retained gain: Moving the 24-byte linked initializer into its only conditional lifetime reduces the candidate frame by eight bytes to 0x90 without changing size, word score, or behavior. The assembly fallback remains active.
- relocation proof: The runtime table and candidate each have five records. Three offsets/types align; all five candidate identities remain unresolved, so `function_preflight.py` correctly fails closed and this is not promotable.
- bounded attempts: Ten coherent scope, setup-order, scale-width, direct-conversion, post-decrement-loop, allocation-result-lifetime, and explicit-decrement forms were compiled and compared. No flag lattice or generic permutation batch was run.
- donor evidence: JFG `lightShowInit` at `src/overlays/o64/overlay_64.c` (assembly body `asm/nonmatchings/overlays/o64/overlay_64/lightShowInit.s`) is a 716-byte, 0.284 masked-skeleton analogue. It confirms the allocate, record-initialize, world-transform, optional-link flow and the 0x78 frame, but supplies no C body and no Mickey relocation identities; use it only as structural context.
- reopen condition: Resume only with new evidence for the original count/scale temporary lifetimes or the five runtime identities. Do not repeat these ten forms or the historical 119-flag/permuter searches.
<!-- plateau-handoff:overlay83BuildBatch:end -->
