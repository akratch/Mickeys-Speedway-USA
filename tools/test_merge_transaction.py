#!/usr/bin/env python3
"""Exercise merge preservation and generated staging in disposable Git repos."""
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent


class MergeTransactionTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="mickey-merge-test-")
        self.root = Path(self.tmp.name)
        self.env = dict(os.environ, GIT_CONFIG_GLOBAL=os.devnull,
                        GIT_CONFIG_NOSYSTEM="1", GIT_AUTHOR_NAME="Fixture",
                        GIT_AUTHOR_EMAIL="fixture@example.invalid",
                        GIT_COMMITTER_NAME="Fixture",
                        GIT_COMMITTER_EMAIL="fixture@example.invalid")
        self.run_command("git", "init", "-q")
        self.write("overlay_undefined_syms.us.txt", "initial symbols\n")
        self.write("owned.c", "initial\n")
        self.write("unrelated.txt", "preserve\n")
        self.run_command("git", "add", ".")
        self.run_command("git", "commit", "-qm", "base")
        self.run_command("git", "checkout", "-qb", "fixture-lane")
        self.write("owned.c", "candidate\n")
        self.run_command("git", "commit", "-qam", "candidate")
        self.run_command("git", "checkout", "-q", "-")
        self.run_command("git", "merge", "--no-commit", "--no-ff", "fixture-lane")

    def tearDown(self):
        self.tmp.cleanup()

    def write(self, path, text):
        target = self.root / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(text)
        return target

    def run_command(self, *args, check=True):
        return subprocess.run(args, cwd=self.root, env=self.env, text=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                              check=check)

    def transaction(self, action, check=True):
        return self.run_command(sys.executable, str(TOOLS / "merge_transaction.py"),
                                action, check=check)

    def test_generated_symbols_staged(self):
        self.transaction("begin")
        self.write("overlay_undefined_syms.us.txt", "regenerated symbols\n")
        self.transaction("stage")
        self.assertEqual(self.run_command("git", "show", ":overlay_undefined_syms.us.txt").stdout,
                         "regenerated symbols\n")
        self.assertEqual(self.run_command("git", "diff", "--name-only").stdout, "")

    def test_unexpected_dirt_preserved_without_staging(self):
        self.transaction("begin")
        before = self.run_command("git", "write-tree").stdout
        self.write("overlay_undefined_syms.us.txt", "generated\n")
        self.write("unrelated.txt", "valuable edit\n")
        result = self.transaction("stage", check=False)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("unrelated.txt", result.stderr)
        self.assertEqual(self.run_command("git", "write-tree").stdout, before)
        self.assertEqual((self.root / "unrelated.txt").read_text(), "valuable edit\n")

    def test_begin_refuses_unstaged_input(self):
        self.write("unrelated.txt", "user edit\n")
        self.assertNotEqual(self.transaction("begin", check=False).returncode, 0)

    def test_changed_index_fails(self):
        self.transaction("begin")
        self.write("owned.c", "unvalidated edit\n")
        self.run_command("git", "add", "owned.c")
        self.assertNotEqual(self.transaction("stage", check=False).returncode, 0)

    def test_deleted_then_regenerated_path_is_staged_even_if_ignored(self):
        # Model the incoming merge deleting this generated path, followed by
        # its ordinary generator recreating the validated output.
        self.run_command("git", "rm", "overlay_undefined_syms.us.txt")
        self.write(".git/info/exclude", "overlay_undefined_syms.us.txt\n")
        self.transaction("begin")
        self.write("overlay_undefined_syms.us.txt", "regenerated after deletion\n")
        self.transaction("stage")
        self.run_command("git", "commit", "-qm", "validated merge")
        self.assertEqual(self.run_command("git", "show", "HEAD:overlay_undefined_syms.us.txt").stdout,
                         "regenerated after deletion\n")
        self.transaction("clean")

    def test_preexisting_untracked_generated_input_is_not_adopted(self):
        self.run_command("git", "rm", "overlay_undefined_syms.us.txt")
        self.write("overlay_undefined_syms.us.txt", "user work\n")
        result = self.transaction("begin", check=False)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("preexisting untracked", result.stderr)
        self.assertEqual((self.root / "overlay_undefined_syms.us.txt").read_text(), "user work\n")

    def test_clean_rejects_untracked_generated_output(self):
        self.run_command("git", "rm", "overlay_undefined_syms.us.txt")
        self.run_command("git", "commit", "-qm", "synthetic deletion")
        self.write("overlay_undefined_syms.us.txt", "uncommitted generated output\n")
        self.assertNotEqual(self.transaction("clean", check=False).returncode, 0)

    def install_fake_gates(self):
        self.write(".venv/bin/python", "#!/bin/sh\nexec " + sys.executable + ' "$@"\n').chmod(0o755)
        shutil.copy(TOOLS / "merge_transaction.py", self.write("tools/merge_transaction.py", ""))
        for name in ("refresh_atlas_digest", "fix_stale_externs", "check_match_regression",
                     "check_duplicate_bodies", "fix_jumptable_claim"):
            self.write(f"tools/{name}.py", "print('fixture gate OK')\n")
        self.write("tools/with_verify_lock.sh", '#!/bin/sh\nexec "$@"\n').chmod(0o755)
        self.write("bin/gmake", """#!/bin/sh
case " $* " in
  *" overlay-syms "*) printf 'generated symbols\\n' > overlay_undefined_syms.us.txt ;;
esac
case " $* " in
  *" check-overlay-syms "*) [ "$FAIL_GATE" != yes ] || exit 1 ;;
esac
printf 'OK fixture\\n'
""").chmod(0o755)
        self.write("bin/timeout", '#!/bin/sh\nshift\nexec "$@"\n').chmod(0o755)
        # Keep finish_merge's informational progress pipeline successful.
        self.write("bin/gmake-progress", "")
        self.env["PATH"] = str(self.root / "bin") + os.pathsep + self.env["PATH"]

    def test_gate_failure_keeps_merge_uncommitted(self):
        self.install_fake_gates()
        self.env["FAIL_GATE"] = "yes"
        head = self.run_command("git", "rev-parse", "HEAD").stdout
        result = self.run_command("bash", str(TOOLS / "finish_merge.sh"), check=False)
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(self.run_command("git", "rev-parse", "HEAD").stdout, head)
        self.run_command("git", "rev-parse", "--verify", "MERGE_HEAD")
        self.assertEqual((self.root / "overlay_undefined_syms.us.txt").read_text(),
                         "generated symbols\n")

    def test_hook_failure_propagates(self):
        self.install_fake_gates()
        self.write(".git/hooks/pre-commit", "#!/bin/sh\nexit 1\n").chmod(0o755)
        head = self.run_command("git", "rev-parse", "HEAD").stdout
        result = self.run_command("bash", str(TOOLS / "finish_merge.sh"), check=False)
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(self.run_command("git", "rev-parse", "HEAD").stdout, head)
        self.assertEqual(self.run_command("git", "show", ":overlay_undefined_syms.us.txt").stdout,
                         "generated symbols\n")


if __name__ == "__main__":
    unittest.main()
