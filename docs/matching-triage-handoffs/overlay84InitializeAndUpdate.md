<!-- plateau-handoff:overlay84InitializeAndUpdate:start -->
### `overlay84InitializeAndUpdate` plateau handoff

- source: `src/overlays/o084/overlay84InitializeAndUpdate.c`
- score: 12 differing words
- frame: 0x58
- relocations: 5
- first mismatch: +0x14
- assignment base: `cef9b4e7`
- owned range: overlay 84 `+0x48..+0x314`, 716 bytes / 179 words
- score proof: 12 differing positional words, improved from 71; 167/179 words exact
- frame proof: exact `0x58`
- relocation proof: five call sites align by offset and type, but all five static identities still differ; preflight fails closed on unresolved candidate identities
- mismatch proof: first mismatch is `+0x14`
- attempts: all 119 flag combinations, ten coherent source forms, then one interrupted bounded two-thread permutation batch; the best completed mutation was retained
- retained improvement: split the loop node from the initialized node, add the required two-word local home, declare the loop index first, and scope the metadata slot to its use
- residual: seven stack-home constant words, a four-word initialization scheduling move, and the five unresolved call identities; all integer and floating-point allocation lanes are otherwise exact
- next action: re-open only with a concrete stack-home or relocation-identity hypothesis; do not repeat declaration shuffles or the completed flag lattice
- JFG: `doorRegisterOpener` was the strongest similarity result at only 6.95% and remains assembly-only in the pinned JFG tree, so it is neither a credible donor nor a public-ledger-quality lead
<!-- plateau-handoff:overlay84InitializeAndUpdate:end -->
