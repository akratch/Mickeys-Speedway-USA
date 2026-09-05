#!/usr/bin/env python3
"""Focused source-level regression tests for reloc_surface safety gates."""

from __future__ import annotations

import contextlib
import io
import json
import os
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import reloc_surface as rs  # noqa: E402


class LinkedObjectCompletenessTests(unittest.TestCase):
    def test_missing_linker_object_fails_closed(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            (repo / "mickey.us.ld").write_text(
                "build/src/overlays/o001/present.c.o(.text)\n"
                "build/src/overlays/o002/missing.c.o(.text)\n"
            )
            present = repo / "build/src/overlays/o001/present.c.o"
            present.parent.mkdir(parents=True)
            present.write_bytes(b"placeholder")

            with mock.patch.object(rs, "REPO", repo):
                with self.assertRaises(SystemExit) as caught:
                    rs.linked_overlay_objects()

            message = str(caught.exception)
            self.assertIn("complete linker object set", message)
            self.assertIn("build/src/overlays/o002/missing.c.o", message)

    def test_complete_linker_object_set_is_returned_in_order(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            rels = [
                "build/src/overlays/o002/second.c.o",
                "build/src/overlays/o001/first.c.o",
            ]
            (repo / "mickey.us.ld").write_text(
                "\n".join(rel + "(.text)" for rel in rels) + "\n"
            )
            for rel in rels:
                path = repo / rel
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(b"placeholder")

            with mock.patch.object(rs, "REPO", repo):
                got = rs.linked_overlay_objects()

            self.assertEqual([(2, repo / rels[0]), (1, repo / rels[1])], got)


class ResidentRebindSafetyTests(unittest.TestCase):
    def test_dry_run_reports_plan_without_objcopy(self):
        objects = [(40, Path("build/src/overlays/o040/example.c.o"))]
        with mock.patch.object(rs, "resident_defined_names", return_value=set()):
            with mock.patch.object(
                rs,
                "resident_call_aliases",
                return_value=({"func_80000000": "func_80000000_o040Reloc"}, [], []),
            ):
                refusals, notes, planned = rs.rebind_resident_calls(
                    objects,
                    b"",
                    {40: []},
                    [None] * 40,
                    [],
                    records_cache={40: []},
                    apply=False,
                )

        self.assertEqual([], refusals)
        self.assertEqual([], notes)
        self.assertEqual(
            [("example.c.o", "func_80000000", "func_80000000_o040Reloc")],
            planned,
        )

    def test_check_mode_refuses_pending_rebind(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            config = repo / "config"
            config.mkdir()
            (config / "overlays.us.json").write_text(json.dumps({"modules": []}))
            rom = repo / "rom.z64"
            out = repo / "surface.txt"
            rom.write_bytes(b"")
            out.write_text("tracked\n")
            diag = {
                "pending_rebinds": [
                    ("example.c.o", "func_80000000", "func_80000000_o040Reloc")
                ]
            }

            stderr = io.StringIO()
            with mock.patch.object(rs, "REPO", repo):
                with mock.patch.object(rs, "generate", return_value=("generated\n", diag)) as gen:
                    with contextlib.redirect_stderr(stderr):
                        status = rs.cmd_generate(
                            ["--check", "--rom", str(rom), "--out", str(out)]
                        )

            self.assertEqual(1, status)
            self.assertFalse(gen.call_args.kwargs["mutate_objects"])
            self.assertIn("read-only generation will not modify", stderr.getvalue())


class FunctionSurfaceComparisonTests(unittest.TestCase):
    GENERATED_O22 = "func_overlay_022_F0000D30_1878E38"

    class BoundaryElf:
        def __init__(self, *, path=Path("missing"), names=(), symbols=(),
                     relocations=(), text=b""):
            self.path = Path(path)
            self.names = list(names)
            self._symbols = list(symbols)
            self._relocations = list(relocations)
            self._text = bytes(text)

        def section(self, name):
            if name == ".text" and ".text" in self.names:
                return self.names.index(".text"), object()
            return None, None

        def section_bytes(self, name):
            return self._text if name == ".text" else b""

        def symbols(self):
            return list(self._symbols)

        def relocations(self, target=r"\.text"):
            return list(self._relocations)

    def o22_atlas(self, ownership=None):
        if ownership is None:
            ownership = [{
                "offset": "0xD30",
                "end_offset": "0xE9C",
                "size": "0x16C",
                "type": "c",
                "source": "overlays/o022/overlay22RemoveObject",
            }]
        return {
            "modules": [{
                "overlay": 22,
                "identity": "overlay:22",
                "synthetic_vma": "0xF0000000",
                "rom": {"start": "0x1878108"},
                "sections": {
                    "text": {
                        "start": "0x1878108",
                        "end": "0x1878FA8",
                        "size": "0xEA0",
                    }
                },
                "text_ownership": ownership,
            }]
        }

    def o22_boundary_fixture(self, root, *, rtype=None, linked_value=None):
        source = root / "src/overlays/o022/overlay22RemoveObject.c"
        source.parent.mkdir(parents=True, exist_ok=True)
        source.write_text("void placeholder(void) {}\n", encoding="utf-8")
        obj = root / "build/src/overlays/o022/overlay22RemoveObject.c.o"
        obj.parent.mkdir(parents=True, exist_ok=True)
        obj.write_bytes(b"ELF placeholder")
        generated = self.GENERATED_O22
        candidate = self.BoundaryElf(
            names=["", ".text"],
            symbols=[(generated, 0, 0, 0, rs.SHN_UNDEF)],
            relocations=[(
                ".text", 0x274,
                rs.R_MIPS_26 if rtype is None else rtype,
                0,
            )],
            text=b"\0" * 0x2B0,
        )
        canonical = self.BoundaryElf(
            path=obj,
            names=["", ".text"],
            # Metadata keeps the pre-trim size; physical .text is the exact
            # atlas-owned function boundary.
            symbols=[(generated, 0, 0x1B0, rs.STT_FUNC, 1)],
            text=b"\0" * 0x16C,
        )
        target = self.BoundaryElf(
            names=["", ".overlay_022"],
            symbols=[(
                generated,
                rs.SYNTHETIC_VMA + 0xD30 if linked_value is None else linked_value,
                0x1B0,
                rs.STT_FUNC,
                1,
            )],
        )
        return candidate, canonical, target, source, obj

    def test_o22_generated_call_uses_unique_owned_function_boundary(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root)
            )
            resolved, ambiguous = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt",
                candidate,
                22,
                target,
                self.o22_atlas(),
                0,
                0x2B0,
                root=root,
                elf_loader=lambda _path: canonical,
            )

        self.assertEqual({self.GENERATED_O22: (22, 0xD30)}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_generated_call_accepts_named_function_in_multi_function_object(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root)
            )
            canonical._symbols.append(
                ("neighbor", 0x100, 0x20, rs.STT_FUNC, 1)
            )
            resolved, ambiguous = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt",
                candidate,
                22,
                target,
                self.o22_atlas(),
                0,
                0x2B0,
                root=root,
                elf_loader=lambda _path: canonical,
            )

        self.assertEqual({self.GENERATED_O22: (22, 0xD30)}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_generated_call_rejects_duplicate_named_function(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root)
            )
            canonical._symbols.append(
                (self.GENERATED_O22, 0, 0x1B0, rs.STT_FUNC, 1)
            )
            with self.assertRaisesRegex(
                rs.SurfaceComparisonError, "ambiguous function symbols"
            ):
                rs._stable_overlay_call_identities(
                    root / "missing-aliases.txt",
                    candidate,
                    22,
                    target,
                    self.o22_atlas(),
                    0,
                    0x2B0,
                    root=root,
                    elf_loader=lambda _path: canonical,
                )

    def test_generated_call_rejects_ambiguous_ownership(self):
        owner = self.o22_atlas()["modules"][0]["text_ownership"][0]
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root)
            )
            with self.assertRaisesRegex(
                rs.SurfaceComparisonError, "ambiguous overlapping atlas owners"
            ):
                rs._stable_overlay_call_identities(
                    root / "missing-aliases.txt", candidate, 22, target,
                    self.o22_atlas([dict(owner), dict(owner)]), 0, 0x2B0,
                    root=root, elf_loader=lambda _path: canonical,
                )

    def test_generated_call_rejects_cross_overlay_and_non_call(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root)
            )
            cross, _ = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt", candidate, 21, target,
                self.o22_atlas(), 0, 0x2B0, root=root,
                elf_loader=lambda _path: canonical,
            )
            non_call_candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root, rtype=rs.R_MIPS_HI16)
            )
            non_call, _ = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt", non_call_candidate, 22, target,
                self.o22_atlas(), 0, 0x2B0, root=root,
                elf_loader=lambda _path: canonical,
            )

        self.assertEqual({}, cross)
        self.assertEqual({}, non_call)

    def test_generated_call_rejects_section_only_and_missing_boundary(self):
        broad = [{
            "offset": "0x0",
            "end_offset": "0xEA0",
            "size": "0xEA0",
            "type": "c",
            "source": "overlays/o022/overlay_022",
        }]
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(root)
            )
            section_only, _ = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt", candidate, 22, target,
                self.o22_atlas(broad), 0, 0x2B0, root=root,
                elf_loader=lambda _path: canonical,
            )
            missing, _ = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt", candidate, 22, target,
                self.o22_atlas([]), 0, 0x2B0, root=root,
                elf_loader=lambda _path: canonical,
            )

        self.assertEqual({}, section_only)
        self.assertEqual({}, missing)

    def test_generated_call_rejects_stale_object(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, source, obj = (
                self.o22_boundary_fixture(root)
            )
            newer = obj.stat().st_mtime + 5
            os.utime(source, (newer, newer))
            resolved, _ = rs._stable_overlay_call_identities(
                root / "missing-aliases.txt", candidate, 22, target,
                self.o22_atlas(), 0, 0x2B0, root=root,
                elf_loader=lambda _path: canonical,
            )

        self.assertEqual({}, resolved)

    def test_generated_call_refuses_conflicting_linked_symbol(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            candidate, canonical, target, _source, _obj = (
                self.o22_boundary_fixture(
                    root, linked_value=rs.SYNTHETIC_VMA + 0xE00
                )
            )
            with self.assertRaisesRegex(
                rs.SurfaceComparisonError,
                "linked symbol conflicts with canonical overlay ownership",
            ):
                rs._stable_overlay_call_identities(
                    root / "missing-aliases.txt", candidate, 22, target,
                    self.o22_atlas(), 0, 0x2B0, root=root,
                    elf_loader=lambda _path: canonical,
                )

    def test_redefine_alias_propagates_stable_identity(self):
        class FakeElf:
            def __init__(self, target=False):
                self.target = target

            def section(self, name):
                self.assert_name = name
                return 1, object()

            def symbols(self):
                old = (
                    ("func_80005750", 0x80005750, 4, 0, 1)
                    if self.target
                    else ("func_80005750", 0, 0, 0, rs.SHN_UNDEF)
                )
                return [
                    old,
                    ("func_80005750_o001Reloc", 0, 0, 0, rs.SHN_UNDEF),
                ]

        resolved, ambiguous = rs._stable_symbol_identities(
            Path("missing"), FakeElf(), 1, 0x3578, FakeElf(target=True),
            {"func_80005750_o001Reloc": "func_80005750"},
        )
        self.assertEqual(
            resolved["func_80005750_o001Reloc"], (0, 0x5300),
        )
        self.assertNotIn("func_80005750_o001Reloc", ambiguous)

    def test_resident_absolute_symbol_uses_absolute_identity_namespace(self):
        class FakeElf:
            def __init__(self, target=False):
                self.target = target

            def symbols(self):
                return [
                    ("D_7BE08", 0x7BE08 if self.target else 0,
                     0, 0, rs.SHN_ABS if self.target else rs.SHN_UNDEF),
                ]

            def section(self, _name):
                return None, None

        resolved, ambiguous = rs._stable_symbol_identities(
            Path("missing"), FakeElf(), None, 0, FakeElf(target=True)
        )
        self.assertEqual(
            (rs.ri.ABSOLUTE_IDENTITY, 0x7BE08), resolved["D_7BE08"]
        )
        self.assertNotIn("D_7BE08", ambiguous)

    def test_transitive_redefine_alias_propagates_stable_identity(self):
        class FakeElf:
            def __init__(self, target=False):
                self.target = target

            def section(self, _name):
                return 1, object()

            def symbols(self):
                original = (
                    ("func_80005750", 0x80005750, 4, 0, 1)
                    if self.target
                    else ("func_80005750", 0, 0, 0, rs.SHN_UNDEF)
                )
                return [
                    original,
                    ("middle", 0, 0, 0, rs.SHN_UNDEF),
                    ("final", 0, 0, 0, rs.SHN_UNDEF),
                ]

        resolved, ambiguous = rs._stable_symbol_identities(
            Path("missing"),
            FakeElf(),
            1,
            0x3578,
            FakeElf(target=True),
            {"middle": "func_80005750", "final": "func_80005750"},
        )
        self.assertEqual((0, 0x5300), resolved["final"])
        self.assertNotIn("final", ambiguous)

    def test_exact_surface_counts_shape_and_identity(self):
        target = [
            rs.SurfaceRecord(0x10, rs.R_MIPS_26, (0, 0x1234)),
            rs.SurfaceRecord(0x28, rs.R_MIPS_HI16, (7, 0x1BA8)),
            rs.SurfaceRecord(0x2C, rs.R_MIPS_LO16, (7, 0x1BA8)),
        ]
        result = rs.compare_record_sets(target, list(target))

        self.assertEqual(3, result["target_runtime_record_count"])
        self.assertEqual(3, result["candidate_record_count"])
        self.assertEqual(3, result["offset_type_alignment_count"])
        self.assertEqual(3, result["stable_identity_alignment_count"])
        self.assertTrue(result["offset_type_exact"])
        self.assertTrue(result["stable_identity_exact"])

    def test_shifted_surface_does_not_count_as_aligned(self):
        target = [rs.SurfaceRecord(0x10, rs.R_MIPS_26, (0, 0x1234))]
        candidate = [rs.SurfaceRecord(0x14, rs.R_MIPS_26, (0, 0x1234))]

        result = rs.compare_record_sets(target, candidate)

        self.assertEqual(0, result["offset_type_alignment_count"])
        self.assertEqual(0, result["stable_identity_alignment_count"])
        self.assertFalse(result["offset_type_exact"])
        self.assertFalse(result["stable_identity_exact"])

    def test_wrong_identity_preserves_shape_but_not_identity(self):
        target = [rs.SurfaceRecord(0x10, rs.R_MIPS_26, (7, 0xCCC))]
        candidate = [rs.SurfaceRecord(0x10, rs.R_MIPS_26, (59, 0x70))]

        result = rs.compare_record_sets(target, candidate)

        self.assertEqual(1, result["offset_type_alignment_count"])
        self.assertEqual(0, result["stable_identity_alignment_count"])
        self.assertTrue(result["offset_type_exact"])
        self.assertFalse(result["stable_identity_exact"])

    def test_ambiguous_overlay_owner_fails_closed(self):
        row = {
            "type": "c",
            "source": "overlays/o007/example",
            "offset": "0x0",
            "end_offset": "0x20",
            "size": "0x20",
        }
        atlas = {
            "modules": [
                {"overlay": 7, "text_ownership": [dict(row)]},
                {"overlay": 8, "text_ownership": [dict(row)]},
            ]
        }

        with self.assertRaisesRegex(rs.SurfaceComparisonError,
                                    "2 overlay text owners"):
            rs.resolve_overlay_ownership(
                Path("build/src/overlays/o007/example.c.o"), atlas)


class OverlayDataIdentityTests(unittest.TestCase):
    class FakeElf:
        def __init__(self, path, names, symbols=(), relocations=(), text=b"",
                     section_headers=None):
            self.path = Path(path)
            self.names = list(names)
            self._symbols = list(symbols)
            self._relocations = list(relocations)
            self._text = bytes(text)
            self._section_headers = section_headers or {}

        def section(self, name):
            if name in self.names:
                return (self.names.index(name),
                        self._section_headers.get(name, object()))
            return None, None

        def section_bytes(self, name):
            return self._text if name == ".text" else b""

        def symbols(self):
            return list(self._symbols)

        def relocations(self, target=r"\.text"):
            return list(self._relocations)

    def fixture(self, root, *, overlay=7, name="D_0", assignment=0,
                object_value=0, linked_offset=0xFC0, object_section=".data",
                linked_overlay=None, sites=None, stale_object=False,
                stale_link=False, extra_linked=(), linked_name=True):
        linked_overlay = overlay if linked_overlay is None else linked_overlay
        caller = "overlays/o%03d/caller" % overlay
        owner = "overlays/o%03d/owner" % overlay
        caller_source = root / "src" / (caller + ".c")
        owner_source = root / "src" / (owner + ".c")
        candidate_path = root / "build_non_matching/src" / (caller + ".c.o")
        owner_path = root / "build/src" / (owner + ".c.o")
        target_path = root / "build/mickey.us.elf"
        for path in (caller_source, owner_source, candidate_path,
                     owner_path, target_path):
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(b"fixture")
        now = max(path.stat().st_mtime_ns for path in
                  (caller_source, owner_source, candidate_path,
                   owner_path, target_path))
        os.utime(caller_source, ns=(now, now))
        os.utime(owner_source, ns=(now, now))
        os.utime(candidate_path, ns=(now + 1, now + 1))
        os.utime(owner_path, ns=(now + 1, now + 1))
        os.utime(target_path, ns=(now + 2, now + 2))
        if stale_object:
            os.utime(owner_source, ns=(now + 3, now + 3))
        if stale_link:
            os.utime(target_path, ns=(now, now))

        if sites is None:
            sites = [(0, rs.R_MIPS_HI16), (4, rs.R_MIPS_LO16)]
        candidate = self.FakeElf(
            candidate_path, ["", ".text"],
            [(name, 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", offset, rtype, 0) for offset, rtype in sites],
            b"\0" * 0x40)
        canonical_name = name if linked_name else "gCanonicalStorage"
        canonical = self.FakeElf(
            owner_path, ["", object_section],
            [(canonical_name, object_value, 0, rs.STT_OBJECT, 1)],
            section_headers={
                object_section: (0, 0, 0, 0, 0,
                                 0x2A0 if object_section == ".bss" else 0x950,
                                 0, 0, 0, 0),
            })
        linked_section = ".overlay_%03d%s" % (
            linked_overlay, "_bss" if object_section == ".bss" else "")
        target = self.FakeElf(
            target_path, ["", linked_section] + [row[4] for row in extra_linked],
            [(name, assignment, 0, rs.STT_OBJECT | 0x10, rs.SHN_ABS)] +
            ([(name, rs.SYNTHETIC_VMA + linked_offset, 0,
               rs.STT_OBJECT, 1)] if linked_name else []) +
             [row[:4] + (2 + i,) for i, row in enumerate(extra_linked)],
            section_headers={
                linked_section: (
                    0, 0, 0,
                    rs.SYNTHETIC_VMA + (0x1ED0 if object_section == ".bss"
                                        else 0),
                    0, 0x2A0 if object_section == ".bss" else 0x1ED0,
                    0, 0, 0, 0),
            })
        module = {
            "overlay": overlay,
            "identity": "overlay:%d" % overlay,
            "synthetic_vma": "0xF0000000",
            "rom": {"size": "0x1ED0"},
            "bss_size": "0x2B0",
            "sections": {
                "text": {"size": "0xFC0"},
                "data_rodata": {"size": "0x950"},
            },
            "text_ownership": [
                {"type": "c", "source": caller},
                {"type": "c", "source": owner},
            ],
            "data_rodata_ownership": ([{
                "offset": "0x0", "end_offset": "0x950", "size": "0x950",
                "type": "c", "section": object_section, "source": owner,
            }] if object_section != ".bss" else None),
        }
        values = root / "values.txt"
        values.write_text("%s = 0x%X;\n" % (name, assignment))
        return candidate, canonical, target, module, values

    def resolve(self, fixture, *, loader=None):
        candidate, canonical, target, module, values = fixture
        return rs._stable_overlay_data_identities(
            values, candidate, module, target, 0, 0x40,
            root=values.parent,
            elf_loader=loader or (lambda _path: canonical))

    def test_generic_data_pair_authenticates_without_hand_alias(self):
        with tempfile.TemporaryDirectory() as td:
            resolved, ambiguous = self.resolve(self.fixture(Path(td)))
        self.assertEqual({"D_0": (7, 0xFC0)}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_multiple_hilo_references_share_candidate_side_identity(self):
        sites = [(0, rs.R_MIPS_HI16), (4, rs.R_MIPS_LO16),
                 (8, rs.R_MIPS_HI16), (12, rs.R_MIPS_LO16)]
        with tempfile.TemporaryDirectory() as td:
            resolved, _ = self.resolve(self.fixture(Path(td), sites=sites))
        self.assertEqual((7, 0xFC0), resolved["D_0"])

    def test_unpaired_hilo_is_not_authenticated(self):
        with tempfile.TemporaryDirectory() as td:
            resolved, _ = self.resolve(self.fixture(
                Path(td), sites=[(0, rs.R_MIPS_HI16)]))
        self.assertEqual({}, resolved)

    def test_bss_definition_uses_exact_overlay_offset(self):
        with tempfile.TemporaryDirectory() as td:
            resolved, _ = self.resolve(self.fixture(
                Path(td), name="gPool", assignment=0x284,
                object_value=0x284, linked_offset=0x2154,
                object_section=".bss"))
        self.assertEqual((7, 0x1B94), resolved["gPool"])

    def test_friendly_name_without_linked_alias_uses_unique_whole_bss(self):
        with tempfile.TemporaryDirectory() as td:
            resolved, _ = self.resolve(self.fixture(
                Path(td), name="gFriendlyState", assignment=0x11C,
                object_value=0x11C, object_section=".bss",
                linked_name=False))
        self.assertEqual((7, 0x1A2C), resolved["gFriendlyState"])

    def test_same_numeric_assignment_in_two_overlays_uses_caller_context(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            fixture = self.fixture(root, name="D_0")
            candidate, canonical, target, module, values = fixture
            target.names.append(".overlay_008")
            target._symbols.append(("D_0", rs.SYNTHETIC_VMA + 0xFC0, 0,
                                    rs.STT_OBJECT, 2))
            resolved, _ = self.resolve(
                (candidate, canonical, target, module, values))
        self.assertEqual((7, 0xFC0), resolved["D_0"])

    def test_ambiguous_same_overlay_aliases_fail_closed(self):
        with tempfile.TemporaryDirectory() as td:
            fixture = self.fixture(Path(td), name="D_0")
            fixture[2]._symbols.append(
                ("D_0", rs.SYNTHETIC_VMA + 0xFC4, 0, rs.STT_OBJECT, 1))
            with self.assertRaisesRegex(
                    rs.SurfaceComparisonError, "ambiguous overlay definitions"):
                self.resolve(fixture)

    def test_cross_overlay_definition_is_not_used(self):
        with tempfile.TemporaryDirectory() as td:
            resolved, _ = self.resolve(self.fixture(
                Path(td), linked_overlay=8))
        self.assertEqual({}, resolved)

    def test_stale_object_or_linked_elf_is_not_used(self):
        with tempfile.TemporaryDirectory() as td:
            stale_object, _ = self.resolve(self.fixture(
                Path(td) / "object", stale_object=True))
            stale_link, _ = self.resolve(self.fixture(
                Path(td) / "link", stale_link=True))
        self.assertEqual({}, stale_object)
        self.assertEqual({}, stale_link)

    def test_conflicting_assignment_fails_closed(self):
        with tempfile.TemporaryDirectory() as td:
            fixture = self.fixture(Path(td), assignment=4, object_value=0)
            with self.assertRaisesRegex(
                    rs.SurfaceComparisonError, "canonical object"):
                self.resolve(fixture)

    def test_overlay_numeric_value_never_borrows_target_identity(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("D_0", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0)], b"\0" * 8)
        target = [
            rs.SurfaceRecord(0, rs.R_MIPS_HI16, (7, 0xFC0), 0),
            rs.SurfaceRecord(4, rs.R_MIPS_LO16, (7, 0xFC0), 0),
        ]
        records = rs._candidate_surface_records(
            candidate, 0, 8, target, {}, {"D_0": 0}, set(), 7)
        self.assertTrue(all(record.identity is None for record in records))

    def test_authenticated_hilo_applies_signed_rel_addend(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("D_base", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0)],
            struct.pack(">II", 0, 0xFFFC))
        records = rs._candidate_surface_records(
            candidate, 0, 8, [], {"D_base": (7, 0x1000)}, {}, set(), 7)
        self.assertEqual([(7, 0xFFC), (7, 0xFFC)],
                         [record.identity for record in records])

    def test_ambiguous_hilo_identity_remains_structurally_visible(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("D_base", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0)], b"\0" * 8)
        records = rs._candidate_surface_records(
            candidate, 0, 8, [], {"D_base": (7, 0x1000)}, {},
            {"D_base"}, 7)
        self.assertEqual([None, None], [record.identity for record in records])

    def test_ambiguous_overlay_call_remains_structurally_visible(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("overlayCall", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_26, 0)], b"\0" * 4)
        records = rs._candidate_surface_records(
            candidate, 0, 4, [], {}, {}, set(), 7,
            {"overlayCall": (7, 0x200)}, {"overlayCall"})
        self.assertEqual([None], [record.identity for record in records])

    @staticmethod
    def runtime_module(overlay):
        return {
            "overlay": overlay,
            "identity": "overlay:%d" % overlay,
            "synthetic_vma": "0xF0000000",
        }

    def test_runtime_correlated_local_pair_resolves_o29_proxy(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("gOverlay29MinimumYReloc", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0x20, rs.R_MIPS_HI16, 0),
             (".text", 0x24, rs.R_MIPS_LO16, 0)],
            b"\0" * 0x28)
        target = [
            rs.SurfaceRecord(0x20, rs.R_MIPS_HI16, (29, 0x16E4), 20),
            rs.SurfaceRecord(0x24, rs.R_MIPS_LO16, (29, 0x16E4), 20),
        ]

        resolved, ambiguous = rs._runtime_correlated_overlay_hilo_identities(
            candidate, self.runtime_module(29), 0, 0x28, target)

        self.assertEqual({"gOverlay29MinimumYReloc": (29, 0x16E4)}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_runtime_correlated_pairs_resolve_o41_proxy_addends(self):
        symbols = [
            ("gQueueActive", 0, 0, 0, rs.SHN_UNDEF),
            ("gQueueEntries", 0, 0, 0, rs.SHN_UNDEF),
        ]
        text = bytearray(0x10)
        struct.pack_into(">I", text, 0x4, 11)
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"], symbols,
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0),
             (".text", 8, rs.R_MIPS_HI16, 1),
             (".text", 12, rs.R_MIPS_LO16, 1)],
            bytes(text))
        target = [
            rs.SurfaceRecord(0, rs.R_MIPS_HI16, (0xFFF, 0x512E3), 11),
            rs.SurfaceRecord(4, rs.R_MIPS_LO16, (0xFFF, 0x512E3), 11),
            rs.SurfaceRecord(8, rs.R_MIPS_HI16, (0xFFF, 0x512E4), 12),
            rs.SurfaceRecord(12, rs.R_MIPS_LO16, (0xFFF, 0x512E4), 12),
        ]

        resolved, ambiguous = rs._runtime_correlated_overlay_hilo_identities(
            candidate, self.runtime_module(41), 0, 0x10, target)

        self.assertEqual(
            {"gQueueActive": (0xFFF, 0x512D8),
             "gQueueEntries": (0xFFF, 0x512E4)},
            resolved)
        self.assertEqual(set(), ambiguous)

    def test_shared_vma_overlay_identities_remain_distinct(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("gOverlay7", 0, 0, 0, rs.SHN_UNDEF),
             ("gOverlay8", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0),
             (".text", 8, rs.R_MIPS_HI16, 1),
             (".text", 12, rs.R_MIPS_LO16, 1)],
            b"\0" * 0x10)
        target = [
            rs.SurfaceRecord(0, rs.R_MIPS_HI16, (7, 0xFC0), 0),
            rs.SurfaceRecord(4, rs.R_MIPS_LO16, (7, 0xFC0), 0),
            rs.SurfaceRecord(8, rs.R_MIPS_HI16, (8, 0xFC0), 0),
            rs.SurfaceRecord(12, rs.R_MIPS_LO16, (8, 0xFC0), 0),
        ]

        resolved, ambiguous = rs._runtime_correlated_overlay_hilo_identities(
            candidate, self.runtime_module(7), 0, 0x10, target)

        self.assertEqual((7, 0xFC0), resolved["gOverlay7"])
        self.assertEqual((8, 0xFC0), resolved["gOverlay8"])
        self.assertEqual(set(), ambiguous)

    def test_conflicting_overlay_identities_for_one_proxy_fail_closed(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("gAmbiguous", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0),
             (".text", 8, rs.R_MIPS_HI16, 0),
             (".text", 12, rs.R_MIPS_LO16, 0)],
            b"\0" * 0x10)
        target = [
            rs.SurfaceRecord(0, rs.R_MIPS_HI16, (7, 0xFC0), 0),
            rs.SurfaceRecord(4, rs.R_MIPS_LO16, (7, 0xFC0), 0),
            rs.SurfaceRecord(8, rs.R_MIPS_HI16, (8, 0xFC0), 0),
            rs.SurfaceRecord(12, rs.R_MIPS_LO16, (8, 0xFC0), 0),
        ]

        resolved, ambiguous = rs._runtime_correlated_overlay_hilo_identities(
            candidate, self.runtime_module(7), 0, 0x10, target)

        self.assertNotIn("gAmbiguous", resolved)
        self.assertIn("gAmbiguous", ambiguous)

    def test_defined_candidate_symbol_requires_canonical_section_owner(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text", ".data"],
            [("gCandidateData", 0, 4, rs.STT_OBJECT, 2)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0)],
            b"\0" * 8)
        target = [
            rs.SurfaceRecord(0, rs.R_MIPS_HI16, (7, 0xFC0), 0),
            rs.SurfaceRecord(4, rs.R_MIPS_LO16, (7, 0xFC0), 0),
        ]

        resolved, ambiguous = rs._runtime_correlated_overlay_hilo_identities(
            candidate, self.runtime_module(7), 0, 8, target)

        self.assertEqual({}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_runtime_correlated_proxy_keeps_pre_objcopy_name(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("D_current", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0, rs.R_MIPS_HI16, 0),
             (".text", 4, rs.R_MIPS_LO16, 0)],
            b"\0" * 8)
        target = [
            rs.SurfaceRecord(0, rs.R_MIPS_HI16, (7, 0xFC0), 0),
            rs.SurfaceRecord(4, rs.R_MIPS_LO16, (7, 0xFC0), 0),
        ]

        resolved, ambiguous = rs._runtime_correlated_overlay_hilo_identities(
            candidate, self.runtime_module(7), 0, 8, target,
            {"D_current": "gOriginalProxy"})

        self.assertEqual({"gOriginalProxy": (7, 0xFC0)}, resolved)
        self.assertEqual(set(), ambiguous)


