# Local decomp model as a bounded candidate specialist

**Historical.** The `n64dllm-v1` lane described here was evaluated and
retired on 2026-08-23: 5 targets, 18 samples, 0 improvements over the manual
matching basin, every lane graded "Harmful." See
`docs/adr/0009-model-routing-and-agent-operation.md` for the decision and
`docs/acceleration-survey.md` §8 for the measurement. This document is kept
as a record of the contract that was tried, not as an active operating
procedure; do not route matching work to a local model.

This is Mickey's operating contract for `n64dllm-v1` and the paired private
`akratch/decomp-agent` checkout. Record the exact harness commit in every run
ledger. Mickey's build, diff, clean-room, provenance, and canonical-integration
rules remain authoritative.

`n64dllm-v1` is an untrusted C-candidate generator, not a repository navigator,
campaign planner, coordinator, or autonomous shell agent. The campaign
coordinator approves one bounded job. The solver owns requests, scratch paths,
compilation, comparison, budgets, and the attempt ledger. The model owns no
tools and returns one C function per request.

The production lane is the hardened `decomp-agent solve` controller with
coordinator-prepared Mickey job arguments. `decomp-workbench` provides
object-stage comparison; Mickey's configured-object, relocation, linked-output,
ROM, provenance, and canonical gates remain external. Codex, Qwen Code, and
OpenCode are diagnostic harnesses only.

## One-time setup

The GGUF is 18,556,687,968 bytes (about 18.6 GB / 17.3 GiB). The server, KV
cache, and operating system also need headroom. A human chooses when to download
or start the model; agents must not do either implicitly on an occupied
workstation.

1. Install the server and Hugging Face CLI if needed:

   ```sh
   brew install llama.cpp hf
   ```

2. Download the exact upstream GGUF:

   ```sh
   hf download jlebthedude/n64dllm-v1 \
     n64dllm-v1-Q4_K_M.gguf --local-dir "$HOME/models"
   ```

   Hugging Face reports SHA-256
   `eecaaef1e94d2a6d56f44be336b661ae6fef73db67f081da8a409f19b18c69c3`.
   A human may verify it with `shasum -a 256` when the full disk read is wanted;
   agents must not do so automatically.

3. The optional Codex diagnostic profile requires Codex CLI 0.149.0 or newer:

   ```sh
   mkdir -p "$HOME/.codex"
   cp .codex/decomp.config.toml.example "$HOME/.codex/decomp.config.toml"
   cp .codex/n64dllm-model-catalog.json \
     "$HOME/.codex/n64dllm-model-catalog.json"
   ```

   This profile is opt-in and does not affect normal Codex sessions. It proves
   custom-provider wire compatibility only; it is not the supported campaign
   execution path.

## Start the server

Start `llama-server` explicitly in a visible dedicated terminal:

```sh
llama-server \
  -m "$HOME/models/n64dllm-v1-Q4_K_M.gguf" \
  --alias n64dllm-v1 \
  --host 127.0.0.1 \
  --port 8081 \
  -ngl 99 \
  -c 49152 \
  -ctk q8_0 \
  -ctv q8_0 \
  -np 1 \
  --jinja \
  -fa on
```

Keep the terminal visible and stop the server with Ctrl-C when finished. It
reports open CORS and no API key, which is acceptable only on `127.0.0.1`.
Never expose the port to the LAN or internet without an authenticated boundary.

The 49,152-token server context is a compatibility ceiling, not a request
target. Use the pinned solver limits: at most 16,000 target-assembly characters,
1,536 generated tokens, and 32,768 candidate bytes unless a reviewed job records
different limits. The target-assembly limit approximates the model's 4,000-token
training boundary. Reject oversize evidence; never rely on transcript compaction.

## Bounded solver contract

The coordinator-authorized solver job must enforce all of these conditions:

- Start a fresh Chat Completions request for each independent sample and
  advertise no tools.
- Supply one exact function identity, compiler version, real prototype, and
  bounded target assembly. The model returns one complete definition of that
  function and no other text.
