# m2c in Mickey

Use m2c to recover control-flow shape, not as source authority or proof of a
match. Mickey provides wrappers with its exact target and paths:

```sh
./generate_ctx.sh
./mips_to_c.sh <symbol>
```

`generate_ctx.sh` writes ignored `ctx.c` from project headers.
`mips_to_c.sh` finds the exact ignored disassembly, invokes the bundled m2c with
`--target mips-ido-c`, and writes an ignored draft under `m2cfiles/`.

Retain useful control flow and call order, but replace guessed types, offsets,
names, and signatures with declarations evidenced by Mickey's own headers and
source. Never paste target instructions or m2c scratch output into tracked
documentation. Do not import a struct layout from another game.
