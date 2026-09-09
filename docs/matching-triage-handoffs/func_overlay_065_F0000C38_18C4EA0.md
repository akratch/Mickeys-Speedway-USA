<!-- plateau-handoff:func_overlay_065_F0000C38_18C4EA0:start -->
### `func_overlay_065_F0000C38_18C4EA0` plateau handoff

- source: `src/overlays/o065/func_overlay_065_F0000C38_18C4EA0.c`
- score: 824/887 words
- frame: 0x80
- relocations: 24
- first mismatch: +0x30
- summary: Recovered types/CFG and frame; configured MIPS-I cannot emit target MIPS-II operations. Next: target-local ISA parity review and initializer identity repair.

### Structural reconstruction, 2026-09-08

No new exact functions or bytes. The retained candidate is `NON_MATCHING`.
Ownership is Overlay 65 text `+0xC38..+0x1A14`, ROM
`0x18C4EA0..0x18C5C7C`: 3,548 bytes / 887 words, excluding the adjacent
initializer and all padding. The configured candidate is 3,440 bytes / 860
words, a 108-byte deficit. Raw and relocation-masked comparisons both report
824 differing words; first mismatch is `+0x30`. The 128-byte frame and the
five observed saved-float homes plus both cursor homes now agree with target.

The lane reproduced the assigned baseline before editing: 865 candidate
words, 824 differing words, frame 120 versus 128 bytes, first `+0x0`.
That baseline's lower size deficit did not establish semantic equivalence.
The reconstructed candidate corrects two discrepancies: a trail active at
entry must still render after this update deactivates it, and the final
random call must precede the camera Y/Z loads and spawn-coordinate arithmetic.

Tier B identity evidence comes from Mickey's own runtime relocation tables:
all nine external call sites distinguish the camera getter, camera setter,
view setup, three random-range calls, sine, cosine, and render-state setup.
The adjacent spawn function is a same-overlay JUMP. LOCAL records separately
identify the BSS record pool/timer/drift/vertex cursor and the data gravity
and polygon command address. The raw extracted call carrier is not a shared
callee. The existing resident view implementation establishes `Gfx **` and
`Mtx **` cursor types. No external body or decompiler output was adopted.

There are 24 candidate static text relocations and 24 owned runtime records.
The generic extracted target object has only 22 static text relocations:
its zero-base pool address pair is represented by runtime LOCAL records.
Equal counts are not identity proof. `reloc_surface.py compare` still refuses
with `overlay65Initialize linked symbol conflicts with canonical overlay
ownership`; authenticated summary generation encounters the same conflict
once the canonical ELF exists. Raw workbench comparisons remain diagnostic.
No authenticated stable-identity alignment or linked C result is claimed.

1. Cursor types: 865 candidate words, 824 differing words, 120-byte frame.
2. Final-frame drawing: 859 candidate words, 834 differing words, 120-byte frame.
3. Spawn evaluation: 864 candidate words, 835 differing words, 120-byte frame.
4. Standard packet macros: 904 candidate words, 895 differing words, 168-byte frame.
5. Packet word view: 860 candidate words, 829 differing words, 128-byte frame.
6. Observed local homes: 860 candidate words, 824 differing words, 128-byte frame.

Attempt 4 also increased static relocations to 32; the retained forms have
24. Its standard union-backed packet spelling is rejected for this candidate;
aliasing is a heuristic explanation for the larger memory/register surface. Attempts 2 and 3 establish behavior despite
positional regressions. Attempt 6 is the best admissible reconstructed source;
the original baseline is preserved for measurement, not for adoption.
The checkpoint's numeric best may retain that baseline on a score tie because
its ordering cannot disqualify an extra source guard. Use the final source and
attempt 6 artifacts, not that baseline. Initial static symbol-name coincidences
are not authenticated runtime identities; current identity credit is zero.

The workbench verdict remains `structure-mismatch`. The required
`structure-buckets` levers were consulted, especially flag/context parity and
the spill-slot census. Recovering two packet locals established the missing
frame size; the identified draft's live-value homes then fixed declaration
placement. This was not an arbitrary declaration permutation.

Stopping evidence is an early mechanism boundary, not an attempt-count cap
or a claim of five stalled attempts. The committed target-specific rule in
`mk/overlays.mk` selects MIPS-I. The target contains 54 direct float-to-word
truncations and two branch-likely operations; the configured candidate emits
no such operations and uses FPU-control conversion sequences instead. Ordinary
C reconstruction under this rule cannot establish instruction identity. The
prior source already records MIPS-II experiments, but changing compiler mode
or repairing shared ownership is outside this structural-only packet. No
flag sweep, permuter run, source assembly, instruction editing, or shared
policy change was used. All six source attempts resolved a hypothesis or
recovered behavior/layout; no artificial stall sequence was spent.

Next concrete action: review target-local ISA parity on this corrected source
and resolve the initializer's canonical identity conflict before reopening
matching. Re-derive the configured full-TU baseline and complete relocation
surface after those changes; then classify any remaining source deficit.
Do not repeat the old flag lattice on the old, behaviorally incorrect body.

Validation: configured full-TU C builds; baseline and final actual IDO
preprocessor outputs accepted by the context comparator against themselves;
workbench comparison and diagnosis; runtime-table identity audit; near-match
skeleton scan (best similarity 0.042, no usable donor); passing `gmake verify`;
and passing `gmake check-nonmatching-builds` across 243 candidate TUs. The
canonical GLOBAL_ASM-owned range independently compares equal to the baserom.
This verifies preservation of the fallback, not promotion of the C candidate.

Ignored lane evidence is retained under `build/t2-9/`: baseline and every
material source/object attempt, raw comparison JSON, mismatch diagnoses,
runtime relocation records, compiler/preprocessor evidence, and gate logs.
`final.c`, `final.o`, and `final-raw.json` identify the retained codegen result.
The initial global m2c context failed on an unrelated incomplete array; the
successful target-local draft and identity-disambiguated input remain in
ignored `m2cfiles/`. No draft, instruction listing, or workbench ledger is tracked.

The stronger `gmake overlay-donors-scan-check` was attempted but blocked by
the out-of-tree JFG checkout: observed commit
`efd5abb1c79636e297b831f7c2d5bf47eac39c0c`, expected pinned commit
`c82affffe8f11cb5b440cfa918f4582ad8573279`. No reference checkout or donor pin
was changed. This failure is separate from the successful Mickey-only source,
ROM and clean-room checks.

Banked steps: `3eb772e7` documents runtime identities; `a4c7bfdb` recovers
the cursor types. The final structural/plateau commit contains only the owned
source and this handoff. `gmake overlay-donors` ledger integrity and
`gmake check-scoreboard` also pass.
<!-- plateau-handoff:func_overlay_065_F0000C38_18C4EA0:end -->
