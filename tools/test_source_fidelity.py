#!/usr/bin/env python3
"""Original-coordinate preparation adapter regressions (no search)."""
import sys
import unittest
import json
import tempfile
import hashlib
import subprocess
from types import SimpleNamespace
import contextlib
from unittest.mock import patch
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import permute_batch as pb
sys.path.insert(0, str(pb.PERMUTER_DIR))
from src import ast_util
from perm_pycparser import c_ast


class SourceGroups(unittest.TestCase):
    def plan_inputs(self, directory):
        return {"context": {"source_group_plan": hashlib.sha256(
            (directory / "source-groups.json").read_bytes()).hexdigest()}}
    def convert(self, source, symbol="f"):
        ast = ast_util.parse_c(source, from_import=True)
        plan = pb.preserve_source_groups(ast, source, symbol, c_ast)
        raw = ast_util.to_c_raw(ast)
        # Same two passes as prune_source and Candidate.get_source.
        return plan, raw, ast_util.to_c(ast_util.parse_c(raw))

    def test_simple(self):
        plan, raw, result = self.convert("void f(void) {\n int x;\n x=1; x++;\n}\n")
        self.assertEqual(len(plan["groups"]), 1)
        self.assertIn("#pragma _permuter sameline start", raw)
        self.assertIn("x = 1; x++;", result)
        self.assertNotIn("_permuter", result)

    def test_do_cast(self):
        plan, _, result = self.convert("void f(void) {\n int x;\n x=(int)0; do {\n x++;\n } while(x<2);\n}\n")
        self.assertEqual(plan["groups"][0]["control"], "DoWhile")
        self.assertIn("x = (int) 0; do {\n", result)
        self.assertNotIn("do { x++", result)

    def test_if(self):
        _, _, result = self.convert("void f(void) {\n int x;\n x=0; if(x) {\n x++;\n }\n}\n")
        self.assertIn("x = 0; if (x) {\n", result)

    def test_nested(self):
        plan, _, result = self.convert("void f(void) {\n int x;\n if(x) {\n x=0; do {\n x++; x--;\n } while(x);\n }\n}\n")
        self.assertEqual(len(plan["groups"]), 2)
        self.assertIn("x = 0; do {\n", result)
        self.assertIn("x++; x--;", result)

    def test_whole_line_standalone_compound_preserves_declarations_and_scope(self):
        source = "void f(int *p) {\n { int *q = p++; *q = 1; *q += 2; };\n p++;\n}\n"
        plan, raw, result = self.convert(source)
        self.assertEqual(plan["groups"], [{"line": 2, "statements": 2, "control": "Compound"}])
        self.assertIn("{ int *q = p++; *q = 1; *q += 2; } ;\n", result)
        self.assertIn("\n  p++;", result)
        # Markers only: deleting them restores the original declaration scope,
        # statement ordering and every expression in the parser's AST.
        ast = ast_util.parse_c(raw)
        ast.ext[0].body.block_items = [n for n in ast.ext[0].body.block_items
                                     if not isinstance(n, c_ast.Pragma)]
        self.assertEqual(ast_util.to_c_raw(ast),
                         ast_util.to_c_raw(ast_util.parse_c(source, from_import=True)))

    def test_whole_line_compound_inside_multiline_control(self):
        source = "void f(int *p) {\n if(p) {\n { int x = (int)*p; *p = x; }\n p++;\n }\n}\n"
        _, _, result = self.convert(source)
        self.assertIn("{ int x = (int) (*p); *p = x; }\n", result)
        self.assertNotIn("if (p) { {", result)

    def test_partial_or_ambiguous_compound_retains_original_for_measurement(self):
        for body in ("{ int x;\n x=1;\n }", "{ int x; } { int y; }",
                     "{ if(p) *p=1; }", "if(p) { *p=1; }"):
            source = "void f(int *p) {\n " + body + "\n}\n"
            original = ast_util.parse_c(source, from_import=True)
            before = ast_util.to_c_raw(original)
            result, plan = pb.prepare_source_groups(original, source, "f", c_ast)
            with self.subTest(body=body):
                self.assertEqual(plan["status"], "measurement-required")
                self.assertIs(result, original)
                self.assertEqual(ast_util.to_c_raw(result), before)

    def test_compound_literal_braces_do_not_supply_a_fake_boundary(self):
        source = 'void f(void) {\n { char *p = "}"; foo(p); }\n}\n'
        _, _, result = self.convert(source)
        self.assertIn('{ char *p = "}"; foo(p); }\n', result)

    def test_only_selected_function(self):
        plan, raw, _ = self.convert("void g(void) { int y; y=1; y++; }\nvoid f(void) {\n int x;\n x=1;\n x++;\n}\n")
        self.assertEqual(plan["groups"], [])
        self.assertNotIn("sameline", raw)

    def test_line_directives(self):
        plan, _, result = self.convert('#line 90 "original.c"\nvoid f(void) {\n int x;\n x=1; x++;\n}\n')
        self.assertEqual(plan["groups"][0]["line"], 92)
        self.assertIn("x = 1; x++;", result)

    def test_duplicate_line_identity(self):
        with self.assertRaisesRegex(ValueError, "ambiguous"):
            self.convert('#line 4 "a.c"\nvoid f(void) {\n int x;\n#line 5 "a.c"\n x=1; x++;\n}\n')

    def test_unsupported_groups(self):
        for body in ("x=0; do x++; while(x);", "x=0; if(x) x++;",
                     "x=0; while(x) {\n x++;\n }", "x=0; do { x++; } while(x);"):
            with self.subTest(body=body), self.assertRaisesRegex(ValueError, "unsupported"):
                self.convert("void f(void) {\n int x;\n" + body + "\n}\n")

    def test_multiline_statement_not_silently_joined(self):
        with self.assertRaises(ValueError):
            self.convert("void f(void) {\n int x;\n x=\n 1; x++;\n}\n")

    def test_preexisting_pragma(self):
        with self.assertRaisesRegex(ValueError, "preexisting"):
            self.convert("void f(void) {\n#pragma _permuter sameline start\n return;\n}\n")

    def test_missing_function(self):
        with self.assertRaisesRegex(ValueError, "one selected"):
            self.convert("void g(void) {}")

    def test_non_sibling_group_detection(self):
        for source in ("void f(void) { return; }", "void f(int x) {\n if(x) x++;\n}",
                       "void f(int x) {\n if(x) { x++; }\n}",
                       "void f(int x) {\n switch(x) {\n case 1: x++;\n }\n}"):
            with self.subTest(source=source), self.assertRaisesRegex(ValueError, "unsupported"):
                self.convert(source)

    def test_unsupported_original_ast_is_measured_not_rewritten(self):
        source = "void f(void) { return; }"
        ast = ast_util.parse_c(source, from_import=True)
        result, plan = pb.prepare_source_groups(ast, source, "f", c_ast)
        self.assertIs(result, ast)
        self.assertEqual(plan["status"], "measurement-required")

    def test_nested_trial_mutation_never_leaks_on_outer_refusal(self):
        source = "void f(int x) {\n if(x) {\n x=1; x++;\n }\n x=0; while(x) {\n x++;\n }\n}\n"
        ast = ast_util.parse_c(source, from_import=True)
        before = ast_util.to_c_raw(ast)
        result, plan = pb.prepare_source_groups(ast, source, "f", c_ast)
        self.assertEqual(plan["status"], "measurement-required")
        self.assertIs(result, ast)
        self.assertEqual(ast_util.to_c_raw(result), before)

    def test_closing_control_boundary_requires_measurement(self):
        source = "void f(int x) {\n if(x) {\n x=1; x++;\n } x++;\n}\n"
        ast = ast_util.parse_c(source, from_import=True)
        result, plan = pb.prepare_source_groups(ast, source, "f", c_ast)
        self.assertIs(result, ast)
        self.assertEqual(plan["status"], "measurement-required")

    def test_multiline_lexical_endpoints_require_measurement(self):
        for body in ("foo(\n ); x++;", "x=(1\n ); x++;", "foo(\n ); /* ; */ x++;"):
            source = "void f(void) {\n int x;\n" + body + "\n}\n"
            ast = ast_util.parse_c(source.replace("/* ; */", "       "), from_import=True)
            result, plan = pb.prepare_source_groups(ast, source, "f", c_ast)
            self.assertIs(result, ast)
            self.assertEqual(plan["status"], "measurement-required")

    def test_literal_semicolon_is_not_a_statement_boundary(self):
        source = 'void f(void) {\n foo(";");\n return;\n}\n'
        ast = ast_util.parse_c(source, from_import=True)
        _, plan = pb.prepare_source_groups(ast, source, "f", c_ast)
        self.assertEqual(plan["status"], "ungrouped")

    def test_plan_status_and_digest_fail_closed(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            item = pb.QueueItem("f", root / "src/f.c")
            plan = {"contract": pb.SOURCE_GROUP_CONTRACT, "symbol": "f", "groups": []}
            for status in (None, "unknown", "preserved", "measurement-required"):
                plan["status"] = status
                (root / "source-groups.json").write_text(json.dumps(plan))
                with self.subTest(status=status), self.assertRaisesRegex(RuntimeError, "status"):
                    pb.grouped_baseline_fidelity(item, root, root, self.plan_inputs(root), None)
            plan["status"] = "ungrouped"
            (root / "source-groups.json").write_text(json.dumps(plan))
            old = self.plan_inputs(root)
            (root / "source-groups.json").write_text(json.dumps({**plan, "reason": "replaced"}))
            with self.assertRaisesRegex(RuntimeError, "changed"):
                pb.grouped_baseline_fidelity(item, root, root, old, None)

    def test_importer_recipe_normalizes_only_generated_lane_plumbing(self):
        first = b'#!/bin/sh\ncd /lane-a\ntools/ido/cc -O2 -DNAME="/lane-a" "$INPUT" -o "$OUTPUT"\n'
        second = first.replace(b"cd /lane-a", b"cd /lane-b")
        with patch.object(pb, "ROOT", Path("/lane-a")):
            expected = pb.compile_script_digest(first)
        with patch.object(pb, "ROOT", Path("/lane-b")):
            self.assertEqual(expected, pb.compile_script_digest(second))
            self.assertNotEqual(expected, pb.compile_script_digest(second.replace(b"-O2", b"-O1")))
            self.assertNotEqual(expected, pb.compile_script_digest(second.replace(b'DNAME="/lane-a"', b'DNAME="/lane-b"')))

    def test_fidelity_missing_plan_refuses_before_build(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            item = pb.QueueItem("f", root / "src/f.c", overlay=1)
            with patch.object(pb, "ROOT", root), patch.object(pb, "bounded_capture") as run:
                with self.assertRaisesRegex(RuntimeError, "missing"):
                    pb.grouped_baseline_fidelity(item, root, root, {}, None)
                run.assert_not_called()

    def test_ungrouped_does_not_build(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            item = pb.QueueItem("f", root / "src/f.c")
            (root / "source-groups.json").write_text(json.dumps({
                "contract": pb.SOURCE_GROUP_CONTRACT, "symbol": "f", "groups": [], "status": "ungrouped"}))
            with patch.object(pb, "ROOT", root), patch.object(pb, "bounded_capture") as run:
                pb.grouped_baseline_fidelity(item, root, root, self.plan_inputs(root), None)
                run.assert_not_called()

    def test_symlinked_object_parent_refuses_before_build(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            out = root / "run"
            out.mkdir()
            (root / "outside").mkdir()
            (root / "build_non_matching").symlink_to(root / "outside", target_is_directory=True)
            item = pb.QueueItem("f", root / "src/f.c")
            (out / "source-groups.json").write_text(json.dumps({
                "contract": pb.SOURCE_GROUP_CONTRACT, "symbol": "f", "groups": [{}], "status": "preserved"}))
            with patch.object(pb, "ROOT", root), patch.object(pb, "bounded_capture") as run:
                with self.assertRaisesRegex(RuntimeError, "symlinked"):
                    pb.grouped_baseline_fidelity(item, out, root, self.plan_inputs(out), None)
                run.assert_not_called()

    def normalized(self, word, records):
        with patch.object(pb.reloc_surface, "Elf") as elf, patch.object(
                pb.reloc_surface, "_unique_symbol", return_value=(0, 4, ".text")):
            elf.return_value.section_bytes.return_value = word.to_bytes(4, "big")
            elf.return_value.relocations.return_value = records
            return pb.normalized_owned_instructions(Path("synthetic.o"), "f")

    def test_normalization_preserves_registers_and_nonrelocation_fields(self):
        record = [(".text", 0, 6, 1)]
        self.assertEqual(self.normalized(123, record), self.normalized(456, record))
        self.assertNotEqual(self.normalized(123, record), self.normalized((1 << 16) + 123, record))
        self.assertNotEqual(self.normalized(123, []), self.normalized(456, []))

    def test_duplicate_unknown_and_misaligned_relocations_refuse(self):
        for records in ([(".text", 0, 2, 1)], [(".text", 1, 6, 1)],
                        [(".text", 0, 6, 1), (".text", 0, 6, 1)]):
            with self.subTest(records=records), self.assertRaisesRegex(RuntimeError, "unsupported"):
                self.normalized(123, records)

    def source_records(self, name="a", value=0, info=16, section=0, word=4, duplicate=False):
        with patch.object(pb.reloc_surface, "Elf") as elf, patch.object(
                pb.reloc_surface, "_unique_symbol", return_value=(0, 4, ".text")):
            row = (name, value, 0, info, section)
            elf.return_value.symbols.return_value = [row, row] if duplicate else [row]
            elf.return_value.section_bytes.return_value = word.to_bytes(4, "big")
            elf.return_value.relocations.return_value = [(".text", 0, 4, 0)]
            return pb.raw_source_relocations(Path("synthetic.o"), "f")

    def test_source_correspondence_binds_name_type_binding_addend(self):
        original = self.source_records()
        self.assertEqual(original, self.source_records())
        for kwargs in ({"name": "b"}, {"info": 18}, {"info": 32}, {"word": 5}):
            self.assertNotEqual(original, self.source_records(**kwargs))
        self.assertNotEqual(self.source_records(section=0xFFF1, value=5),
                            self.source_records(section=0xFFF1, value=6))

    def test_defined_and_duplicate_source_names_refuse(self):
        for kwargs in ({"section": 1}, {"duplicate": True}):
            with self.assertRaisesRegex(RuntimeError, "independent"):
                self.source_records(**kwargs)

    def test_partial_target_allows_faithful_unsupported_source_but_not_wrong_addend(self):
        for wrong in (False, True):
            with self.subTest(wrong_addend=wrong), tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
                root = Path(tmp)
                out, scratch = root / "run", root / "run/scratch"
                scratch.mkdir(parents=True)
                item = pb.QueueItem("f", root / "src/f.c", overlay=1)
                target = root / "build_non_matching/src/f.c.o"
                target.parent.mkdir(parents=True)
                (out / "source-groups.json").write_text(json.dumps({"contract": pb.SOURCE_GROUP_CONTRACT,
                    "symbol": "f", "status": "measurement-required", "reason": "unsupported fixture", "groups": []}))
                script = b"current local importer recipe"
                (out / "importer-compile.sh").write_bytes(script)
                for name in ("base.c", "compile.sh", "target.s", "target.o", "settings.toml"):
                    (scratch / name).write_bytes(b"fixture")
                authorities = [root / name for name in ("build/mickey.us.elf", "rom", "atlas", "values")]
                for path in authorities:
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_bytes(b"authority")
                def run(args, deadline):
                    if "--analysis-only" in args:
                        target.write_bytes(bytes(4))
                        text = json.dumps({"candidate_object": "build_non_matching/src/f.c.o",
                                           "linked_symbol": "f", "preflight": {"status": "partial"}})
                    else:
                        Path(args[-1]).write_bytes(int(wrong).to_bytes(4, "big"))
                        text = "compiled"
                    return subprocess.CompletedProcess(args, 0, text)
                class Elf:
                    def __init__(self, path): self.data = path.read_bytes()
                    def section_bytes(self, name): return self.data
                    def symbols(self): return [("external", 0, 0, 16, 0)]
                    def relocations(self): return [(".text", 0, 4, 0)]
                for name, value in (("ROOT", root), ("BASEROM", authorities[1]), ("ATLAS_PATH", authorities[2])):
                    stack.enter_context(patch.object(pb, name, value))
                stack.enter_context(patch.object(pb.reloc_surface, "LINK_SYMS", authorities[3]))
                stack.enter_context(patch.object(pb.reloc_surface, "Elf", Elf))
                stack.enter_context(patch.object(pb.reloc_surface, "_unique_symbol", return_value=(0, 4, ".text")))
                stack.enter_context(patch.object(pb.reloc_surface, "function_surface_comparison", return_value={"stable_identity_exact": False}))
                stack.enter_context(patch.object(pb, "bounded_capture", side_effect=run))
                stack.enter_context(patch.object(pb, "validate_baseline"))
                stack.enter_context(patch.object(pb, "checked_tool_identity"))
                stack.enter_context(patch.object(pb, "measure_seed_stage", return_value=(
                    SimpleNamespace(source=b"void f(void) { return; }", object=bytes(4), object_sha256="fixture"), 10)))
                inputs = self.plan_inputs(out)
                inputs["context"]["importer_recipe"] = pb.compile_script_digest(script)
                if wrong:
                    with self.assertRaisesRegex(RuntimeError, "source-symbol"):
                        pb.grouped_baseline_fidelity(item, out, scratch, inputs, None)
                else:
                    pb.grouped_baseline_fidelity(item, out, scratch, inputs, None)
                    report = json.loads((out / "source-fidelity/report.json").read_text())
                    self.assertTrue(report["source_fidelity_exact"])
                    self.assertEqual(report["identity_route"], "raw-source-symbols-not-runtime-proof")


if __name__ == "__main__":
    unittest.main()
