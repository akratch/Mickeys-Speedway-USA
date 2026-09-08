<!-- plateau-handoff:overlay11UpdateMenu:start -->
### `overlay11UpdateMenu` plateau handoff

- source: `src/overlays/o011/overlay11UpdateMenu.c`
- score: 299/301 words
- frame: 0x48
- relocations: 102
- first mismatch: +0x138
- summary: Zero new attempts: prior ten order/line forms exhausted; unchanged spill pair. Reopen only with source-authentic spill-dependency evidence.

Revalidated 2026-09-08 on assignment base
`419037148f512f2d38042f3a17ad4e2589db0768`. The retained C remains
`NON_MATCHING`, with the original assembly fallback.

- Ownership: Overlay 11 text `+0x1398..+0x184C`, ROM
  `0x1869BE0..0x186A094`; 1,204 executable bytes / 301 words, no owned padding.
  The candidate text section has 12 additional bytes of compiler alignment;
  they are outside this function and earn no credit.
- Configured baseline and metadata reproof: 285/301 raw positional words
  (16 differ, first `+0x14`), 299/301 relocation-masked positional words
  (2 differ, at `+0x138` and `+0x140`), zero size delta, frame `0x48` on both
  sides. The two differences exchange the index and handle spill stores
  across the same call. All register assignment lanes agree.
- Fresh function preflight authenticates 102/102 relocation offsets, types,
  identities, and addends against the overlay runtime table, with no
  unresolved identities. There is one local inbound at module `+0x930` and
  no exports. Synthetic names in the extracted target object are not the
  runtime relocation authority.
- Workbench comparison verdict: `allocation-mismatch`, routed guide
  `pool-position`. Its aligned diagnosis instead reports mixed constants and
  scheduling, `playbook=constant-audit`, and `lever: none-known`. Those early
  constant/identity warnings come from the extracted object's incomplete
  relocation representation: the runtime preflight resolves the full surface.
  The remaining two non-relocation words are the already-recorded spill pair;
  the generic pool-position route supplies no evidence of a new allocation
  defect. No constants, flags, identities, or allocation inputs changed.
- Similarity oracle: the best hit is `joys_handle_retrace` (Jaccard 0.0663);
  `menu_options_loop` scores 0.0603. These are weak structural hints, not
  donors. The local matched two-option and five-option menu siblings use the
  already-investigated pointer/index loop and supply no new spill dependency.

Stopping evidence: **zero new source attempts**. The last recorded strict
source gain was the natural declaration/lifetime repair in the earlier
identity pass. The latest source/handoff commit,
`c7ac01796dbcd314c79f2bf7a30c0e00e5d261c1`, explicitly records ten later
non-improving declaration-order and physical-line forms, plus a byte-inert as1
trace: both stores have the same call line, dependency, and zero aftercycles,
and the handle store ranks first. The retained notes also close the prior
flag lattice, explicit-handle form, loop/scope forms, and bounded batch.
The current baseline reproduces that exhausted state. ADR 0018's early
exhaustion clause applies; this run did not spend three attempts repeating
mechanisms whose inputs have not changed. Baselines, metadata migration, and
reproofs are not source attempts.

Next concrete lever: obtain source-authentic evidence for a changed
**pre-call spill dependency/emission order**, then pin a new reopen packet.
A paired ugen emit-provenance / as1 selection receipt should identify the
first differing spill-production decision while preserving the authenticated
callee ABI, stack homes, loop behavior, and relocation surface. Another
source-line, declaration-order, loop-form, or flag sweep without changed
inputs does not meet that bar. No new such source spelling is established here.

The structured source marker was moved from its legacy inline position to EOF
for `tools/finalize_plateau.py`. Recompilation proves the entire candidate
text section and relocation section unchanged from the archived baseline.
The C body and its fallback are retained; there is no new exact C credit.

Evidence is retained under ignored `build/wb/o11-menu/` (baseline C/object,
comparison and preflight reports, raw and masked first-mismatch receipt) and
`build/wb/overlay11UpdateMenu.*` (current comparison, diagnosis, and preflight).
Commands run: `tools/wb_compare.sh --summary-json overlay11UpdateMenu`;
`decomp-workbench diagnose` on the target and configured full-TU candidate;
`decomp-workbench guide allocation-mismatch` and `guide constant-audit`;
`tools/function_preflight.py overlay11UpdateMenu --json`;
`tools/skeleton_scan.py similar --target 11:+0x1398 --top 5 --json`;
`tools/finalize_plateau.py` with the measured fields above. Validation receipts
are retained with the ignored evidence.

Validation: `gmake verify`, `gmake cleanroom`, and `gmake check-docs` pass;
the structured handoff audit reports no issues. The linked ELF's owned
1,204-byte range was extracted and byte-compared with those same baserom
offsets: exact **assembly fallback only**. The rebuilt US ROM SHA1 is
`507341c0a40ca3e9a7cee969b396ee53facfb548`. No matching status, symbol-table
line, atlas ownership, or scoreboard credit changed. This handoff and the
source metadata migration are the only tracked changes in the lane commit.
<!-- plateau-handoff:overlay11UpdateMenu:end -->