- Use and record the pinned solver sampling ladder, `top_p=0.8`, `top_k=20`,
  `repetition_penalty=1.05`, and per-attempt seed. Sampling may vary; solver
  control flow and acceptance do not.
- An optional retry request contains only the immutable base prompt, current
  best C, and compact structured verifier feedback. Do not carry an interactive
  transcript.
- Never expose filesystem, shell, compiler, comparison, repository, git,
  network, goal, todo, skill, subagent, or process-control tools to the model.
- Stop on exact object-function bytes, the reviewed attempt or time budget, or
  an external dependency. Record and skip duplicate candidates; the model may
  not expand the job.

A tool-like response, path request, explanation, or wrong-function definition
is a protocol failure. A duplicated candidate is a recorded non-improving
result. In either case, the solver decides whether the bounded job may continue.

## Model request shapes

Fresh samples use this shape:

```text
You are an N64 matching-decompilation model.
Compiler: <exact IDO version>
Declared prototype: <real project declaration>
Target assembly: <bounded target function disassembly>

Return exactly one complete definition of <symbol> and no other text.
```

An optional retry adds the current best function and compact structured
verifier feedback to the same immutable base. Do not ask the model to orient,
discover the toolchain, design a campaign, inspect the repository, emit several
variants, or decide whether a result is safe to integrate.

## Host validation and ledger

Treat every response as an untrusted hypothesis. Before compilation,
mechanically reject malformed protocol output, preprocessor directives, inline
assembly, the wrong function definition, or clear violations of fixed source
facts. Frame, owned size and bytes, instruction shape, and relocations are
compiled outcomes. The coordinator reviews semantics and invariants before any
promotion.

Give every run a new or empty ignored `--workdir` under `build/wb/`, and pass an
explicit `--out` beneath the target's owned workbench. Do not use `--force` to
replace another run's result. The coordinator must begin `--compile-cmd` with
`tools/with_compile_token.sh`; the wrapper syntax is
`tools/with_compile_token.sh command [args ...]`. Never execute the generated
object, ROM, or another binary.

The solver ledger retains each request, sampling settings, and parsed response
content and metadata, plus extracted C, the full compilation unit, compiler
output, object, comparison, first mismatch, and hashes when those artifacts are
produced. The coordinator records the Mickey-specific evidence that remains:

- exact configured compiler route and flags;
- owned-text size and authoritative score;
- relocation count, types, offsets, and identities;
- configured-object, linked-range, and ROM disposition;
- semantic review and project-gate disposition.

The model's prose, claimed score, or claimed safety is not evidence. Exact owned
bytes and relocations, configured-object validation, linked comparison, and the
normal canonical gates remain required. Promotion and integration belong to the
primary coordinator.

## Codex profile is diagnostic only

Current Codex supports this custom provider through the Responses API. The
checked-in profile disables apps, plugins, browsing, image generation, goals,
and multi-agent features and supplies the server's full context metadata.
Codex's generic prompt and shell-oriented tool surface are still too large for
the supported bounded lane.

If protocol diagnosis is necessary, start read-only:

```sh
codex --profile decomp --strict-config --sandbox read-only
```

Use only an inline, no-tool probe. Do not use `--yolo`, grant workspace writes,
or execute a decomp campaign through this profile. Exit after the diagnostic.

## Troubleshooting

- Confirm `llama-server` is visible and listening only on `127.0.0.1:8081`.
- Confirm the server was started with `--alias n64dllm-v1`.
- The server does not support `previous_response_id`; a controller must send the
  complete bounded history on each request.
- If target assembly cannot fit the pinned 16,000-character limit, reduce or
  split the ownership unit deterministically or use a stronger model. Do not
  increase conversation length or use compaction.
- The model was not trained on assembly inputs longer than roughly 4,000 tokens.
  Split a large target before inference.
- Stop the server if it causes memory pressure. Do not silently change model,
  quantization, sampling, or compiler settings; record deliberate changes as a
  separate evaluation lane.
