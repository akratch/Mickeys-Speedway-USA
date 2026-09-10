<!-- plateau-handoff:overlay57UpdateSelection:start -->
### `overlay57UpdateSelection` plateau handoff

- source: `src/overlays/o057/overlay57UpdateSelection.c`
- score: 162/283 words
- frame: 0x48
- relocations: 89
- first mismatch: +0x4C
- summary: Exact-size at 283 words; the target reads *primaryState once at +0x4C and reuses it for all three comparisons while the volatile pointer reloads, but every non-volatile and cached-read shape tried is 60 to 110 words worse, so the volatiles are load-bearing and the residual is the surrounding branch shape.
<!-- plateau-handoff:overlay57UpdateSelection:end -->
