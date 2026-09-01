<!-- plateau-handoff:overlay89InitializeEffect:start -->
### `overlay89InitializeEffect` plateau handoff

- source: `src/overlays/o089/overlay89InitializeEffect.c`
- score: 147/205 words
- frame: 0x58
- relocations: 5
- first mismatch: +0x40
- summary: Fresh exact-size V0 retains 58 differences; 5/5 relocation sites align, three identities resolve, and create/maintain proxies remain unresolved.

- geometry: Target and configured C are both `0x334`/820 bytes/205 words with frame `0x58`; the owned Overlay 89 range is `+0x270..+0x5A4`, ROM `0x18D44A0..0x18D47D4`, immediately followed by `overlay89UpdateStateAndParticles`.
- relocation proof: Target and candidate each contain five records and all five offsets/types align. The `+0x50/+0x54` HI16/LO16 pair resolves exactly to Overlay 89 `+0x7D4` with addend 4, and the `+0x148` call resolves exactly to local `+0x138`. Candidate identities at `+0x21C` and `+0x288` remain unresolved while the runtime table authenticates target identities `resident:+0x18764` and `overlay:89:+0x0`, respectively.
- diagnosis: Workbench reports structural mismatch with 24 opcode mismatches and four alignment gaps; the target/candidate relocation count and site layout do not explain the remaining source/codegen divergence.
- stale-record result: The retained 820-byte geometry, 58-word deficit, frame, and first mismatch `+0x40` reproduce unchanged under the canonical relocation tooling.
- next action: Preserve the fallback pending unique candidate proof for the create/maintain proxies or a genuinely new source-authentic state-lifetime mechanism. Prior lifetime forms, flags, scaffolding, and permutation remain closed.
<!-- plateau-handoff:overlay89InitializeEffect:end -->
