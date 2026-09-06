<!-- plateau-handoff:overlay1DispatchMode:start -->
### `overlay1DispatchMode` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 199/199 words
- frame: 0x28
- relocations: 61
- first mismatch: +0x2C
- summary: All instruction bytes and eight table destinations retained; 58/61 identities proved. Remaining: table HI/LO and generated cross-overlay call +0x48. No exact credit.

The eleven external call sites now name canonical resident and overlay
definitions with their pointer/integer ABI, authenticated by the shipped
runtime records and section-qualified linked definitions. Eight executed
object-local aliases preserve the previous overlay call carriers; the three
random calls use the existing resident `mathRnd` alias. Every instruction in
the configured full TU, all relocation offsets/types, and every non-owned
relocation remain unchanged. All eight compiler table destinations agree
with the retail table relative to this function's entry.

The current generated-name resolver cannot prove the cross-overlay, interior
mixed-TU call at `+0x48`; the source names its actual canonical definition.
The two table records also remain unresolved. The existing positive-anchor
externalization edits instruction fields and is not a valid exact-C route.
Retain the fallback: source bindings, diagnostic instruction equality and
fallback ROM verification do not establish linked C equality. No allocation,
flag, or permutation attempt was made by this binding repair.
<!-- plateau-handoff:overlay1DispatchMode:end -->
