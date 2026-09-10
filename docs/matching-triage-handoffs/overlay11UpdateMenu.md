<!-- plateau-handoff:overlay11UpdateMenu:start -->
### `overlay11UpdateMenu` plateau handoff

- source: `src/overlays/o011/overlay11UpdateMenu.c`
- score: 299/301 words
- frame: 0x48
- relocations: 102
- first mismatch: +0x138
- summary: Load base is $3; its existing noalias fact covers later menuInput, not handle at the spill pair. Indexed menuInput is byte-identical and adds no temps.


Current diagnosis, 2026-09-10, lane `astra-o11`, assignment base
`701478e37ab623dc870605e630cf7b12be2539a9`.
This remeasurement supersedes the earlier universal claims that the fact
requires an induction pointer, that every indexed form adds compiler temps,
and that the residual is unreachable from C. No C unreachability proof is
claimed here.

The retained guarded C scores **299/301 relocation-masked words**: 1,204
executable bytes, zero size delta, two masked differences at `+0x138` and
`+0x140`, 16 raw differences (14 relocation artifacts), frame `0x48`.
Ownership remains Overlay 11 text `+0x1398..+0x184C`, ROM
`0x1869BE0..0x186A094`. No padding or new matching credit is claimed.

### Register and scope measurements

The argument load in the handle loop uses **$3 (v1)** as its address base;
its result goes to $4 (a0). The index is in $2 (v0). The two spill homes are
frame offsets 68 for the index and 36 for the handle. These facts were read
from the actual configured TU's compiler listing, not inferred from a score.

- Earlier accesses to D_1BC: $6 against $sp, later $2 against $sp: Both facts are explicitly closed before the handle loop.
- Handle loop and its two stores: None for $3 against $sp: The pointer is initialized from D_1CC and walked; no directive covers its argument load or spill pair.
- Later menuInput accesses: $3 against $sp: Three scopes, all after the loop, where $3 holds D_menuBase + 0x1C4. Each has a matching close.

The indexed `D_0[D_1C4]` accesses emit symbolic memory operands, with no
explicit `.noalias` directive of their own in this listing. Their presence
therefore does not establish a fact for the handle pointer.

The proposed `menuInput = (s32 *)&D_menuBase[0x1C4]` rewrite has **identical
full text and identical relocation tuples** to the original decay-plus-offset
form. Its five `.noalias` and five `.alias` directives have the same registers
and scope locations. The original form already emits the menuInput fact in
this TU. Thus hypothesis (a) is true only in the sense that the fact already
exists elsewhere in the body. Hypothesis (b), a different physical register,
is false for menuInput: it also uses $3. The blocker is **a different value
lifetime and a later directive position**, not the numerical register name.
Facts for a later materialized address do not cover the earlier handle load.

### Faithful replay and controlled attempts

Recovered the recipe with `nm_ranking.configured_compile_commands`, retained
its game-code flags and NON_MATCHING define, and resolved every include path
against the original compile directory. The unchanged TU was copied into
ignored scratch before running `cc -S`; no listing was written into `src/`.
A direct scratch compile and the configured asm-processor compile have
identical full text and relocation tuples. Replaying the listing through
`as0` and `as1`, using the compiler-path flags and excluding the assembly-input
`-pic0`/`-noglobal` additions, reproduces both exactly. The replay baseline
reproduces the configured 16 raw / 2 masked score before any perturbation.

- 1, assembly diagnostic: Open the $3/$sp fact at the load and keep it through both stores: permit the crossing.: 0 masked differences, 14 raw relocation artifacts, zero size delta, 102 relocation records.
- 2, assembly diagnostic: Name $2 instead: leave the load base unproved.: 2 masked differences, first +0x138, zero size delta.
- 3, assembly diagnostic: Open the $3 fact but close it before the second store: remove permission at the decision point.: 2 masked differences, first +0x138, zero size delta.
- 4, C remeasurement: Index the later menuInput address: leave the handle lifetime's fact unchanged.: Byte- and relocation-identical to baseline; 2 masked differences, frame 0x48.

The assembly-only zero is a diagnostic, never a C match or a promotion.
No instruction was edited in a compiled object. The remaining raw relocation
fields were not treated as exact linked bytes.

Both original and indexed menuInput forms were also compiled with `cc -g3`.
Parsing the ECOFF `.mdebug` HDRR/FDR/SYMR records gives the same eleven named
local entries and offsets, with no compiler temporary local entries. Both
have frame `0x48`. The debug objects contain 1,208 function bytes, versus
1,204 in the configured build, so their table is secondary allocation
evidence; the debug code is not used as matching evidence. The zero-cost
menuInput rewrite disproves a blanket indexed-syntax temp penalty. It does
not supply a zero-cost fact for the handle value.

Stopping evidence: the assignment's ordered diagnostic question is resolved.
Three targeted phase controls and one C remeasurement identify the missing
input precisely. Only one C form was tested here, and it left that input
unchanged. This is not a spelling sweep or an exhaustion argument. Earlier
indexed-handle regressions are not promoted into a universal lower bound.
The precise remaining C lever is a source-authentic form that emits the
$3/$sp fact **for handle at the spill pair**, at the existing 0x48 frame and
with no compiler temp cells. A named-array fact for another value or a later
scope does not meet that requirement.

The assignment gate reports stale reopen pins on this base. This session
followed the owner's explicit diagnostic assignment; it did not change
`config/lane-reopen-authorizations.us.json` or assign additional targets.
The C body, guard, fallback, symbols and atlas ownership remain unchanged.

Evidence is preserved in ignored `build/wb/o11-alias/`: baseline C and objects,
configured command and flags, listing and phase commands, `fidelity.json`,
`controls.json`, per-attempt sources/listings/objects/scores/hypotheses,
`.mdebug` tables, and validation logs. Compiler listings, objects and raw
evidence are not tracked. Reproduction commands are the retained `setup.py`,
`measure.py`, and `controls.py`; the source score uses
`python3 tools/score_symbol.py overlay11UpdateMenu`.

Fresh `function_preflight.py --json` authenticates all **102/102** relocation
offsets, types, stable identities and effective identities, with zero
unresolved candidate identities. The canonical linked assembly fallback's
owned 1,204-byte range equals the same baserom range byte for byte.
`gmake verify` passes with SHA1
`507341c0a40ca3e9a7cee969b396ee53facfb548`; `gmake cleanroom` and
`gmake check-docs` pass. The fresh lane needed `gmake overlay-syms` after the
expected resident-link failure. `tools/finalize_plateau.py` refreshed the
source marker and this shard and passed its gates. A final configured source
score still reports 2 masked differences at the original extent.

Files changed for this handoff: only this shard and the source's diagnostic
comment/plateau marker. The candidate body is unchanged. The separate generic
compiler-learning correction records the scope lesson without target-specific
addresses or attempt scores. Next action is the zero-temp handle-lifetime
producer described above, if new causal evidence establishes one; this lane
hands off a diagnosis, not a C match.

<!-- plateau-handoff:overlay11UpdateMenu:end -->
