<!-- plateau-handoff:overlay1FindType5ByKey:start -->
### `overlay1FindType5ByKey` plateau handoff

- source: `src/overlays/o001/overlay_001.c`
- score: 22/39 raw and normalized words
- frame: 48 bytes
- relocations: 1
- first mismatch: +0x1C
- summary: Canonical fallback-backed C remains 156 bytes with seventeen register-only residuals; the configured full-TU and actual compiler-input baseline agree, and actual-input self-context is unchanged. The 119-flag lattice, explicit array/cursor split and scoped record-key carrier are exhausted. Separately retained, source-reviewed search output reproduces 156 bytes/frame48 with four register-only differences first at +0x20, the same exact relocation and unchanged neighboring code in a configured full-TU transfer. That diagnostic retains an unused old key declaration, a used Boolean predicate carrier and reversed loop comparison; their effects have not been isolated. No new matching credit or canonical source adoption. ADR 0017 permits a separately pinned causal packet, not another undirected sweep.
<!-- plateau-handoff:overlay1FindType5ByKey:end -->
