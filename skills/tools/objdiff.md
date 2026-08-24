# objdiff in Mickey

Use objdiff-cli for a per-object match-percentage report over the whole
tree, not for reading individual instruction diffs -- that's `./diff.sh -m
<symbol>` (asm-differ). objdiff's target ("expected") side is a snapshot of
a previously `gmake verify`-clean `build/`, not the baserom directly: it
diffs *object files*, not raw ROM bytes.

```sh
./tools/setup_objdiff.sh          # fetch tools/objdiff/objdiff-cli (gitignored binary)
gmake -j8 && gmake verify
./tools/make_expected.sh          # snapshot build/ -> expected/build/
./tools/objdiff_report.sh         # summary + worst-40 by match %
./tools/objdiff_report.sh --full  # every unit
```

Re-run `make_expected.sh` any time `build/` changes and re-verifies, to keep
the target side current -- until then the report is comparing the current
build against a stale one, not against anything ROM-derived.

`objdiff.json` (committed -- paths and names only, no ROM content) lists one
unit per built object; `tools/objdiff_report.sh` regenerates it (and
`tools/objdiff_exclude.txt`, gitignored -- see `docs/tools.md` for why that
one isn't committed) from the current `build/` tree on every run. ~686 of
this project's C objects carry a Makefile `POSTPROCESS` override
(`trim_elf_section.py`, `normalize_elf_instructions.py`) and objdiff-cli's
report generator cannot parse the resulting section layout; those are
excluded. This is a known scope limit, not a bug to route around by
loosening a clean-room rule -- see `docs/tools.md`.
