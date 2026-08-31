<!-- plateau-handoff:overlay34InitStorage:start -->
### `overlay34InitStorage` plateau handoff

- source: `src/overlays/o034/overlay34InitStorage.c`
- score: 45/50 words
- frame: 0x30
- relocations: 8
- first mismatch: +0x24
- summary: Fresh V0 remains exact-sized at 46/50 relocation-normalized words with a 0x30 frame and four `sp+0x18` versus target `sp+0x1C` sites. All 119 flags and the retained stack-home trace remain nonexact. A later exact lexical-block precedent justified two bounded natural forms: phase-local scopes retained 50 words but regressed to 41/50 with a 0x40 frame, while branch-local countdowns emitted 49 words with a 0x38 frame and structural/register drift. No isolated legal scope lever remains; preserve V0 and resume only with new producer-emitted stack-home evidence.
<!-- plateau-handoff:overlay34InitStorage:end -->
