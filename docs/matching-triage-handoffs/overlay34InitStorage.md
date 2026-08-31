<!-- plateau-handoff:overlay34InitStorage:start -->
### `overlay34InitStorage` plateau handoff

- source: `src/overlays/o034/overlay34InitStorage.c`
- score: 45/50 words
- frame: 0x30
- relocations: 8
- first mismatch: +0x24
- summary: Fresh V0 remains exact-sized at 46/50 relocation-normalized words with a 0x30 frame and four `sp+0x18` versus target `sp+0x1C` sites. A fidelity-gated whole-itable capture identifies abstract home -0x14 as the synthesized old-value carrier for `countdown--`, while the byte-size multiply/conversion web is colored to `a2`. Making that old value explicit moves `size` to -0x14 but leaves the emitted caller-save slot at sp+0x18, proving frontend home and final save-slot assignment are decoupled. Assignment-in-call returns V0; a second call-live byte-count alias regresses to 48 words. Preserve V0 and reopen only with later save-slot/UGEN producer evidence.
<!-- plateau-handoff:overlay34InitStorage:end -->
