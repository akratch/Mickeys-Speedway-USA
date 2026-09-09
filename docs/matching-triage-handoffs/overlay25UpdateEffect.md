<!-- plateau-handoff:overlay25UpdateEffect:start -->
### `overlay25UpdateEffect` plateau handoff

- source: `src/overlays/o025/overlay_025.c`
- score: 132/259 words
- frame: 0xC8
- relocations: 25
- first mismatch: +0x0
- summary: Fidelity-clean proc-1 trace emits no source semantics or virtual/final stack homes; no natural lifetime edit is uniquely justified.

Fresh authorized evidence on base
`b9bb22f036d55f90cefcd46f00a0f71b79e8c8b4`:

- the current guarded body and prior structured ledger are both pinned by
  `72efc7e5ab2a2de533c895a4b391092102121e7e`; the body was not changed in
  this pass;
- fallback and adjacent-symbol evidence uniquely bound overlay 25 `.text`
  `+0x17C..+0x588`, ROM `0x1879E04..0x187A210`, with no target padding before
  `overlay25SetVectorFlags`. The ABI remains
  `void (Overlay25Object *, s32)`; ORT entry 1274 and resident relocation 116
  authenticate the sole inbound call from `func_8000AEEC+0x274`;
- configured IDO 5.3 `-O2 -mips2 -32 -Wab,-r4300_mul` again emits exactly
  1,036 bytes / 259 words, with 132 positional matches, 127 raw and masked
  differences, and first mismatch `+0x0`. The candidate frame is `0xC8`
  versus the target's `0xA0`;
- target and candidate each expose 25 relocations. Twenty-four sites align by
  offset and type, but only two candidate identities resolve and align; 23
  identities remain unresolved, so relocation evidence remains partial;
- the retained named Ucode stream and complete UOPT procedure index map this
  symbol uniquely to procedure 1 of 3, with 34 allocator decisions. The
  stock candidate object hash is
  `63dd21e7251ec739dd69e8d7ff03b5446fbcb56d6c9eb4acbb5852c950e2eb55`;
- the procedure-scoped capture passes `.text`, `.data`, `.rodata`, relocation,
  and symbol fidelity against that stock object. It records 30 integer and 39
  floating-point temporary births for procedure 1;
- the producer capability audit reports zero direct `source_semantic`,
  `virtual_offset`, or `final_offset` fields. Logical lines, itable symbols,
  allocator ordinals, registers, and opaque words are not source attribution
  or stack-home evidence, so the observed `0x28` frame excess cannot be tied
  uniquely to a natural C lifetime;
- no source experiment was authorized by the trace. The exhausted 119 flags,
  declaration/scope/order variants, fake guards, and permutation were not
  repeated. The donor oracle remains non-actionable (best size-compatible
  similarity `0.056`, with no corresponding permitted DKR/JFG source).

Next lever: productize a hash-pinned compiler producer hook that carries an
explicit source/expression identity into the selected allocator web and emits
its virtual and final stack homes. Repeat this single fidelity-gated capture
only after that producer surface exists; do not infer a lifetime from line or
register order.
<!-- plateau-handoff:overlay25UpdateEffect:end -->
