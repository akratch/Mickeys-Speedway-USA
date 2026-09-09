<!-- plateau-handoff:overlay43FilterImage:start -->
### `overlay43FilterImage` plateau handoff

- source: `src/overlays/o043/overlay43FilterImage.c`
- score: 27/43 words
- frame: frameless
- relocations: 0
- first mismatch: +0x4
- summary: Coalescing pixel[0] into sum makes the temp lane exact and cuts 33 to 16 words; four accepted p2 color forces leave only a two-word schedule swap.

Fresh evidence on base `dc3afd3399fd37af8099a2854d0ffa7b57422964`:

- ownership remains overlay 43 `+0x1378..+0x1424` / ROM
  `0x188B348..0x188B3F4`, exactly 172 bytes with separate three-word padding
  after the owner. Two local JUMP records at `+0x218/+0x24C` are the only
  inbounds; the function has no export or outbound relocation;
- configured stock IDO `-O2 -mips2 -32` emits the exact 43-instruction,
  frameless, zero-relocation extent. The retained candidate is 27/43 words,
  first mismatch `+0x4`; the linked trial independently reports 16 differing
  words inside the owner and zero bytes outside it;
- initializing `sum` from `pixel[0]` and then adding `pixel[1]` removes the
  separate `first` web. This is the retained strict gain from 10/43 to 27/43
  words. The complete 18-slot temporary lane becomes identical, so the
  remaining register residual is confined to the UOPT pool;
- a hash-bound allocator receipt maps the retained Ucode to procedure zero.
  The traced compiler passes `.text`, `.data`, `.rodata`, relocation, and
  symbol fidelity against stock. It records eight integer decisions, all in
  p2, and no p1 or floating-point decision;
- the diagnostic force
  `p2:w6=c4,p2:w9=c5,p2:w12=c6,p2:w57=c2` is accepted at all four sites. It
  rotates row, column, sum, and the post-decrement scratch to the target pool
  registers and improves the object to 41/43 words; only the mask/count
  constant order at `+0x7C/+0x80` remains. This proves color reachability but
  is not a match and no forced object enters the canonical build;
- unsigned counter types, unsigned post-decrement casts, an inline named
  scratch carrier, and a statement-separated carrier are byte-identical to
  the retained candidate. Narrow signed counters regress to 48 instructions.
  The earlier 119-row flag lattice and generic permutation remain closed and
  were not repeated.

Next lever: obtain new source-authentic evidence that forms or numbers the
post-decrement scratch before the row/column/sum p2 cycle. Do not repeat the
four flat carrier/type forms, narrow-counter probe, forced-color diagnostic,
flag lattice, or permutation.
<!-- plateau-handoff:overlay43FilterImage:end -->
