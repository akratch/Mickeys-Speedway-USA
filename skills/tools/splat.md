# splat and repository layout in Mickey

Treat `mickey.us.yaml` and `config/overlays.us.json` as maps to read, not files
to rewrite during a one-function matching task. For resident code, the YAML
maps ROM offsets to source/assembly translation units. For overlay code, use
the tuple `(overlay, section, offset)` from the overlay manifest; a synthetic
VMA is not a unique identity.

Do not run splat directly. `gmake setup` and `gmake extract` own regeneration,
including stale-assembly pruning. Those commands require explicit human
authorization under this workstation's validation rules and may change
generated ignored files.

Do not turn an `asm` segment into a `c` segment, edit YAML, or rename a symbol
as a side effect of matching one function. Propose any translation-unit split
or layout change separately with the exact affected paths.
