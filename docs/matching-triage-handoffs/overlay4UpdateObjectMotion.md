<!-- plateau-handoff:overlay4UpdateObjectMotion:start -->
### `overlay4UpdateObjectMotion` plateau handoff

- source: `src/overlays/o004/overlay_004.c`
- score: 222/230 words
- frame: 0x60
- relocations: 11
- first mismatch: +0x3C
- summary: fidelity-clean allocator trace proves zero-cost v1/a0 ties but has no source-attributed webs; two natural carrier forms are flat

#### Fresh allocator and relocation reproof (2026-08-31)

- Base `ff35da3630776733b7abedd72e6a48d47c864bbb` reproduces the configured full-TU candidate at 920 bytes / 230 words, 222/230 relocation-masked words, and the exact `0x60` frame. The eight normalized residual words are register allocation only: three uses of the mode web and five uses of the threshold/delta webs. The first raw differing word is `+0x3C`, a relocation-bearing call word; the first non-relocation/register difference is `+0x44`.
- The candidate emits all 11 `R_MIPS_26` records at the target offsets and types. Static comparison resolves 10/11 identities. Current relocation synthesis proves that the call at `+0x354` carries the required stored cross-overlay linker value `0xf0000000`; the runtime target table independently authenticates its destination as Overlay 36 `+0x7B0`. Shared synthetic VMA evidence does not establish the candidate's stable cross-overlay identity, so the handoff deliberately retains the 10/11 static-identity result.
- The IDO 5.3 UOPT source at the campaign-pinned revision generated hash `b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f`. Instrumented procedure 1 maps uniquely to this symbol and preserves full `.text`, `.data`, `.rodata`, relocation, and symbol fidelity. Its 13 allocator decisions include zero-cost colour ties for web 48 (natural `v1`) and web 50 (natural `a0`), matching the threshold/delta allocation basin. The trace reports zero source-attributed webs, so it does not justify forced-colour or synthetic carrier source.
- Two source-authentic probes stayed exactly at 222/230: reversing the two equivalent threshold comparisons, and capturing `config->threshold` in an ordinary signed local before both comparisons. Functional baseline C is restored. No flag sweep or permutation was repeated because neither probe produced a strict gain.
- Next lever: first obtain a fidelity-clean capture with calibrated direct source-semantic or producer-line attribution for the two tied webs, then test only the source formation named by that trace. Exact relocation promotion must additionally prove the Overlay 36 endpoint from the linked/runtime candidate rather than infer identity from a shared synthetic VMA.
<!-- plateau-handoff:overlay4UpdateObjectMotion:end -->