class RepeatedOverlayCallIdentityTests(unittest.TestCase):
    FakeElf = OverlayDataIdentityTests.FakeElf

    @staticmethod
    def module(overlay=101):
        return {
            "overlay": overlay,
            "identity": "overlay:%d" % overlay,
            "synthetic_vma": "0xF0000000",
        }

    def candidate(self, *, sites=(0x3C, 0x6C), words=None,
                  shndx=rs.SHN_UNDEF, extra_relocations=()):
        size = max(sites + tuple(row[1] for row in extra_relocations)) + 4
        text = bytearray(size)
        for offset, word in (words or {}).items():
            struct.pack_into(">I", text, offset, word)
        return self.FakeElf(
            Path("missing"), ["", ".text", ".data"],
            [("overlay101BuildIntensityColorsReloc", 0, 0, 0, shndx)],
            ([('.text', offset, rs.R_MIPS_26, 0) for offset in sites]
             + list(extra_relocations)),
            bytes(text))

    @staticmethod
    def target(sites=(0x3C, 0x6C), identities=None):
        identities = identities or [(101, 0x2CE4)] * len(sites)
        return [
            rs.SurfaceRecord(offset, rs.R_MIPS_26, identity, 0)
            for offset, identity in zip(sites, identities)
        ]

    def test_repeated_o101_proxy_resolves_one_same_overlay_base(self):
        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(), self.module(), 0, 0x70, self.target()))

        self.assertEqual(
            {"overlay101BuildIntensityColorsReloc": (101, 0x2CE4)},
            resolved)
        self.assertEqual(set(), ambiguous)

    def test_each_candidate_rel_addend_derives_same_base(self):
        candidate = self.candidate(
            words={0x3C: 1, 0x6C: 2})
        target = self.target(
            identities=[(101, 0x2CE8), (101, 0x2CEC)])

        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                candidate, self.module(), 0, 0x70, target))

        self.assertEqual((101, 0x2CE4),
                         resolved["overlay101BuildIntensityColorsReloc"])
        self.assertEqual(set(), ambiguous)

    def test_single_site_is_not_an_independent_witness(self):
        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(sites=(0x3C,)), self.module(), 0, 0x40,
                self.target(sites=(0x3C,))))

        self.assertEqual({}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_conflicting_repeated_bases_are_ambiguous(self):
        target = self.target(
            identities=[(101, 0x2CE4), (101, 0x2CF0)])

        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(), self.module(), 0, 0x70, target))

        self.assertEqual({}, resolved)
        self.assertIn("overlay101BuildIntensityColorsReloc", ambiguous)

    def test_duplicate_runtime_shape_is_ambiguous(self):
        target = self.target()
        target.append(rs.SurfaceRecord(
            0x3C, rs.R_MIPS_26, (101, 0x2CE4), 0))

        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(), self.module(), 0, 0x70, target))

        self.assertEqual({}, resolved)
        self.assertIn("overlay101BuildIntensityColorsReloc", ambiguous)

    def test_cross_overlay_runtime_tuple_is_unresolved(self):
        target = self.target(
            identities=[(101, 0x2CE4), (102, 0x2CE4)])

        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(), self.module(), 0, 0x70, target))

        self.assertEqual({}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_nonzero_runtime_call_addend_is_ambiguous(self):
        target = self.target()
        target[1] = rs.SurfaceRecord(
            0x6C, rs.R_MIPS_26, (101, 0x2CE4), 4)

        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(), self.module(), 0, 0x70, target))

        self.assertEqual({}, resolved)
        self.assertIn("overlay101BuildIntensityColorsReloc", ambiguous)

    def test_defined_or_mixed_type_proxy_is_not_runtime_correlated(self):
        defined, defined_ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(shndx=2), self.module(), 0, 0x70,
                self.target()))
        mixed_candidate = self.candidate(
            extra_relocations=((".text", 0x70, rs.R_MIPS_HI16, 0),))
        mixed, mixed_ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                mixed_candidate, self.module(), 0, 0x74, self.target()))

        self.assertEqual(({}, set()), (defined, defined_ambiguous))
        self.assertEqual(({}, set()), (mixed, mixed_ambiguous))

    def test_missing_aligned_runtime_tuple_remains_unresolved(self):
        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                self.candidate(), self.module(), 0, 0x70,
                self.target(sites=(0x3C,))))

        self.assertEqual({}, resolved)
        self.assertEqual(set(), ambiguous)

    def test_call_identity_pipeline_uses_repeated_runtime_witness(self):
        candidate = self.candidate()
        empty_target = self.FakeElf(Path("missing"), [""], [], [], b"")

        resolved, ambiguous = rs._stable_overlay_call_identities(
            Path("missing"), candidate, 101, empty_target, {}, 0, 0x70,
            module=self.module(), target_records=self.target())

        self.assertEqual(
            {"overlay101BuildIntensityColorsReloc": (101, 0x2CE4)},
            resolved)
        self.assertEqual(set(), ambiguous)

    def test_call_identity_pipeline_rejects_cross_overlay_owner(self):
        candidate = self.candidate()
        empty_target = self.FakeElf(Path("missing"), [""], [], [], b"")

        with self.assertRaisesRegex(
                rs.SurfaceComparisonError, "owner conflicts"):
            rs._stable_overlay_call_identities(
                Path("missing"), candidate, 100, empty_target, {}, 0, 0x70,
                module=self.module(), target_records=self.target())

    def test_o101_draw_mixed_groups_resolve_local_and_leave_resident_partial(self):
        symbols = [
            ("overlay101SetScissor2Reloc", 0, 0, 0, rs.SHN_UNDEF),
            ("overlay101SetTransformModeReloc", 0, 0, 0, rs.SHN_UNDEF),
        ]
        sites = [(0x1B8, 0), (0x1DC, 1), (0x22C, 1), (0x27C, 0)]
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"], symbols,
            [(".text", offset, rs.R_MIPS_26, index)
             for offset, index in sites],
            b"\0" * 0x280)
        target = [
            rs.SurfaceRecord(0x1B8, rs.R_MIPS_26, (101, 0x1F80), 0),
            rs.SurfaceRecord(0x1DC, rs.R_MIPS_26, (0, 0x34994), 0),
            rs.SurfaceRecord(0x22C, rs.R_MIPS_26, (0, 0x34994), 0),
            rs.SurfaceRecord(0x27C, rs.R_MIPS_26, (101, 0x1F80), 0),
        ]
        empty_target = self.FakeElf(Path("missing"), [""], [], [], b"")

        resolved, ambiguous = rs._stable_overlay_call_identities(
            Path("missing"), candidate, 101, empty_target, {}, 0, 0x280,
            module=self.module(), target_records=target)
        records = rs._candidate_surface_records(
            candidate, 0, 0x280, target, {}, {}, set(), 101,
            resolved, ambiguous)

        self.assertEqual(
            {"overlay101SetScissor2Reloc": (101, 0x1F80)}, resolved)
        self.assertEqual(set(), ambiguous)
        self.assertEqual(2, sum(record.identity is not None for record in records))
        self.assertEqual(2, sum(record.identity is None for record in records))

    def test_o3_repeated_resident_proxy_remains_partial_not_ambiguous(self):
        candidate = self.FakeElf(
            Path("missing"), ["", ".text"],
            [("overlay3RandomRangeReloc", 0, 0, 0, rs.SHN_UNDEF)],
            [(".text", 0x11C, rs.R_MIPS_26, 0),
             (".text", 0x15C, rs.R_MIPS_26, 0)],
            b"\0" * 0x160)
        target = [
            rs.SurfaceRecord(0x11C, rs.R_MIPS_26, (0, 0x2952C), 0),
            rs.SurfaceRecord(0x15C, rs.R_MIPS_26, (0, 0x2952C), 0),
        ]

        resolved, ambiguous = (
            rs._runtime_correlated_overlay_r26_identities(
                candidate, self.module(3), 0, 0x160, target))
        records = rs._candidate_surface_records(
            candidate, 0, 0x160, target, {}, {}, set(), 3,
            resolved, ambiguous)

        self.assertEqual({}, resolved)
        self.assertEqual(set(), ambiguous)
        self.assertTrue(all(record.identity is None for record in records))


