<!-- plateau-handoff:func_overlay_060_F0000334_18BA10C:start -->
### `func_overlay_060_F0000334_18BA10C` plateau handoff

- source: `src/overlays/o060/overlay60Prefix.c`
- score: 2510 differing words
- frame: 0x198
- relocations: 864
- first mismatch: +0x4
- summary: NON_MATCHING: frame and every declared-local home now byte-exact (enabled 0x13C, text 0xBC, glyph 0xB4); mnemonic census within 45 of 2789 and the -12 is 45 insertions against 48 deletions, not one hole; the whole remaining residual is one uopt address-CSE that keeps &gOverlay60Data0A8 in a saved register for the entire target and rematerializes it 24 times here.

This packet reopened only decompiler-assisted structural reconstruction from
Mickey's extracted assembly. The prior size-deficit plateau was read before
editing, and the configured baseline was reproduced at assignment base
`cbd755d9b2e828b5f4c05a4efe81d70353d205cb`.

Measured outcome (configured full TU, compiled `NON_MATCHING` C):

- Owned target: overlay 60, text `[0x0334, 0x2EC8)`, ROM
  `[0x18BA10C, 0x18BCCA0)`, 11156 executable bytes / 2789 words.
  The terminal return delay slot is included; section padding adds no credit.
- Baseline: 3484 bytes / 871 words, 2775 raw and relocation-masked differing
  words, first mismatch `+0x8`, frame `0x198`, 92 compiler relocations.
  The target has 838 runtime relocations: 244 calls and 297 HI/LO pairs.
- Retained candidate: 11144 bytes / 2786 words, 2519 raw and 2511
  relocation-masked differing words, first mismatch `+0x4`, frame `0x198`,
  864 compiler relocations: 244 calls and 310 HI/LO pairs. The generated
  score above is the masked workbench value, not a matched-word count.
- The size deficit fell from 7672 bytes to 12 bytes. This is source and
  structural progress only: zero new matched-byte credit, no atlas or
  symbol-table promotion, and no canonical linked-C or full-ROM proof.
- An ordinary diagnostic link of untouched compiler output at module offset
  `0x334`, using the authenticated stored addends, differs from the owned
  baserom range in 2512 words, beginning at `+0x4`. Its import aliases and
  compiler-owned case-table placement are diagnostic, not canonical proof.

Recovered evidence and source decisions:

- Tier B: all 244 target call records resolve to 63 distinct callee identities.
  The candidate has the same ordered call identities. A static traversal also
  agrees on every next-call adjacency set and all ten dispatch-entry first-call
  sets. This check abstracts branch predicates; it is call-CFG evidence, not
  whole-function semantic equivalence or matching acceptance.
- Tier B/D: real `ControlActor`, `AnimPath`, matrix and graphics types replace
  draft scalar guesses. Save slots contain four eight-byte records with a
  word time, three initial bytes and a character byte. The two coordinate
  tables have four signed-halfword XY records. Settings reads and
  byte-preserving writes use the recovered bitfield positions.
- The reconstruction restores all ten panels, four detail modes, record and
  character loops, model updates, graphics commands, settings and input paths.
  Source was written as ordinary C from Mickey-only evidence. The raw m2c
  draft remains ignored and was not copied into tracked code or comments.
- Draft mistakes corrected before retaining the candidate include reloading
  the selected model and object after the animation callback, rechecking the
  current detail state after navigation/Back callbacks, preserving word-sized
  title/list coordinates, and reading enabled entries as unsigned bytes.
  Initial conversion preserves the target's two narrowed results and full
  third result; model setup passes a floating-point zero.
- The rank lookup uses a byte span that includes the preceding byte, keeping
  the observed predecessor-byte read defined. IDO folds this to a different
  address/addend spelling. It must not be replaced by an out-of-bounds read
  solely to recover the target relocation.
- The physical display-list alias denotes overlay-local data plus its physical
  bias. It is not a resident global. Its canonical alias and runtime relocation
  binding remain unproved; a zero-valued proxy link cannot close this task.

Relocation and remaining-mechanism limits:

- The explicit Mickey runtime-identity adapter resolves every candidate record.
  Its diagnostic multiset intersection is 808 of 838 target records; 862 of
  864 candidate type/identity pairs occur somewhere in the target, and only
  155 identities align at the same offsets. None of these aggregate facts
  means the required offset/type/identity table is exact.
