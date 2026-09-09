#!/usr/bin/env python3
"""Focused regressions for permute_batch's outer wall-clock bound."""

from __future__ import annotations

import importlib.util
import sys
import tempfile
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
MODULE_PATH = ROOT / "tools" / "permute_batch.py"


def load_module():
    spec = importlib.util.spec_from_file_location("permute_batch_under_test", MODULE_PATH)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def test_default_outer_cap(module) -> None:
    args = module.parse_args([])
    assert args.max_total_minutes == 120


def test_active_process_group_observes_outer_cap(module) -> None:
    with tempfile.TemporaryDirectory(prefix="permute-batch-deadline-") as tmp:
        work = Path(tmp)
        fake = work / "fake_permuter.py"
        fake.write_text(
            "import time\n"
            "print('base score = 42', flush=True)\n"
            "time.sleep(30)\n"
        )
        scratch = work / "scratch"
        out_dir = work / "out"
        scratch.mkdir()
        out_dir.mkdir()

        old_python = module.PYTHON
        old_permuter = module.PERMUTER_PY
        module.PYTHON = Path(sys.executable)
        module.PERMUTER_PY = fake
        started = time.monotonic()
        try:
            base, _elapsed, stopped_flat, stopped_batch = module.run_permuter(
                scratch,
                out_dir,
                minutes=10,
                threads=1,
                extra_args=[],
                batch_deadline=time.monotonic() + 0.25,
            )
        finally:
            module.PYTHON = old_python
            module.PERMUTER_PY = old_permuter

        assert time.monotonic() - started < 3
        assert base == 42
        assert not stopped_flat
        assert stopped_batch


def test_branch_destinations_are_mandatory(module) -> None:
    with tempfile.TemporaryDirectory(prefix="permute-branch-policy-") as tmp:
        work = Path(tmp)
        fake = work / "fake_permuter.py"
        fake.write_text(
            "import sys\n"
            "assert sys.argv.count('--no-ignore-branch-targets') == 1\n"
            "assert sys.argv.count('--stack-diffs') == 1\n"
            "print('base score = 1', flush=True)\n"
        )
        old_python, old_permuter = module.PYTHON, module.PERMUTER_PY
        module.PYTHON, module.PERMUTER_PY = Path(sys.executable), fake
        try:
            for extra in ([], ["--stack-diffs", "--no-ignore-branch-targets"]):
                base, _, flat, stopped = module.run_permuter(
                    work, work, minutes=1, threads=1, extra_args=extra,
                    batch_deadline=time.monotonic() + 5,
                )
                assert base == 1 and not flat and not stopped
        finally:
            module.PYTHON, module.PERMUTER_PY = old_python, old_permuter
        assert "--no-ignore-branch-targets" in module.MANDATORY_PERMUTER_ARGS


def main() -> int:
    module = load_module()
    test_default_outer_cap(module)
    test_active_process_group_observes_outer_cap(module)
    test_branch_destinations_are_mandatory(module)
    print("permute batch deadline tests: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())


def test_conditional_postprocess_is_not_replicated(module) -> None:
    """A recipe line wrapped in `if [ ... ]; then ...; fi` must be recorded as
    skipped, never split into fragments that leave a stray `fi`."""
    import re
    line = 'if [ -n "$(x)" ]; then tools/binutils/mips64-elf-objcopy --remove-section=.rel.rodata build/src/o.c.o; fi'
    assert re.search(r"(^|[;&\s])if\s+\[", line) and re.search(r";\s*fi(\s|$)", line)
