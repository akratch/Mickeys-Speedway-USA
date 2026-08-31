<!-- plateau-handoff:fxScreenEffect:start -->
### `fxScreenEffect` plateau handoff

- source: `src/main/fx.c`
- score: 123 differing words
- frame: 0x30
- relocations: 10
- first mismatch: +0x8
- summary: Seven-word geometry and parameter lifetime/register allocation remain; next try a source-authentic stack-argument preload form.
- relocation identity: Candidate and target each have 10 static relocations; 7 offsets, types, and identities align in fallback-static evidence.
- flag lattice: All 119 combinations were nonexact. The `-O2 -g3 -mips2` diagnostic was 141 words with 131 differing words and first mismatch `+0x4`; its remaining ABI/structure mismatch does not justify a translation-unit flag change.
- donor result: Mickey-only `m2c` reproduced the stale candidate; JFG provides assembly-only structural context, and the similarity scan found no credible exact C donor.
<!-- plateau-handoff:fxScreenEffect:end -->