class MatchedOverlayRelocationWitnessTests(unittest.TestCase):
    class FakeElf:
        def __init__(self, path, names, symbols=(), relocations=(), sections=None):
            self.path = Path(path)
            self.names = list(names)
            self._symbols = list(symbols)
            self._relocations = list(relocations)
            self._sections = dict(sections or {})

        def section(self, name):
            if name not in self.names:
                return None, None
            data = self._sections.get(name, b"")
            return self.names.index(name), (0, 0, 0, 0, 0, len(data), 0, 0, 0, 0)

        def section_bytes(self, name):
            return self._sections.get(name, b"")

        def symbols(self):
            return list(self._symbols)

        def relocations(self, target=r"\.text"):
            return list(self._relocations)

    def fixture(self, root, *, matched=True, owners=1):
        overlay = 7
        rom_start = 0x100
        row_size = 0x20
        linked_bytes = b"\0" * (row_size * owners)
        rom = bytearray(rom_start + len(linked_bytes))
        rom[rom_start:] = linked_bytes
        rows = []
        canonicals = {}
        linked_symbols = []
        for index in range(owners):
            source = "overlays/o007/witness%d" % index
            source_path = root / "src" / (source + ".c")
            object_path = root / "build" / "src" / (source + ".c.o")
            source_path.parent.mkdir(parents=True, exist_ok=True)
            object_path.parent.mkdir(parents=True, exist_ok=True)
            source_path.write_text("void witness%d(void) {}\n" % index)
            object_path.write_bytes(b"ELF fixture")
            now = max(source_path.stat().st_mtime_ns,
                      object_path.stat().st_mtime_ns)
            os.utime(source_path, ns=(now, now))
            os.utime(object_path, ns=(now + 1, now + 1))
            function = "witness%d" % index
            canonical = self.FakeElf(
                object_path, ["", ".text"],
                symbols=[
                    ("gSharedProxy", 0, 0, 0, rs.SHN_UNDEF),
                    (function, 0, row_size, rs.STT_FUNC, 1),
                ],
                relocations=[
                    (".text", 0x8, rs.R_MIPS_HI16, 0),
                    (".text", 0xC, rs.R_MIPS_LO16, 0),
                ],
                sections={".text": b"\0" * row_size},
            )
            canonicals[object_path] = canonical
            start = index * row_size
            rows.append({
                "offset": "0x%X" % start,
                "end_offset": "0x%X" % (start + row_size),
                "size": "0x%X" % row_size,
                "type": "c",
                "source": source,
                "matched": matched,
                "nonmatching": False,
            })
            linked_symbols.append((
                function, rs.SYNTHETIC_VMA + start, row_size,
                rs.STT_FUNC, 1,
            ))
        target_path = root / "build/mickey.us.elf"
        target_path.parent.mkdir(parents=True, exist_ok=True)
        target_path.write_bytes(b"ELF fixture")
        newest = max(path.stat().st_mtime_ns for path in canonicals)
        os.utime(target_path, ns=(newest + 1, newest + 1))
        target = self.FakeElf(
            target_path, ["", ".overlay_007"],
            symbols=linked_symbols,
            sections={".overlay_007": linked_bytes},
        )
        module = {
            "overlay": overlay,
            "identity": "overlay:7",
            "synthetic_vma": "0xF0000000",
            "rom": {"start": "0x100", "size": "0x%X" % len(linked_bytes)},
            "sections": {"text": {"size": "0x%X" % len(linked_bytes)}},
            "text_ownership": rows,
        }
        runtime_module = {"overlay": overlay, "rom_start": rom_start}
        return bytes(rom), module, runtime_module, target, canonicals

    def call_fixture(self, root, *, matched=True, owners=1):
        fixture = self.fixture(root, matched=matched, owners=owners)
        for canonical in fixture[-1].values():
            canonical._relocations = [
                (".text", 0x8, rs.R_MIPS_26, 0),
            ]
        return fixture

    def test_exact_function_sized_sibling_authenticates_proxy_name(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = self.fixture(root)
            records = [
                rs.SurfaceRecord(0x8, rs.R_MIPS_HI16, (7, 0xFC0)),
                rs.SurfaceRecord(0xC, rs.R_MIPS_LO16, (7, 0xFC0)),
            ]
            with mock.patch.object(rs, "_target_runtime_records",
                                   return_value=records):
                witnessed = rs._matched_overlay_relocation_witnesses(
                    module, target, rom, runtime_module, {"gSharedProxy"},
                    root=root, elf_loader=lambda path: canonicals[path])
        self.assertEqual({(7, 0xFC0)}, witnessed["gSharedProxy"])

    def test_nonmatched_owner_is_not_a_witness(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = self.fixture(
                root, matched=False)
            with mock.patch.object(rs, "_target_runtime_records") as runtime:
                witnessed = rs._matched_overlay_relocation_witnesses(
                    module, target, rom, runtime_module, {"gSharedProxy"},
                    root=root, elf_loader=lambda path: canonicals[path])
        self.assertEqual({}, witnessed)
        runtime.assert_not_called()

    def test_linked_rom_mismatch_is_not_a_witness(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = self.fixture(root)
            mismatched_rom = bytearray(rom)
            mismatched_rom[0x100] = 1
            with mock.patch.object(rs, "_target_runtime_records") as runtime:
                witnessed = rs._matched_overlay_relocation_witnesses(
                    module, target, bytes(mismatched_rom), runtime_module,
                    {"gSharedProxy"}, root=root,
                    elf_loader=lambda path: canonicals[path])
        self.assertEqual({}, witnessed)
        runtime.assert_not_called()

    def test_conflicting_exact_witnesses_remain_ambiguous(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = self.fixture(
                root, owners=2)
            values = root / "values.txt"
            values.write_text("gSharedProxy = 0x0;\n")
            candidate = self.FakeElf(
                root / "build_non_matching/src/overlays/o007/caller.c.o",
                ["", ".text"],
                symbols=[("gSharedProxy", 0, 0, 0, rs.SHN_UNDEF)],
                relocations=[
                    (".text", 0, rs.R_MIPS_HI16, 0),
                    (".text", 4, rs.R_MIPS_LO16, 0),
                ],
                sections={".text": b"\0" * 8},
            )

            def runtime_records(_rom, _context, start, _size):
                identity = (7, 0xFC0 if start == 0 else 0x1FC0)
                return [
                    rs.SurfaceRecord(0x8, rs.R_MIPS_HI16, identity),
                    rs.SurfaceRecord(0xC, rs.R_MIPS_LO16, identity),
                ]

            with mock.patch.object(rs, "_target_runtime_records",
                                   side_effect=runtime_records):
                resolved, ambiguous = rs._stable_overlay_data_identities(
                    values, candidate, module, target, 0, 8,
                    root=root, elf_loader=lambda path: canonicals[path],
                    rom=rom, runtime_module=runtime_module)
        self.assertNotIn("gSharedProxy", resolved)
        self.assertIn("gSharedProxy", ambiguous)

    def test_witness_identity_does_not_align_shifted_offset(self):
        identity = (7, 0xFC0)
        result = rs.compare_record_sets(
            [rs.SurfaceRecord(0x8, rs.R_MIPS_HI16, identity)],
            [rs.SurfaceRecord(0xC, rs.R_MIPS_HI16, identity)],
        )
        self.assertEqual(0, result["offset_type_alignment_count"])
        self.assertEqual(0, result["stable_identity_alignment_count"])

    def test_exact_sibling_authenticates_call_proxy_name(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = (
                self.call_fixture(root)
            )
            object_text = bytearray(0x20)
            struct.pack_into(">I", object_text, 0x8, 1)
            next(iter(canonicals.values()))._sections[".text"] = bytes(object_text)
            records = [
                rs.SurfaceRecord(0x8, rs.R_MIPS_26, (0, 0x1234)),
            ]
            with mock.patch.object(rs, "_target_runtime_records",
                                   return_value=records):
                witnessed = rs._matched_overlay_relocation_witnesses(
                    module, target, rom, runtime_module, {"gSharedProxy"},
                    root=root, elf_loader=lambda path: canonicals[path])
        self.assertEqual({(0, 0x1230)}, witnessed["gSharedProxy"])

    def test_nonmatched_sibling_does_not_authenticate_call_proxy(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = (
                self.call_fixture(root, matched=False)
            )
            with mock.patch.object(rs, "_target_runtime_records") as runtime:
                witnessed = rs._matched_overlay_relocation_witnesses(
                    module, target, rom, runtime_module, {"gSharedProxy"},
                    root=root, elf_loader=lambda path: canonicals[path])
        self.assertEqual({}, witnessed)
        runtime.assert_not_called()

    def test_conflicting_call_proxy_witnesses_remain_ambiguous(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            rom, module, runtime_module, target, canonicals = (
                self.call_fixture(root, owners=2)
            )
            candidate = self.FakeElf(
                root / "build_non_matching/src/overlays/o007/caller.c.o",
                ["", ".text"],
                symbols=[("gSharedProxy", 0, 0, 0, rs.SHN_UNDEF)],
                relocations=[(".text", 0, rs.R_MIPS_26, 0)],
                sections={".text": b"\0" * 8},
            )

            def runtime_records(_rom, _context, start, _size):
                identity = (0, 0x1234 if start == 0 else 0x5678)
                return [rs.SurfaceRecord(0x8, rs.R_MIPS_26, identity)]

            with mock.patch.object(rs, "_target_runtime_records",
                                   side_effect=runtime_records):
                resolved, ambiguous = rs._stable_overlay_call_identities(
                    root / "missing-aliases.txt", candidate, 7, target,
                    {"modules": []}, 0, 8, root=root,
                    elf_loader=lambda path: canonicals[path], module=module,
                    rom=rom, runtime_module=runtime_module)
        self.assertNotIn("gSharedProxy", resolved)
        self.assertIn("gSharedProxy", ambiguous)

    def test_call_proxy_witness_does_not_align_shifted_offset(self):
        identity = (0, 0x1234)
        result = rs.compare_record_sets(
            [rs.SurfaceRecord(0x8, rs.R_MIPS_26, identity)],
            [rs.SurfaceRecord(0xC, rs.R_MIPS_26, identity)],
        )
        self.assertEqual(0, result["offset_type_alignment_count"])
        self.assertEqual(0, result["stable_identity_alignment_count"])


class ResidentTargetRangeTests(unittest.TestCase):
    class FakeElf:
        def __init__(self, path, section_name, section_address, section_data,
                     symbols, relocations=()):
            self.path = Path(path)
            self.names = ["", section_name]
            self._section_name = section_name
            self._section_address = section_address
            self._section_data = bytes(section_data)
            self._symbols = list(symbols)
            self._relocations = list(relocations)

        def section(self, name):
            if name != self._section_name:
                return None, None
            return 1, (0, 0, 0, self._section_address, 0,
                       len(self._section_data), 0, 0, 0, 0)

        def section_bytes(self, name):
            return self._section_data if name == self._section_name else b""

        def symbols(self):
            return list(self._symbols)

        def relocations(self, target=r"\.text"):
            return list(self._relocations)

    def _fixture(self, repo):
        candidate = repo / "build_non_matching/src/main/fx.c.o"
        target_path = repo / "build/src/main/fx.c.o"
        candidate.parent.mkdir(parents=True)
        target_path.parent.mkdir(parents=True)
        candidate.write_bytes(b"candidate")
        target_path.write_bytes(b"target")

        object_start = 0x100
        size = 0x190
        names = [
            ("func_800498FC", object_start, size, 2, 1),
            ("D_800D5F58", 0, 0, 0, rs.SHN_UNDEF),
            ("func_80021FB0", 0, 0, 0, rs.SHN_UNDEF),
            ("camGetMode", 0, 0, 0, rs.SHN_UNDEF),
        ]
        relative = [
            (0x2C, rs.R_MIPS_HI16, 1),
            (0x30, rs.R_MIPS_LO16, 1),
            (0x88, rs.R_MIPS_26, 2),
            (0x9C, rs.R_MIPS_26, 3),
            (0xC4, rs.R_MIPS_26, 2),
        ]
        relocations = [
            (".text", object_start + offset, rtype, symbol_index)
            for offset, rtype, symbol_index in relative
        ]
        object_data = bytearray(object_start + size)
        linked_start = 0x80049800
        linked_value = 0x800498FC
        linked_data = bytearray(0x300)
        linked_offset = linked_value - linked_start
        linked_data[linked_offset:linked_offset + size] = object_data[
            object_start:object_start + size]
        addresses = {
            1: 0x800D5F58,
            2: 0x80021FB0,
            3: 0x80012340,
        }
        for offset, rtype, symbol_index in relative:
            address = addresses[symbol_index]
            if rtype == rs.R_MIPS_26:
                word = (0x03 << 26) | ((address >> 2) & 0x03FFFFFF)
            elif rtype == rs.R_MIPS_HI16:
                word = (0x0F << 26) | (((address + 0x8000) >> 16) & 0xFFFF)
            else:
                word = (0x0D << 26) | (address & 0xFFFF)
            struct.pack_into(">I", linked_data, linked_offset + offset, word)

        target_object = self.FakeElf(
            target_path, ".text", 0, object_data, names, relocations)
        linked_symbols = [
            ("func_800498FC", linked_value, size, 2, 1),
            ("D_800D5F58", 0x800D5F58, 4, 1, 1),
            ("func_80021FB0", 0x80021FB0, 4, 2, 1),
            ("camGetMode", 0x80012340, 4, 2, 1),
        ]
        linked = self.FakeElf(
            repo / "build/mickey.us.elf", ".main", linked_start,
            linked_data, linked_symbols)
        return candidate, target_path, target_object, linked, linked_value, size

    def test_func_800498fc_boundary_yields_five_static_tuples(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            candidate, target_path, target_object, linked, value, size = \
                self._fixture(repo)

            def open_elf(path):
                self.assertEqual(target_path, Path(path))
                return target_object

            with mock.patch.object(rs, "REPO", repo), \
                    mock.patch.object(rs, "Elf", side_effect=open_elf):
                records = rs._resident_target_records(
                    candidate, "main/fx", linked, "func_800498FC",
                    value, size, ".main", repo / "missing-values.txt")

            self.assertEqual([0x2C, 0x30, 0x88, 0x9C, 0xC4],
                             [record.offset for record in records])
            self.assertTrue(all(record.identity is not None for record in records))
            self.assertEqual(records[0].identity, records[1].identity)

    def _pc16_fixture(self, repo, destination_offset=0x40, addend_words=-1):
        candidate, target_path, target, linked, value, size = self._fixture(repo)
        offset = 0x10
        object_start = target.symbols()[0][1]
        # A global auxiliary label causes GAS to retain PC16 with A=-4;
        # the final branch still targets S, because the architectural PC is P+4.
        word = (4 << 26) | (1 << 21)
        object_data = bytearray(target._section_data)
        struct.pack_into(">I", object_data, object_start + offset,
                         word | (addend_words & 0xFFFF))
        names = target.symbols() + [("local_case", object_start + destination_offset, 0, 18, 1)]
        relocations = target.relocations() + [(".text", object_start + offset, rs.R_MIPS_PC16, len(names) - 1)]
        target = self.FakeElf(target_path, ".text", 0, object_data, names, relocations)
        effective_offset = destination_offset + addend_words * 4 + 4
        linked_data = bytearray(linked._section_data)
        struct.pack_into(">I", linked_data, value - linked._section_address + offset,
                         word | (((effective_offset - offset - 4) // 4) & 0xFFFF))
        linked = self.FakeElf(linked.path, ".main", linked._section_address, linked_data,
                              linked.symbols() + [("local_case", value + destination_offset, 0, 18, 1)])
        return candidate, target, linked, value, size, effective_offset

    def _pc16_records(self, repo, fixture):
        candidate, target, linked, value, size, _ = fixture
        with mock.patch.object(rs, "REPO", repo), mock.patch.object(rs, "Elf", return_value=target):
            return rs._resident_target_records(candidate, None, linked, "func_800498FC",
                                                value, size, ".main", repo / "missing-values.txt")

    def test_pc16_proves_forward_backward_and_nonzero_addend_without_dropping_tuple(self):
        for destination, addend in ((0x40, -1), (0, -1), (0x40, 1)):
            with self.subTest(destination=destination, addend=addend), tempfile.TemporaryDirectory() as td:
                repo = Path(td)
                fixture = self._pc16_fixture(repo, destination, addend)
                records = self._pc16_records(repo, fixture)
                self.assertEqual(len(records), 6)
                branch = next(record for record in records if record.rtype == rs.R_MIPS_PC16)
                self.assertEqual(branch.offset, 0x10)
                self.assertEqual(branch.identity, (0, fixture[3] + fixture[5] - rs.ot.RESIDENT_VRAM_BASE))

    def test_pc16_refuses_wrong_opcode_or_linked_displacement(self):
        for bit, error in ((1 << 21, "opcode/register"), (1, "does not reproduce")):
            with self.subTest(bit=bit), tempfile.TemporaryDirectory() as td:
                repo = Path(td)
                fixture = list(self._pc16_fixture(repo))
                linked = fixture[2]
                data = bytearray(linked._section_data)
                offset = fixture[3] - linked._section_address + 0x10
                word = struct.unpack_from(">I", data, offset)[0]
                struct.pack_into(">I", data, offset, word ^ bit)
                linked._section_data = bytes(data)
                with self.assertRaisesRegex(rs.SurfaceComparisonError, error):
                    self._pc16_records(repo, fixture)

    def test_pc16_refuses_external_unaligned_and_out_of_function_destinations(self):
        for kind in ("external", "unaligned", "outside"):
            with self.subTest(kind=kind), tempfile.TemporaryDirectory() as td:
                repo = Path(td)
                fixture = self._pc16_fixture(repo)
                target = fixture[1]
                name, value, size, info, section = target._symbols[-1]
                if kind == "external":
                    section = rs.SHN_UNDEF
                else:
                    value += 1 if kind == "unaligned" else fixture[4]
                target._symbols[-1] = (name, value, size, info, section)
                with self.assertRaisesRegex(rs.SurfaceComparisonError, "resident PC16"):
                    self._pc16_records(repo, fixture)

    def test_pc16_refuses_symbol_address_inconsistent_with_local_layout(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            fixture = self._pc16_fixture(repo)
            linked = fixture[2]
            name, value, size, info, section = linked._symbols[-1]
            linked._symbols[-1] = (name, value + 4, size, info, section)
            with self.assertRaisesRegex(rs.SurfaceComparisonError, "symbol identity disagrees"):
                self._pc16_records(repo, fixture)

    def test_pc16_refuses_nonbranch_instruction(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            fixture = self._pc16_fixture(repo)
            target = fixture[1]
            data = bytearray(target._section_data)
            struct.pack_into(">I", data, target.symbols()[0][1] + 0x10, 0)
            target._section_data = bytes(data)
            with self.assertRaisesRegex(rs.SurfaceComparisonError, "supported branch"):
                self._pc16_records(repo, fixture)

    def test_conflicting_source_assertion_fails_closed(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            candidate, *_rest = self._fixture(repo)
            with mock.patch.object(rs, "REPO", repo):
                with self.assertRaisesRegex(rs.SurfaceComparisonError,
                                            "source is ambiguous"):
                    rs.resolve_resident_target_object(candidate, "main/other")

    def test_sparse_runtime_identity_overrides_matching_static_tuple(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            candidate, _target_path, target_object, linked, value, size = \
                self._fixture(repo)
            runtime = [
                rs.SurfaceRecord(0x88, rs.R_MIPS_26, (7, 0x1234), 0)
            ]
            with mock.patch.object(rs, "REPO", repo), \
                    mock.patch.object(rs, "Elf", return_value=target_object):
                records = rs._resident_target_records(
                    candidate, None, linked, "func_800498FC", value, size,
                    ".main", repo / "missing-values.txt", runtime)

            by_shape = {(record.offset, record.rtype): record for record in records}
            self.assertEqual((7, 0x1234),
                             by_shape[(0x88, rs.R_MIPS_26)].identity)
            self.assertEqual(5, len(records))

    def test_missing_canonical_target_object_fails_closed(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            candidate = repo / "build_non_matching/src/main/fx.c.o"
            candidate.parent.mkdir(parents=True)
            candidate.write_bytes(b"candidate")
            with mock.patch.object(rs, "REPO", repo):
                with self.assertRaisesRegex(rs.SurfaceComparisonError,
                                            "missing canonical resident"):
                    rs.resolve_resident_target_object(candidate)

    def test_nonrelocation_byte_mismatch_fails_closed(self):
        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            candidate, target_path, target_object, linked, value, size = \
                self._fixture(repo)
            linked_data = bytearray(linked._section_data)
            linked_data[value - linked._section_address + 4] = 1
            bad_linked = self.FakeElf(
                linked.path, ".main", linked._section_address, linked_data,
                linked.symbols())

            with mock.patch.object(rs, "REPO", repo), \
                    mock.patch.object(rs, "Elf", return_value=target_object):
                with self.assertRaisesRegex(
                        rs.SurfaceComparisonError, "outside relocation words"):
                    rs._resident_target_records(
                        candidate, None, bad_linked, "func_800498FC",
                        value, size, ".main", repo / "missing-values.txt")


def _site(module_off, rtype, symbol="s", in_table=False):
    return {"symbol": symbol, "obj_off": module_off, "type": rtype,
            "module_off": module_off, "table_off": module_off if in_table
            else None, "shifted": False, "stored": None, "obj": None,
            "in_table": in_table, "op": None, "note": ""}


def _record(target_offset, mode, op_name="LOCAL"):
    return {"target_offset": target_offset, "mode": mode, "op_name": op_name}


class OrderPreservingEmbeddingTests(unittest.TestCase):
    """The alignment rule itself, on synthetic sequences: no ROM data."""

    def embed(self, candidate, retail):
        return rs.unique_order_preserving_embedding(candidate, retail)

    def test_identical_sequence_is_the_identity(self):
        seq = [rs.R_MIPS_HI16, rs.R_MIPS_LO16, rs.R_MIPS_26]
        self.assertEqual([0, 1, 2], self.embed(seq, list(seq)))

    def test_extra_retail_record_of_an_unused_type_is_skipped(self):
        # An intra-module JUMP the object resolves itself leaves an R_MIPS_26
        # record with no candidate site; the rest still embeds uniquely.
        candidate = [rs.R_MIPS_HI16, rs.R_MIPS_LO16]
        retail = [rs.R_MIPS_26, rs.R_MIPS_HI16, rs.R_MIPS_LO16]
        self.assertEqual([1, 2], self.embed(candidate, retail))

    def test_ambiguous_placement_is_refused(self):
        # Two ways to lay one HI16 down over two HI16 records: choosing either
        # would be inventing an addend.
        self.assertIsNone(self.embed([rs.R_MIPS_HI16],
                                     [rs.R_MIPS_HI16, rs.R_MIPS_HI16]))

    def test_a_type_the_retail_order_cannot_supply_is_refused(self):
        self.assertIsNone(self.embed([rs.R_MIPS_HI16, rs.R_MIPS_26],
                                     [rs.R_MIPS_26, rs.R_MIPS_HI16]))

    def test_more_candidate_sites_than_records_is_refused(self):
        self.assertIsNone(self.embed([rs.R_MIPS_HI16, rs.R_MIPS_HI16],
                                     [rs.R_MIPS_HI16]))

    def test_empty_candidate_embeds_trivially(self):
        self.assertEqual([], self.embed([], [rs.R_MIPS_HI16]))


class SiteAlignmentTests(unittest.TestCase):
    """`align_sites` over synthetic site and record lists: no ROM data."""

    def test_fully_corroborated_object_is_not_realigned(self):
        sites = [_site(0x10, rs.R_MIPS_HI16, in_table=True),
                 _site(0x14, rs.R_MIPS_LO16, in_table=True)]
        records = [_record(0x10, rs.R_MIPS_HI16),
                   _record(0x14, rs.R_MIPS_LO16)]
        summary = rs.align_sites(sites, records, 0, 0x100)
        self.assertFalse(summary["attempted"])
        self.assertEqual(2, summary["aligned"])
        self.assertEqual(0, summary["shifted"])
        self.assertEqual([0x10, 0x14], [s["table_off"] for s in sites])

    def test_a_shifted_site_is_aligned_and_counted_as_shifted(self):
        # The candidate moved its LO16 two words later; the HI16 still lands
        # where the table names it.
        sites = [_site(0x10, rs.R_MIPS_HI16, in_table=True),
                 _site(0x1C, rs.R_MIPS_LO16)]
        records = [_record(0x10, rs.R_MIPS_HI16),
                   _record(0x14, rs.R_MIPS_LO16)]
        summary = rs.align_sites(sites, records, 0, 0x100)
        self.assertTrue(summary["attempted"])
        self.assertIsNone(summary["refused"])
        self.assertEqual(2, summary["aligned"])
        self.assertEqual(1, summary["shifted"])
        self.assertEqual([False, True], [s["shifted"] for s in sites])
        self.assertEqual([0x10, 0x14], [s["table_off"] for s in sites])
        self.assertTrue(all(s["in_table"] for s in sites))

    def test_an_exact_offset_that_belongs_to_another_record_is_moved(self):
        # A site can sit exactly on a record and still belong to the previous
        # one: the order is authoritative, so the alignment overrides the
        # offset coincidence and reports the move.
        sites = [_site(0x14, rs.R_MIPS_HI16, in_table=True),
                 _site(0x18, rs.R_MIPS_HI16),
                 _site(0x1C, rs.R_MIPS_LO16),
                 _site(0x20, rs.R_MIPS_LO16)]
        records = [_record(0x10, rs.R_MIPS_HI16),
                   _record(0x14, rs.R_MIPS_HI16),
                   _record(0x18, rs.R_MIPS_LO16),
                   _record(0x1C, rs.R_MIPS_LO16)]
        summary = rs.align_sites(sites, records, 0, 0x100)
        self.assertEqual(4, summary["aligned"])
        self.assertEqual(4, summary["shifted"])
        self.assertEqual([0x10, 0x14, 0x18, 0x1C],
                         [s["table_off"] for s in sites])

    def test_ambiguous_alignment_refuses_and_leaves_every_site_unvalued(self):
        sites = [_site(0x10, rs.R_MIPS_HI16),
                 _site(0x20, rs.R_MIPS_LO16)]
        records = [_record(0x10, rs.R_MIPS_HI16),
                   _record(0x14, rs.R_MIPS_HI16),
                   _record(0x18, rs.R_MIPS_LO16),
                   _record(0x1C, rs.R_MIPS_LO16)]
        summary = rs.align_sites(sites, records, 0, 0x100)
        self.assertTrue(summary["attempted"])
        self.assertIn("no unique order-preserving alignment",
                      summary["refused"])
        self.assertEqual(0, summary["aligned"])
        self.assertEqual([None, None], [s["table_off"] for s in sites])
        self.assertEqual([False, False], [s["in_table"] for s in sites])

    def test_records_outside_the_translation_unit_are_not_candidates(self):
        # The pool is bounded by the object's own text range, so the next
        # module function's records cannot absorb a shifted site.
        sites = [_site(0x10, rs.R_MIPS_HI16), _site(0x14, rs.R_MIPS_LO16)]
        records = [_record(0x10, rs.R_MIPS_HI16),
                   _record(0x14, rs.R_MIPS_LO16),
                   _record(0x40, rs.R_MIPS_HI16),
                   _record(0x44, rs.R_MIPS_LO16)]
        summary = rs.align_sites(sites, records, 0, 0x20)
        self.assertEqual(2, summary["aligned"])
        self.assertEqual(0, summary["shifted"])

    def test_unmapped_sites_are_ignored_by_the_alignment(self):
        unmapped = _site(0x10, rs.R_MIPS_HI16)
        unmapped["module_off"] = None
        sites = [unmapped, _site(0x18, rs.R_MIPS_LO16)]
        records = [_record(0x14, rs.R_MIPS_LO16)]
        summary = rs.align_sites(sites, records, 0, 0x100)
        self.assertEqual(1, summary["sites"])
        self.assertEqual(1, summary["aligned"])
        self.assertEqual(0x14, sites[1]["table_off"])


class PermuterTargetCoverageTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="annotation-synthetic-")
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.bin = rs.REPO / "tools/binutils"
        self.prelude = (".set noreorder\n.set noat\n.text\n"
            ".macro glabel name\n.globl \\name\n.type \\name,@function\n\\name:\n.endm\n"
            ".macro endlabel name\n.size \\name,.-\\name\n.endm\n")
        self.ops = ["lui $at, 1", "lw $t0, -32768($at)", "lw $t1, -32764($at)",
                    "lui $at, 1", "lw $t2, -32760($at)", "jal 0", "nop"]
        raw = self.assemble("raw", "glabel fixture\n" + "\n".join(self.ops) + "\nendlabel fixture\n")
        self.words = rs.Elf(raw).section_bytes(".text")[:28]
        self.rom = bytes(0x100) + self.words
        self.text = "glabel fixture\n" + "\n".join(
            f"/* {0x100+i*4:X} {rs.SYNTHETIC_VMA+i*4:08X} {struct.unpack_from('>I', self.words, i*4)[0]:08X} */ {op}"
            for i, op in enumerate(self.ops)) + "\nendlabel fixture\n"
        candidate = ["lui $at, %hi(known)", "lw $t0, %lo(known)($at)",
                     "lw $t1, %lo(known+4)($at)", "lui $at, %hi(known+8)",
                     "lw $t2, %lo(known+8)($at)", "jal call_target", "nop"]
        self.base = self.assemble("base", "glabel fixture\n" + "\n".join(candidate) + "\nendlabel fixture\n")
        self.records = [dict(table=1, index=i, target_offset=offset, mode=mode,
                             op=0 if mode == 4 else 1, op_name="SYMBOL" if mode == 4 else "LOCAL",
                             symbol_index=100, target_overlay=0, target_symbol_offset=64)
                        for i, (offset, mode) in enumerate(((0,5),(4,6),(8,6),(12,5),(16,6),(20,4)))]
        self.module = dict(overlay=1, rom_start=0x100, text_size=28, data_size=0)

    def assemble(self, name, text):
        source, obj = self.root / (name + ".s"), self.root / (name + ".o")
        source.write_text(self.prelude + text)
        subprocess.run([str(self.bin / "mips64-elf-as"), "-march=vr4300", "-32", "-o", str(obj), str(source)],
                       check=True, capture_output=True)
        return obj

    def annotate(self, text=None, records=None, empty_candidate=False, shifted_candidate=False):
        elf = rs.Elf(self.base)
        if empty_candidate:
            elf.relocations = lambda *args: []
        elif shifted_candidate:
            relocations = [(sec, 8 if off == 12 else off, kind, sym)
                           for sec, off, kind, sym in elf.relocations()]
            elf.relocations = lambda *args: relocations
        with mock.patch.object(rs.ot, "read_headers", return_value=[]), \
             mock.patch.object(rs.ot, "build_modules", return_value=[self.module]), \
             mock.patch.object(rs, "Elf", return_value=elf):
            return rs.permuter_annotation(self.text if text is None else text, self.base, ["fixture"], 1,
                                         self.rom, self.records if records is None else records)

    def roundtrip(self, text, notes):
        obj = self.assemble("annotated", text)
        proof = json.loads(next(note[len("target-proof: "):] for note in notes if note.startswith("target-proof: ")))
        self.assertEqual(sorted((offset, mode) for _, offset, mode, _ in rs.Elf(obj).relocations()),
                         sorted((r["target_offset"], r["mode"]) for r in self.records))
        linked = self.root / "linked.elf"
        args = [str(self.bin / "mips64-elf-ld"), "-m", "elf32ebmip", "-Ttext", "0", "-e", "0",
                "-o", str(linked), str(obj)]
        for name, value in proof["values"].items():
            args.extend(["--defsym", f"{name}=0x{value:X}"])
        subprocess.run(args, check=True, capture_output=True)
        self.assertEqual(rs.Elf(linked).section_bytes(".text")[:28], self.words)
        return obj

    def test_complete_target_independent_of_candidate_with_signed_shared_low_roundtrip(self):
        text, renames, notes = self.annotate(empty_candidate=True)
        self.assertEqual(renames, {})
        self.assertIn("6/6", notes[1])
        self.assertIn(self.text.splitlines()[-2], text)
        self.roundtrip(text, notes)

    def test_proven_shared_low_candidate_renames_score_zero(self):
        text, renames, notes = self.annotate()
        self.assertEqual(renames["known"], "__ovval_00008000")
        self.assertIn("%lo(__ovval_00008000+0x4)", text)
        target = self.roundtrip(text, notes)
        candidate = self.root / "renamed.o"
        args = [str(self.bin / "mips64-elf-objcopy")]
        args.extend(f"--redefine-sym={old}={new}" for old, new in renames.items())
        subprocess.run([*args, str(self.base), str(candidate)], check=True, capture_output=True)
        program = ("import sys;sys.path.insert(0,sys.argv[1]);from src.scorer import Scorer;"
                   "print(Scorer(sys.argv[2],stack_differences=True,algorithm='difflib',debug_mode=False,"
                   "ign_branch_targets=False,objdump_command=sys.argv[4]).score(sys.argv[3])[0])")
        score = subprocess.check_output([str(rs.REPO / ".venv/bin/python"), "-c", program,
            str(rs.REPO / "tools/permuter"), str(target), str(candidate),
            str(self.bin / "mips64-elf-objdump") + " -drz -m mips:4300"], text=True)
        self.assertEqual(score.strip(), "0")

    def test_duplicate_address_record_and_out_of_owner_fail_closed(self):
        with self.assertRaises(rs.AnnotationError):
            self.annotate(text=self.text + self.text.splitlines()[1] + "\n")
        for records in (self.records + [self.records[0]],
                        [{**self.records[0], "target_offset": 32}, *self.records[1:]],
                        [{**self.records[0], "mode": 2}, *self.records[1:]],
                        [{**self.records[0], "target_offset": -4}, *self.records[1:]]):
            with self.subTest(records=records), self.assertRaises(rs.AnnotationError):
                self.annotate(records=records)

    def test_conflicting_candidate_identity_does_not_hide_target_sites(self):
        records = [dict(record) for record in self.records]
        records[3]["symbol_index"] = records[4]["symbol_index"] = 200
        text, renames, notes = self.annotate(records=records)
        self.assertNotIn("known", renames)
        self.assertIn("ambiguous", " ".join(notes))
        self.roundtrip(text, notes)

    def test_one_coincident_pair_cannot_bind_shifted_remaining_symbol_sites(self):
        text, renames, notes = self.annotate(shifted_candidate=True)
        self.assertNotIn("known", renames)
        self.assertIn("incomplete candidate", " ".join(notes))
        self.roundtrip(text, notes)

    def test_equal_zero_bases_do_not_merge_distinct_runtime_identities(self):
        source = ("glabel fixture\n"
                  "lui $at,%hi(left+0x8000)\nlw $t0,%lo(left+0x8000)($at)\n"
                  "lw $t1,%lo(left+0x8004)($at)\nlui $at,%hi(right+0x8008)\n"
                  "lw $t2,%lo(right+0x8008)($at)\njal call_target\nnop\nendlabel fixture\n")
        self.base = self.assemble("distinct", source)
        same_text, same_renames, same_notes = self.annotate()
        self.assertEqual(same_renames["left"], "__ovval_00000000")
        self.assertEqual(same_renames["right"], "__ovval_00000000")
        self.roundtrip(same_text, same_notes)
        for changes in ({"symbol_index": 200}, {"op_name": "DATA", "op": 3}):
            records = [dict(record) for record in self.records]
            records[3].update(changes)
            records[4].update(changes)
            with self.subTest(changes=changes):
                text, renames, notes = self.annotate(records=records)
                self.assertNotIn("left", renames)
                self.assertNotIn("right", renames)
                self.assertIn("distinct runtime identities", " ".join(notes))
                self.roundtrip(text, notes)

    def test_bad_word_operand_and_incomplete_pair_are_refused(self):
        with self.assertRaises(rs.AnnotationError):
            self.annotate(text=self.text.replace("lui $at, 1", "lui $v0, 1", 1))
        with self.assertRaises(rs.AnnotationError):
            self.annotate(text=self.text.replace("3C010001", "3C010002", 1))
        with self.assertRaises(rs.AnnotationError):
            self.annotate(records=[self.records[0]])

    def test_footer_padding_and_neighbor_records_are_not_owned(self):
        self.module["text_size"] = 36
        footer = "/* 11C F000001C 00000000 */ nop\n/* 120 F0000020 00000000 */ nop\n"
        records = [*self.records, {**self.records[0], "target_offset": 28, "mode": 4, "index": 6}]
        text, _, notes = self.annotate(text=self.text + footer, records=records)
        self.assertTrue(text.endswith(footer))
        self.assertIn("6/6", notes[1])
        self.roundtrip(text, notes)

    def test_caller_applies_target_only_and_proves_nonrelocation_words(self):
        import permute_batch as pb
        from types import SimpleNamespace
        text, renames, notes = self.annotate(empty_candidate=True)
        rom = self.root / "rom.bin"
        rom.write_bytes(self.rom)
        target = self.root / "target.s"
        original = self.prelude + self.text
        target.write_text(original)
        compile_script = self.root / "compile.sh"
        compile_script.write_text("manual recipe preserved\n")
        item = SimpleNamespace(func="fixture", overlay=1)
        with mock.patch.object(pb, "BASEROM", rom), \
             mock.patch.object(pb, "find_asm_target", return_value=None), \
             mock.patch.object(pb, "ASSEMBLER_COMMAND", str(self.bin / "mips64-elf-as") + " -march=vr4300 -32"), \
             mock.patch.object(pb.reloc_surface, "permuter_annotation", return_value=(self.prelude + text, renames, notes)):
            self.assertEqual(pb.annotate_overlay_scratch(item, self.root, self.root), 0)
            self.assertIn("__ovtarget_", target.read_text())
            self.assertEqual(compile_script.read_text(), "manual recipe preserved\n")
        # ROM-correct comment tokens cannot hide an edited nonrelocation operand.
        target.write_text(original)
        bad = (self.prelude + text).replace("*/ nop", "*/ addiu $v0, $zero, 1")
        with mock.patch.object(pb, "BASEROM", rom), \
             mock.patch.object(pb, "find_asm_target", return_value=None), \
             mock.patch.object(pb, "ASSEMBLER_COMMAND", str(self.bin / "mips64-elf-as") + " -march=vr4300 -32"), \
             mock.patch.object(pb.reloc_surface, "permuter_annotation", return_value=(bad, renames, notes)):
            self.assertEqual(pb.annotate_overlay_scratch(item, self.root, self.root), 0)
            self.assertEqual(target.read_text(), original)
            self.assertIn("target proof failed", (self.root / "annotation.txt").read_text())
        with mock.patch.object(pb, "BASEROM", rom), \
             mock.patch.object(pb, "find_asm_target", return_value=None), \
             mock.patch.object(pb.reloc_surface, "permuter_annotation", return_value=(original, {}, [])), \
             mock.patch.object(pb, "bounded_capture") as capture:
            self.assertEqual(pb.annotate_overlay_scratch(item, self.root, self.root), 0)
            capture.assert_not_called()


class AnnotationScratchTransactionTests(unittest.TestCase):
    def exercise(self, phase, kind, missing_target=False, preservation_failure=False):
        import permute_batch as pb
        import time
        from types import SimpleNamespace
        with tempfile.TemporaryDirectory(prefix="annotation-transaction-") as directory:
            root = Path(directory)
            scratch, output = root / "scratch", root / "output"
            scratch.mkdir()
            output.mkdir()
            before = {"target.s": b"original target\r\n", "target.o": b"original target object",
                      "compile.sh": b"original script without newline", "base.o": b"original base"}
            if missing_target:
                del before["target.o"]
            for name, data in before.items():
                (scratch / name).write_bytes(data)
                (scratch / name).chmod(0o750 if name == "compile.sh" else 0o640)
            (output / "annotation.txt").write_text("prior diagnostic\n")
            rom = root / "rom"
            rom.write_bytes(b"synthetic")
            error = (subprocess.TimeoutExpired([phase], 1, output="partial sentinel") if kind == "timeout"
                     else RuntimeError("batch cancelled") if kind == "cancel"
                     else KeyboardInterrupt("interrupted sentinel") if kind == "interrupt"
                     else subprocess.CalledProcessError(7, [phase], output="failure sentinel"))
            calls = []
            def fault():
                if kind == "cancel":
                    pb.CANCEL_EVENT.set()
                raise error
            def capture(command, deadline, **kwargs):
                calls.append(command)
                step = "refresh" if command[0] == "bash" else "assembly"
                target = scratch / ("base.o" if step == "refresh" else "target.o")
                target.write_bytes(b"failed attempt artifact")
                target.chmod(0o600)
                if step == phase:
                    fault()
                return subprocess.CompletedProcess(command, 0, "ok", "")
            def proof(*args):
                if phase == "proof":
                    fault()
            pb.CANCEL_EVENT.clear()
            write_bytes = Path.write_bytes
            def preserve(path, data):
                if preservation_failure and path.name == "failed-target.o":
                    raise OSError("synthetic evidence write failure")
                return write_bytes(path, data)
            try:
                with mock.patch.object(pb, "BASEROM", rom), \
                     mock.patch.object(pb, "find_asm_target", return_value=None), \
                     mock.patch.object(pb.reloc_surface, "permuter_annotation", return_value=(
                         "annotated target\n", {"old": "__ovval_00000000"}, [])), \
                     mock.patch.object(pb, "bounded_capture", side_effect=capture), \
                     mock.patch.object(pb, "validate_annotation_target", side_effect=proof), \
                     mock.patch.object(Path, "write_bytes", preserve):
                    if preservation_failure:
                        with self.assertRaisesRegex(RuntimeError, "annotation recovery needs review"):
                            pb.annotate_overlay_scratch(SimpleNamespace(overlay=1, func="fixture"), scratch, output)
                    elif kind in {"timeout", "cancel", "interrupt"}:
                        with self.assertRaises(type(error)) as caught:
                            pb.annotate_overlay_scratch(SimpleNamespace(overlay=1, func="fixture"), scratch, output,
                                                        time.monotonic() + 60)
                        self.assertIs(caught.exception, error)
                    else:
                        self.assertEqual(pb.annotate_overlay_scratch(
                            SimpleNamespace(overlay=1, func="fixture"), scratch, output, time.monotonic() + 60), 0)
            finally:
                pb.CANCEL_EVENT.clear()
            for name, data in before.items():
                self.assertEqual((scratch / name).read_bytes(), data)
                self.assertEqual((scratch / name).stat().st_mode & 0o777, 0o750 if name == "compile.sh" else 0o640)
            if missing_target:
                self.assertFalse((scratch / "target.o").exists())
            self.assertEqual(len(calls), 2 if phase == "refresh" else 1)
            self.assertIn(str(error), (output / "annotation.txt").read_text())
            attempt, = output.glob("annotation-attempt-*")
            self.assertEqual((attempt / "before-annotation.txt").read_text(), "prior diagnostic\n")
            if not preservation_failure:
                self.assertEqual((attempt / "failed-target.o").read_bytes(), b"failed attempt artifact")
            if kind == "timeout":
                self.assertIn("partial sentinel", (attempt / "failure.txt").read_text())

    def test_every_mutation_phase_restores_bytes_modes_and_exception(self):
        for phase in ("assembly", "proof", "refresh"):
            for kind in ("nonzero", "timeout", "cancel", "interrupt"):
                with self.subTest(phase=phase, kind=kind):
                    self.exercise(phase, kind)

    def test_failed_annotation_restores_initial_absence(self):
        self.exercise("proof", "nonzero", missing_target=True)

    def test_evidence_write_failure_does_not_prevent_restoration(self):
        self.exercise("refresh", "nonzero", preservation_failure=True)


if __name__ == "__main__":
    unittest.main()
