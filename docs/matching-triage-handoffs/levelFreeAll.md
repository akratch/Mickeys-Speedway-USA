<!-- plateau-handoff:levelFreeAll:start -->
### `levelFreeAll` plateau handoff

- source: `src/main/level.c`
- score: 114/117 words
- frame: 0x28
- relocations: 36/36 offset/type/identity
- first mismatch: +0x13C
- trace proof: A hash-pinned IDO 5.3 UGEN build passed stock object fidelity for text, rodata, data, relocations, and symbols. Its source-line trace proved the baseline allocates pointer, mask, then scale at the one mismatching expression.
- retained gain: Semantically equivalent byte-offset arithmetic changes that order to mask, scale, then pointer. It makes the masked index exact and improves the body from 112/117 to 114/117 words without changing size, frame, calls, or relocations.
- residual: Three register-only words remain: the pointer and scale temporaries are exchanged. The required natural allocation order is mask, pointer, then scale. Do not repeat the 119-flag lattice, blind permutation, typed-pointer commutations, casts, or assignment forms.
<!-- plateau-handoff:levelFreeAll:end -->
