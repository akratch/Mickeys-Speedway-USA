<!-- plateau-handoff:overlay14CreateValue:start -->
### `overlay14CreateValue` plateau handoff

- source: `src/overlays/o014/overlay14CreateValue.c`
- score: 82/96 words
- frame: 0x28
- relocations: 15
- first mismatch: +0x54
- summary: Line-tag scheduling closes the initial LO16 pair; 14 positional words and one selector-call identity remain.
- base: `b708cbccdb55f54b2191293119c7fc659b4d9c83`
- boundary: overlay 14 `+0x6FC..+0x87C`, 384 bytes / 96 words, no
  padding or export; two local callers at `+0x3C0` and `+0x40C`
- configured result: exact geometry and frame `0x28`, 82/96
  relocation-aware positional words, 14 differing words, first workbench
  mismatch `+0x54`
- scheduler evidence: a native IDO 5.3 `as1 -R` capture was trace-off
  identical in `.text`, `.data`, `.rodata`, and relocation tuples. The first
  block contains independent slot-base and end-pointer HI16/LO16 chains. Their
  low halves are simultaneously ready and the earlier logical source line wins.
- strict gain: keeping `slot = gOverlay14Slots28; scan_loop:` on one physical
  line preserves C tokens and reverses the tied low-half choice, closing
  `+0x18/+0x1C` and improving the reproduced plateau from 80/96 to 82/96
- relocation proof: target and candidate each carry 15 records with all 15
  offsets/types aligned. The line tie lets preflight bind both initial data
  pairs, and spelling the exact matched overlay-14 asset-loader symbol closes
  its call. Fourteen identities now resolve; only the selector call at `+0xAC`
  remains unresolved because the shipped runtime table identifies
  `resident:+0x39DFC` while the synthetic target object names an overlay-14
  zero-VMA symbol. No identity was guessed.
- residual: the `+0x54` workbench row is the already-authenticated active-slot
  data addend in an unlinked candidate. The first source/codegen residual is
  the chosen-pointer web around `+0xA4`: target uses pool register `v1` and one
  `sp+0x18` home, while the candidate introduces `t3` and later spills the
  copied `slot` web at `sp+0x24`. The final value/count block also carries a
  two-word temporary-ring shift.
- exhausted controls: the prior 119-configuration flag lattice remains closed.
  A call/slot/switch physical-line tie was byte-flat. Direct one-pointer and
  nonvolatile-pointer forms grew the owner by four bytes. Assigning `slot`
  before the selector regressed to 11/96 and displaced nine relocation sites;
  an active-pointer carrier regressed to 76/96, and a scan-key carrier to
  74/96.
- bounded permutation: one ten-minute, two-thread, relocation-annotated batch
  improved its scratch score from 345 to 210 but found no zero. Its best forms
  used forbidden inert labels/guards or changed case semantics; the natural
  carrier results regressed in the real TU and were rejected.
- next lever: retain this source. Reopen only with a fidelity-gated producer to
  final-home trace for the chosen/slot pool web, or authoritative runtime
  naming for the `+0xAC` resident selector call. Do not repeat flags, physical
  line ties, generic permutation, pointer-direct, assignment-order, or carrier
  forms.
<!-- plateau-handoff:overlay14CreateValue:end -->
