<!-- plateau-handoff:levelInit:start -->
### `levelInit` plateau handoff

- source: `src/main/level.c`
- score: 113 differing words
- frame: 0x80
- relocations: 110
- first mismatch: +0x238
- summary: The residual is now register-only. One missing word-sized live scalar carrier was the entire head: the target frame is 0x80 where the candidate stood at 0x78, and giving the four counted loops that were reusing lvlStart their own index closes the frame, both incoming argument homes, the displaced local slot and every word through +0x234, taking 122 to 113. The carrier has to be word-sized -- a halfword local adds none -- and exactly one is wanted, since a second overshoots to 117. What remains is one named colouring decision and its ring consequences: a forced-colour receipt shows uopt web 105, the halfword resourceId of the resource loop, taking c1 at zero cost where the target holds it in c5, and forcing that one colour takes 113 to 99. Five spellings of resourceId -- widened type, cast, test form, pointer cursor, function scope -- leave that colour untouched, so the lever is an extra interfering web on c1 across the loop rather than a spelling of the loop.
<!-- plateau-handoff:levelInit:end -->