- The standard relocation-surface path still conflicts with the resident-name
  namespace for `D_800D312C`. The ignored adapter records the authenticated
  runtime namespace explicitly; no shared tool or symbol policy was changed.
  Checkpoints conservatively report zero acceptance-proven exact identities.
- Most extra relocation pairs reload the menu-owner global whose address the
  target retains, while the candidate retains the settings address instead.
  The remaining inter-call length differences also include stack homes,
  register choices, branch duplication and the rank address spelling.
- Initial workbench routing was `structure` / `structure-buckets`: recover
  complete control flow and typed identities before local allocation work.
  Final verdict remains `structure-mismatch`. The `constant-audit` footer was
  checked against Mickey's literals and explicit diagnostic bindings; its
  remaining aligned constants include address and stack-placement differences.
  No flag lattice, permuter, instrumented allocator, instruction editing,
  added padding or inert source-shaping mechanism was substituted.

Stopping evidence and retained alternatives:

- Attempt 26 established the best admissible positional score: 2511 masked
  differing words, 2786 words, frame `0x198`. Attempt 28's complete settings
  view ties that compiled result and is the retained source.
- Attempts 27 through 31 are five consecutive non-improving source-form checks:
  separate character index (2703 masked differences, 2769 words); one complete
  settings view (2511, 2786); early readiness returns (2511, 2786); inner detail
  equality chain (2643, 2784); explicit object-scan increment (2511, 2786).
  All have frame `0x198`. These variants established no new identity or causal
  mismatch classification and did not eliminate the remaining global-address
  or stack-context mechanism. The best residual stayed fixed throughout.
- Earlier structural gains remain useful even where a positional score fell:
  both four-coordinate loops now compile with the target region lengths, and
  separating stereo, screen and preview values recovered the target frame.
  The guarded source was banked continuously in `8df7af84`, `e0eadbdf` and
  `d0526960`; this handoff commit preserves the final best reconstruction.
- Every material attempt retains source, configured object, score, first
  mismatch and hypothesis under ignored `build/whale1/attemptNN/`. Baseline,
  draft, typed context, identity analysis and final receipts are under ignored
  `build/whale1/`; generated drafts are also under ignored `m2cfiles/`.
  The retained baseline object's digest was checked against its original
  measurement. Do not overwrite these alternatives during integration.

Validation and next action:

- Commands run include `tools/wb_compare.sh` against the configured full TU,
  `decomp-workbench diagnose`, `guide structure-buckets`, `guide constant-audit`,
  `generate_ctx.sh`, and `mips_to_c.sh`. The full generated context exposed
  unsupported declarations in m2c, so a narrow Mickey-only typed context was
  used for draft analysis. Scratch identity, memory-width, call-CFG and owned
  range diagnostics provide the additional receipts described above.
- `gmake check-nonmatching-builds` passed all 243 candidate-bearing TUs.
  `gmake cleanroom` and `gmake check-docs` passed, including the plateau audit.
  `tools/finalize_plateau.py` guarded and recorded this measured candidate.
- `gmake verify` failed at the same pre-existing overlay 94 canonical-link
  error reproduced before any source edit. `overlay94UpdateController.c`
  has unresolved resident calls and truncated call relocations; the final
  log again names `func_800254FC`, `func_8002A878`, `func_8002565C` and
  `func_8002A910`, plus truncated calls to `func_8005ABA8`, `func_8005AF14`,
  `func_80019AB8` and `func_8002B040`. No full-ROM verification passed.
  Logs remain in ignored `build/whale1/final/`.
- Next concrete lever: a separately authorized causal packet should explain
  why the menu-owner address loses its persistent register while the settings
  address gains one, using this full reconstructed TU and the stack-resident
  enabled-count value as the baseline. This assignment did not authorize that
  allocator experiment. Keep the rank byte-span and physical-list alias
  questions explicit rather than masking their relocation differences.
- Before promotion, resolve canonical overlay proxies and case-table placement,
  require exact owned bytes plus exact relocation count/type/offset/identity,
  extract the linked owned range against the same baserom offsets, and obtain
  a passing `gmake verify`. The existing overlay 94 canonical-link failure
  also needs a separate owner; this lane did not edit that source.

<!-- plateau-handoff:func_overlay_060_F0000334_18BA10C:end -->
