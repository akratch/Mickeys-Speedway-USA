# asm-differ in Mickey

Use this only after the human authorizes compilation/diffing in the current
task and confirms a dedicated test desktop is available.

Mickey's entry point is:

```sh
./diff.sh -m <symbol>
```

`diff_settings.py` selects the baserom, built ROM, map, source directories, and
the vendored `tools/binutils/mips64-elf-objdump`. Its rebuild command is capped
at two low-priority jobs. Do not invoke asm-differ directly with generic paths.
Do not use `-w` watch mode on the occupied workstation.

Read the first divergent row before changing code. A consistent register
recoloring usually means local declaration order or lifetime; changed offsets
usually mean a wrong type or field; changed branch layout usually means the C
control-flow shape is wrong. Fix one evidenced difference at a time.

No displayed differences is candidate-level evidence. The human decides when
to authorize final `gmake verify` and the clean-room/documentation checks.
