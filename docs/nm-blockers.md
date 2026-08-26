# NON_MATCHING structural-blocker audit

This is a names-only audit of every function body guarded by `NON_MATCHING` in `src/`, reconciled with the plateau notes in `docs/resident.md` and `docs/overlays.md`. Declaration-only guards are excluded. `none` means the recorded blocker is code shape, frame, scheduling, register allocation, ABI typing, or unresolved reconstruction rather than section/TU ownership.

- Guarded functions audited: 418
- Structural blockers found: 39
- Structural blockers resolved: 2
- Structural blockers remaining: 37
- No structural blocker: 379

| Function | TU | Class | Symbols involved | One-line fix idea |
|---|---|---|---|---|
| `func_8005055C` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800508D4` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80050BF4` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80050DF0` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80050E9C` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800511C4` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80051364` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80053550` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80055970` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80055B24` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80055F64` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80056DD8` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8005716C` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800573C8` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8005776C` | `src/main/anim.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800033B0` | `src/main/audio_manager_36D0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80022FD4` | `src/main/camera.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80023598` | `src/main/camera.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80024834` | `src/main/camera.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80024978` | `src/main/camera.c` | data-aggregate | D_80079F48, D_80079F4C | Move the scale/matrix table definition into camera as one owner. |
| `func_8001C114` | `src/main/charControl.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8001D2A0` | `src/main/charControl.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8001D880` | `src/main/charControl.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8001DCD0` | `src/main/charControl.c` | none | — | No ownership change; continue source/codegen work. |
| `diCpuThread` | `src/main/diCpu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80045BBC` | `src/main/diCpu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80045D34` | `src/main/diCpu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80046AA8` | `src/main/diCpu.c` | none | — | No ownership change; continue source/codegen work. |
| `diRcpPrintDL` | `src/main/diRcp.c` | shared-rodata (excluded) | diRcp switch tables | Owned by the active diRcp lane; do not touch here. |
| `diRcpMoveWd` | `src/main/diRcp.c` | shared-rodata (excluded) | jtbl_80083950 | Owned by the active diRcp lane; do not touch here. |
| `func_80044C94` | `src/main/diRcpTrace.c` | none | — | No ownership change; continue source/codegen work. |
| `vsprintf` | `src/main/diprint.c` | asm-data | formatter tables | Move the formatter tables/data from the asm split into diprint. |
| `diPrintfAll` | `src/main/diprint.c` | bss-aggregate | D_800D4A60, D_800D4A62 | Own the adjacent text coordinates as one diprint BSS object. |
| `debug_text_width` | `src/main/diprint.c` | none | — | No ownership change; continue source/codegen work. |
| `debug_text_parse` | `src/main/diprint.c` | mixed: bss+rodata | D_800D4A60, jtbl_80082CD8 | Move both the coordinate aggregate and switch table into diprint. |
| `func_8004B1DC` | `src/main/font.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004BA8C` | `src/main/font.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004BCC4` | `src/main/font.c` | bss-aggregate | D_800D60E8, D_800D64E8 | Restore the font/dialogue BSS bases and addends in one owner. |
| `func_8004C690` | `src/main/font.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004D40C` | `src/main/font.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800475E8` | `src/main/fx.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80047CD8` | `src/main/fx.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004A380` | `src/main/fx.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004ACC4` | `src/main/fx.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004AF68` | `src/main/fx.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800336A8` | `src/main/gameVi.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800339B4` | `src/main/gameVi.c` | static-placement | framebuffer base symbol | Place the framebuffer anchor where gameVi can retain target relocations. |
| `func_80034094` | `src/main/gameVi.c` | shared-rodata/blocked | jtbl_8008249C, D_80082490..D_80082498 | A table-only carve shifts four bytes; the full island needs named-float ownership. |
| `func_8005BA40` | `src/main/gsSnd.c` | shared-rodata | gsSnd event switch tables | Move the measured gsSnd rodata range into the gsSnd TU. |
| `joyInit` | `src/main/joy.c` | bss-aggregate | D_800CF3B4..D_800CF3B7 | Own the adjacent controller-state BSS in joy. |
| `joyRead` | `src/main/joy.c` | bss-aggregate | D_800CF370..D_800CF3BC | Own the controller arrays and counters as one joy BSS layout. |
| `joyResetMap` | `src/main/joy.c` | resolved-plateau | D_800CF3B0 | Controller map is TU-owned; expression-tree mismatch remains. |
| `levelGetCounts` | `src/main/level.c` | resolved-plateau | D_800CF3E0, D_800CF420 | Level BSS ranges are TU-owned; three register words and one end-label relocation remain. |
| `levelInit` | `src/main/level.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80019934` | `src/main/lights.c` | shared-rodata | jtbl_800817B4 | Move the measured switch table and labels into the lights TU. |
| `func_80019DE8` | `src/main/lights.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8001A154` | `src/main/lights.c` | none | — | No ownership change; continue source/codegen work. |
| `mainThread` | `src/main/main.c` | static-placement | RAM-end anchor | Place the RAM-end anchor in main without losing its relocation identity. |
| `func_80026FB4` | `src/main/main.c` | none | — | No ownership change; continue source/codegen work. |
| `mainUpdateZBCheck` | `src/main/main.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80028564` | `src/main/main.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80028FCC` | `src/main/main.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80029274` | `src/main/main.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002AF6C` | `src/main/matrix.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002B040` | `src/main/matrix.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002B524` | `src/main/memory.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002B7AC` | `src/main/memory.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002BB40` | `src/main/memory.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80038750` | `src/main/menu.c` | shared-rodata | jtbl_80082734 | Move the language switch table and labels into menu. |
| `func_80038878` | `src/main/menu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800389CC` | `src/main/menu.c` | shared-rodata | jtbl_80082748 | Move the nineteen-mode switch table and labels into menu. |
| `func_80038BC4` | `src/main/menu.c` | shared-rodata | jtbl_80082794 | Move the nineteen-mode switch table and labels into menu. |
| `func_80038E1C` | `src/main/menu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003968C` | `src/main/menu.c` | bss-aggregate/blocked | D_800D3198 repeat state | A menu BSS carve pools the stores and changes an exact consumer. |
| `func_80039E34` | `src/main/menu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003A2C8` | `src/main/menu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003A520` | `src/main/menu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8001FC50` | `src/main/models.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002057C` | `src/main/models.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80020B10` | `src/main/models.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80020D8C` | `src/main/models.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80020E4C` | `src/main/models.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8005A7A0` | `src/main/models_5B300.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8005A948` | `src/main/models_5B300.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8005AAC0` | `src/main/models_5B300.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003CE10` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003D25C` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003E8D8` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003EC8C` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `partUpdateTriggers` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003F154` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8003F5F8` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8004054C` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80040740` | `src/main/particles.c` | shared-rodata | particle-type switch table | Move the switch table and local labels into particles. |
| `func_80040B88` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80041530` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80041CE4` | `src/main/particles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002EBE0` | `src/main/rcpFast3d.c` | none | — | No ownership change; continue source/codegen work. |
| `rcpClearZBuffer` | `src/main/rcpFast3d.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80031A30` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `runlinkDownloadCode` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `func_800320F0` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `runlinkFreeCode` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `runlinkInit` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `runlinkResumeCode` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `runlinkGetAddressInfo` | `src/main/runlink.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002C69C` | `src/main/saves.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002C70C` | `src/main/saves.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002C94C` | `src/main/saves.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002CB18` | `src/main/saves.c` | none | — | No ownership change; continue source/codegen work. |
| `func_8002CF6C` | `src/main/saves.c` | none | — | No ownership change; continue source/codegen work. |
| `packInit` | `src/main/saves.c` | none | — | No ownership change; continue source/codegen work. |
| `osScGetTaskType` | `src/main/sched.c` | shared-rodata (excluded) | jtbl_800823D8 | Owned by the active sched lane; do not touch here. |
| `func_80030610` | `src/main/sched.c` | none | — | No ownership change; continue source/codegen work. |
| `func_80030910` | `src/main/sched.c` | none | — | No ownership change; continue source/codegen work. |
| `__scHandleRetrace` | `src/main/sched.c` | none | — | No ownership change; continue source/codegen work. |
| `__scYield` | `src/main/sched.c` | bss-aggregate (excluded) | scheduler timestamp words | Owned by the active sched lane; do not touch here. |
| `__scSchedule` | `src/main/sched.c` | shared-rodata (excluded) | jtbl_800823F4 | Owned by the active sched lane; do not touch here. |
| `shadowInitBuffers` | `src/main/shadows.c` | data-aggregate | D_80079434, D_80079440 | Give shadows the original contiguous initialized-data owner. |
| `func_80058250` | `src/main/vehicle_sounds.c` | bss-aggregate (excluded) | vehicle sound slots | Owned by the active vehicle_sounds lane; do not touch here. |
| `func_8005830C` | `src/main/vehicle_sounds.c` | none | — | No ownership change; continue source/codegen work. |
| `doWeather` | `src/main/weather.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00001AC_184C58C` | `src/overlays/o001/overlay_001.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0000378_184C758` | `src/overlays/o001/overlay_001.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0000414_184C7F4` | `src/overlays/o001/overlay_001.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00004B4_184C894` | `src/overlays/o001/overlay_001.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0000614_184C9F4` | `src/overlays/o001/overlay_001.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0007D6C_185414C` | `src/overlays/o001/overlay_001_end.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0000CA8_184D088` | `src/overlays/o001/overlay_001_head.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0000DF4_184D1D4` | `src/overlays/o001/overlay_001_head.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0000F84_184D364` | `src/overlays/o001/overlay_001_head.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00010C8_184D4A8` | `src/overlays/o001/overlay_001_head.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0001A54_184DE34` | `src/overlays/o001/overlay_001_head.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0002744_184EB24` | `src/overlays/o001/overlay_001_middle.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F000280C_184EBEC` | `src/overlays/o001/overlay_001_middle.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0002AA4_184EE84` | `src/overlays/o001/overlay_001_middle.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0003578_184F958` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00036A0_184FA80` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0003750_184FB30` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0003FD8_18503B8` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F000438C_185076C` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0005BF4_1851FD4` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0005ED4_18522B4` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00061F0_18525D0` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0006270_1852650` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00064F8_18528D8` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00067C0_1852BA0` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0006A14_1852DF4` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0006D4C_185312C` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00072A4_1853684` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0007344_1853724` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00073A0_1853780` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0007580_1853960` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0007730_1853B10` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F00078DC_1853CBC` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_001_F0007B64_1853F44` | `src/overlays/o001/overlay_001_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F0000C90_1857A88` | `src/overlays/o002/func_overlay_002_F0000C90_1857A88.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F0001364_185815C` | `src/overlays/o002/func_overlay_002_F0001364_185815C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F0001A94_185888C` | `src/overlays/o002/func_overlay_002_F0001A94_185888C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F0001DF8_1858BF0` | `src/overlays/o002/func_overlay_002_F0001DF8_1858BF0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F00006E0_18574D8` | `src/overlays/o002/overlay2ChooseBoundary.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F00002C4_18570BC` | `src/overlays/o002/overlay2ClassifyBoundary.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F00016A0_1858498` | `src/overlays/o002/overlay2QueryNode.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_002_F0000B70_1857968` | `src/overlays/o002/overlay2SplitRegion.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_003_F000027C_1859FAC` | `src/overlays/o003/overlay3FindClosestObject.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_003_F00000B8_1859DE8` | `src/overlays/o003/overlay3RunCachedModeAction.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_003_F00003B0_185A0E0` | `src/overlays/o003/overlay3SelectScoredObject.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_004_F0000138_185A7B0` | `src/overlays/o004/overlay_004.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_005_F000031C_185B744` | `src/overlays/o005/overlay_005.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_007_F0000324_185C1AC` | `src/overlays/o007/func_overlay_007_F0000324_185C1AC.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_007_F00000A8_185BF30` | `src/overlays/o007/overlay_007.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_007_F0000894_185C71C` | `src/overlays/o007/overlay_007_tail.c` | asm-data | overlay 7 mode switch table | Move the initialized switch table into the consolidated overlay 7 TU. |
| `func_overlay_007_F0000AA0_185C928` | `src/overlays/o007/overlay_007_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_007_F0000CCC_185CB54` | `src/overlays/o007/overlay_007_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_007_F0000DBC_185CC44` | `src/overlays/o007/overlay_007_tail.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F0000058_185DDB0` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F0000894_185E5EC` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F0001000_185ED58` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F0002640_1860398` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F000291C_1860674` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F0003368_18610C0` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F00034A0_18611F8` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F00042A8_1862000` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_008_F0004CF0_1862A48` | `src/overlays/o008/overlay_008.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_009_F0000000_1866678` | `src/overlays/o009/overlay_009.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_009_F0000540_1866BB8` | `src/overlays/o009/overlay_009.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_009_F0000CE4_186735C` | `src/overlays/o009/overlay_009.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_009_F00010B4_186772C` | `src/overlays/o009/overlay_009.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_010_F0000000_1868450` | `src/overlays/o010/overlay10Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_011_F0000150_1868998` | `src/overlays/o011/func_overlay_011_F0000150_1868998.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_011_F0001E4C_186A694` | `src/overlays/o011/func_overlay_011_F0001E4C_186A694.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_011_F00022E8_186AB30` | `src/overlays/o011/func_overlay_011_F00022E8_186AB30.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_011_F0001398_1869BE0` | `src/overlays/o011/overlay11UpdateMenu.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_012_F00003A8_186D628` | `src/overlays/o012/func_overlay_012_F00003A8_186D628.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_012_F0000910_186DB90` | `src/overlays/o012/func_overlay_012_F0000910_186DB90.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_013_F0000874_186F38C` | `src/overlays/o013/overlay13DrawActive.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_013_F0000580_186F098` | `src/overlays/o013/overlay13DrawRecord.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_013_F0000284_186ED9C` | `src/overlays/o013/overlay13ProcessRecord.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F0000000_186F8D8` | `src/overlays/o014/func_overlay_014_F0000000_186F8D8.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F000013C_186FA14` | `src/overlays/o014/func_overlay_014_F000013C_186FA14.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F00009F4_18702CC` | `src/overlays/o014/func_overlay_014_F00009F4_18702CC.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F0001540_1870E18` | `src/overlays/o014/func_overlay_014_F0001540_1870E18.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F0001830_1871108` | `src/overlays/o014/func_overlay_014_F0001830_1871108.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F00006FC_186FFD4` | `src/overlays/o014/overlay14CreateValue.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F000087C_1870154` | `src/overlays/o014/overlay14LoadRelocatedValue.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F0000B5C_1870434` | `src/overlays/o014/overlay14PrepareInputState.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_014_F0000498_186FD70` | `src/overlays/o014/overlay14ResetMode.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_015_F000004C_18723E4` | `src/overlays/o015/overlay_015.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_015_F0000428_18727C0` | `src/overlays/o015/overlay_015.c` | bss-aggregate | overlay 15 star state/bounds | Own the star-state and bounds BSS as one overlay 15 object. |
| `func_overlay_015_F0000500_1872898` | `src/overlays/o015/overlay_015.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_015_F00006E8_1872A80` | `src/overlays/o015/overlay_015.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_015_F00009E0_1872D78` | `src/overlays/o015/overlay_015.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_015_F0000B94_1872F2C` | `src/overlays/o015/overlay_015.c` | bss-aggregate | overlay 15 rain state | Own the rain offsets and pointers as one overlay 15 BSS object. |
| `func_overlay_016_F00001E0_1873678` | `src/overlays/o016/overlay_016.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_017_F0000668_1874020` | `src/overlays/o017/overlay17AdvanceChain.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_017_F0000000_18739B8` | `src/overlays/o017/overlay17CalculateEndpoints.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_017_F0000318_1873CD0` | `src/overlays/o017/overlay17CreateChain.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_017_F00008B4_187426C` | `src/overlays/o017/overlay17DrawStrip.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_018_F0000000_18745B8` | `src/overlays/o018/overlay18Load.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_019_F0000A30_1875C88` | `src/overlays/o019/overlay19BuildAdjacency.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_019_F00001E0_1875438` | `src/overlays/o019/overlay19BuildPlanes.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_019_F0000F58_18761B0` | `src/overlays/o019/overlay19BuildSpatialMasks.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_019_F0000D78_1875FD0` | `src/overlays/o019/overlay19ClassifyEdge.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_020_F000038C_1876964` | `src/overlays/o020/func_overlay_020_F000038C_1876964.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_020_F0001148_1877720` | `src/overlays/o020/func_overlay_020_F0001148_1877720.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_020_F00007C4_1876D9C` | `src/overlays/o020/overlay20BuildTileCommands.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_020_F0001018_18775F0` | `src/overlays/o020/overlay20RemoveEntry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_020_F0000A68_1877040` | `src/overlays/o020/overlay20UpdateGrid.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_020_F0000204_18767DC` | `src/overlays/o020/overlay20UpdateObjectResource.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_021_F000010C_1877D94` | `src/overlays/o021/overlay21ApplyPriorities.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_022_F00002B0_18783B8` | `src/overlays/o022/func_overlay_022_F00002B0_18783B8.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_022_F0000000_1878108` | `src/overlays/o022/overlay22InitializeObject.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_022_F0000D30_1878E38` | `src/overlays/o022/overlay22RemoveObject.c` | tu-boundary | overlay 22 local globals | Consolidate the function with its same-overlay data owner. |
| `func_overlay_022_F0000A7C_1878B84` | `src/overlays/o022/overlay22ResolvePlane.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_025_F0000000_1879C88` | `src/overlays/o025/overlay_025.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_025_F000017C_1879E04` | `src/overlays/o025/overlay_025.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_026_F00001A0_187A598` | `src/overlays/o026/func_overlay_026_F00001A0_187A598.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_026_F0000B18_187AF10` | `src/overlays/o026/func_overlay_026_F0000B18_187AF10.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_026_F0001158_187B550` | `src/overlays/o026/overlay26DrawGroups.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_026_F0000D24_187B11C` | `src/overlays/o026/overlay26HandleEffects.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_027_F0000064_187BA3C` | `src/overlays/o027/overlay_027.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_027_F0000624_187BFFC` | `src/overlays/o027/overlay_027.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_027_F0000A1C_187C3F4` | `src/overlays/o027/overlay_027.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_028_F00004D8_187CDA8` | `src/overlays/o028/overlay_028.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_029_F00005C4_187D874` | `src/overlays/o029/func_overlay_029_F00005C4_187D874.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_029_F00014C8_187E778` | `src/overlays/o029/overlay29DrawGroups.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_029_F00010C4_187E374` | `src/overlays/o029/overlay29HandleEffects.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_029_F000042C_187D6DC` | `src/overlays/o029/overlay29InitializeObject.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_029_F0000EE0_187E190` | `src/overlays/o029/overlay29ProjectPoint.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_030_F00002B4_187F1AC` | `src/overlays/o030/overlay30TransposePixels.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_031_F0000000_187F520` | `src/overlays/o031/overlay31BuildLookupTables.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_031_F0000A84_187FFA4` | `src/overlays/o031/overlay31CreateConfig.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_031_F0000E7C_188039C` | `src/overlays/o031/overlay31CreatePool.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_031_F00006B0_187FBD0` | `src/overlays/o031/overlay31InitializeBuffers.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_031_F00002E8_187F808` | `src/overlays/o031/overlay31InitializeParticleAssets.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_033_F000019C_1880984` | `src/overlays/o033/overlay33BuildDisplayList.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_033_F0000000_18807E8` | `src/overlays/o033/overlay33InitializeBuffers.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_033_F000066C_1880E54` | `src/overlays/o033/overlay33PresentAndSwap.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_034_F00000D4_188127C` | `src/overlays/o034/overlay34CreateRecord.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_034_F0000000_18811A8` | `src/overlays/o034/overlay34InitStorage.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_034_F00002C8_1881470` | `src/overlays/o034/overlay34RemoveRecord.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_034_F0000608_18817B0` | `src/overlays/o034/overlay34SortAndDraw.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_035_F00001E0_1881EC0` | `src/overlays/o035/func_overlay_035_F00001E0_1881EC0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_035_F0000B40_1882820` | `src/overlays/o035/func_overlay_035_F0000B40_1882820.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_035_F0000770_1882450` | `src/overlays/o035/overlay35BuildGridMasks.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_035_F0000000_1881CE0` | `src/overlays/o035/overlay35Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_036_F0000818_1883CD0` | `src/overlays/o036/overlay36CheckNearbyHeight.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_036_F0000A60_1883F18` | `src/overlays/o036/overlay36ChooseWeightedState.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_036_F00001D0_1883688` | `src/overlays/o036/overlay36UpdateInteractiveEntity.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_037_F000019C_18857BC` | `src/overlays/o037/overlay37Render.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_038_F0000000_1885D10` | `src/overlays/o038/func_overlay_038_F0000000_1885D10.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_038_F000047C_188618C` | `src/overlays/o038/func_overlay_038_F000047C_188618C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_038_F0000154_1885E64` | `src/overlays/o038/overlay38UpdateParticles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_040_F0000000_18868B0` | `src/overlays/o040/overlay40AddEntry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_040_F00001A0_1886A50` | `src/overlays/o040/overlay40BuildFrame.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_040_F0000690_1886F40` | `src/overlays/o040/overlay40FadeRecords.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_040_F00000E8_1886998` | `src/overlays/o040/overlay40UpdateEntries.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F0001650_1888988` | `src/overlays/o041/overlay41AddSlot.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F0000000_1887338` | `src/overlays/o041/overlay41AdvanceStepRecords.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F000195C_1888C94` | `src/overlays/o041/overlay41EnqueueTransition.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F0001464_188879C` | `src/overlays/o041/overlay41ProcessEntry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F00002AC_18875E4` | `src/overlays/o041/overlay41SampleCurve.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F0000124_188745C` | `src/overlays/o041/overlay41UpdateColorRecords.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F0000854_1887B8C` | `src/overlays/o041/overlay41UpdateCurveObject.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_041_F0001298_18885D0` | `src/overlays/o041/overlay41UpdateProgress.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_043_F0000324_188A2F4` | `src/overlays/o043/func_overlay_043_F0000324_188A2F4.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_043_F0000BE4_188ABB4` | `src/overlays/o043/func_overlay_043_F0000BE4_188ABB4.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_043_F00010A8_188B078` | `src/overlays/o043/overlay43ComputeMotion.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_043_F0001378_188B348` | `src/overlays/o043/overlay43FilterImage.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_043_F0000000_1889FD0` | `src/overlays/o043/overlay43InitializeState.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_043_F0001264_188B234` | `src/overlays/o043/overlay43SubmitChildren.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_044_F0000580_188BDE0` | `src/overlays/o044/func_overlay_044_F0000580_188BDE0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_044_F0000294_188BAF4` | `src/overlays/o044/overlay44UpdateFrameCache.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_045_F0000764_188CBBC` | `src/overlays/o045/func_overlay_045_F0000764_188CBBC.c` | tu-boundary | overlay 45 constants/locals | Consolidate the function with its constants and local references. |
| `func_overlay_045_F0001158_188D5B0` | `src/overlays/o045/func_overlay_045_F0001158_188D5B0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_046_F0000874_188EC6C` | `src/overlays/o046/func_overlay_046_F0000874_188EC6C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_046_F0001228_188F620` | `src/overlays/o046/func_overlay_046_F0001228_188F620.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_046_F0000120_188E518` | `src/overlays/o046/overlay46UpdateSequence.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_047_F0000000_1890E18` | `src/overlays/o047/func_overlay_047_F0000000_1890E18.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_047_F0000B30_1891948` | `src/overlays/o047/func_overlay_047_F0000B30_1891948.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_047_F00009D0_18917E8` | `src/overlays/o047/overlay47ReleaseResources.c` | data-aggregate | overlay 47 entry/resource block | Own the resource arrays as one overlay 47 initialized-data block. |
| `func_overlay_048_F0000060_1895468` | `src/overlays/o048/overlay48InitializeState.c` | bss-aggregate | overlay 48 seed/state block | Own the seed and indexed tail under the original BSS base. |
| `func_overlay_048_F0000144_189554C` | `src/overlays/o048/overlay48UpdateState.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_049_F0000000_1896410` | `src/overlays/o049/overlay_049.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_050_F0000334_1896CA4` | `src/overlays/o050/func_overlay_050_F0000334_1896CA4.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_050_F0000000_1896970` | `src/overlays/o050/overlay50Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_052_F0000000_189A670` | `src/overlays/o052/overlay52Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_053_F0000240_189DBE8` | `src/overlays/o053/func_overlay_053_F0000240_189DBE8.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_054_F0000000_189ECA0` | `src/overlays/o054/overlay54Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_055_F000031C_18A1E34` | `src/overlays/o055/func_overlay_055_F000031C_18A1E34.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0000000_18A3BF8` | `src/overlays/o057/func_overlay_057_F0000000_18A3BF8.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0001020_18A4C18` | `src/overlays/o057/func_overlay_057_F0001020_18A4C18.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0004460_18A8058` | `src/overlays/o057/func_overlay_057_F0004460_18A8058.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F00060F8_18A9CF0` | `src/overlays/o057/func_overlay_057_F00060F8_18A9CF0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F00032A0_18A6E98` | `src/overlays/o057/overlay57Draw32A0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F00028B4_18A64AC` | `src/overlays/o057/overlay57EaseAndLatch.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0004064_18A7C5C` | `src/overlays/o057/overlay57HandleModeInput.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0002C28_18A6820` | `src/overlays/o057/overlay57SmoothAndCheckDistance.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0003A4C_18A7644` | `src/overlays/o057/overlay57UpdateModeState.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_057_F0004C18_18A8810` | `src/overlays/o057/overlay57UpdateModeTrigger.c` | tu-boundary | overlay 57 local bases | Consolidate the updater with its local data bases. |
| `func_overlay_057_F00035E0_18A71D8` | `src/overlays/o057/overlay57UpdateSelection.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F0000000_18AF1E8` | `src/overlays/o058/func_overlay_058_F0000000_18AF1E8.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F00005FC_18AF7E4` | `src/overlays/o058/func_overlay_058_F00005FC_18AF7E4.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F000138C_18B0574` | `src/overlays/o058/func_overlay_058_F000138C_18B0574.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F00050C8_18B42B0` | `src/overlays/o058/overlay58DrawLargePointQuad.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F0004F28_18B4110` | `src/overlays/o058/overlay58DrawPointQuad.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F0004C04_18B3DEC` | `src/overlays/o058/overlay58DrawSegmentStrip.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_058_F0005554_18B473C` | `src/overlays/o058/overlay58FinalizePackedStatus.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_059_F000036C_18B8ABC` | `src/overlays/o059/overlay59Advance.c` | shared-rodata | overlay 59 state switch table | Move the compiler switch table into the overlay 59 data owner. |
| `func_overlay_059_F0000070_18B87C0` | `src/overlays/o059/overlay59PrepareEntry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_060_F0002F54_18BCD2C` | `src/overlays/o060/func_overlay_060_F0002F54_18BCD2C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_060_F0000000_18B9DD8` | `src/overlays/o060/overlay60Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_060_F0003488_18BD260` | `src/overlays/o060/overlay60ReassignChoiceSlots.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_061_F0001648_18C0A10` | `src/overlays/o061/func_overlay_061_F0001648_18C0A10.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_061_F00001DC_18BF5A4` | `src/overlays/o061/overlay61AddEntry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_061_F00007C4_18BFB8C` | `src/overlays/o061/overlay61DrawList.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_061_F00017B8_18C0B80` | `src/overlays/o061/overlay61WriteCharacter.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_062_F00000D4_18C22F4` | `src/overlays/o062/overlay62Update.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_063_F0000000_18C2B88` | `src/overlays/o063/overlay63Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_063_F00001D4_18C2D5C` | `src/overlays/o063/overlay63UpdateEffects.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_063_F000077C_18C3304` | `src/overlays/o063/overlay63UpdateSequence.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_064_F0000000_18C3B28` | `src/overlays/o064/overlay64GenerateTexture.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_065_F0000C38_18C4EA0` | `src/overlays/o065/func_overlay_065_F0000C38_18C4EA0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_065_F0000080_18C42E8` | `src/overlays/o065/overlay65UpdateParticles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_066_F00004E0_18C6948` | `src/overlays/o066/func_overlay_066_F00004E0_18C6948.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_066_F0000040_18C64A8` | `src/overlays/o066/overlay66SmoothAndDraw.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_068_F000146C_18C85CC` | `src/overlays/o068/overlay68CheckKind.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_068_F0000EFC_18C805C` | `src/overlays/o068/overlay68DrawSortedEntries.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_068_F000051C_18C767C` | `src/overlays/o068/overlay68PromoteSecondary.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_068_F0001250_18C83B0` | `src/overlays/o068/overlay68RebuildSecondaryEntry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_068_F000096C_18C7ACC` | `src/overlays/o068/overlay68UpdateAnimation.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_070_F00000D8_18C92A0` | `src/overlays/o070/func_overlay_070_F00000D8_18C92A0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_070_F0000384_18C954C` | `src/overlays/o070/func_overlay_070_F0000384_18C954C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_071_F0000278_18C9D98` | `src/overlays/o071/func_overlay_071_F0000278_18C9D98.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_071_F0000870_18CA390` | `src/overlays/o071/func_overlay_071_F0000870_18CA390.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_073_F0000D70_18CB830` | `src/overlays/o073/overlay73Draw.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_073_F0000000_18CAAC0` | `src/overlays/o073/overlay73Initialize.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_074_F00000B8_18CBD58` | `src/overlays/o074/overlay74Update.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_075_F0000214_18CC17C` | `src/overlays/o075/overlay75UpdateMovingObject.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_079_F0000134_18CD0D4` | `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c` | data-aggregate | overlay 79 shared data base | Put the state data under one overlay-local initialized base. |
| `func_overlay_079_F0000FA0_18CDF40` | `src/overlays/o079/func_overlay_079_F0000FA0_18CDF40.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_079_F0001290_18CE230` | `src/overlays/o079/func_overlay_079_F0001290_18CE230.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_080_F0000000_18CE8C8` | `src/overlays/o080/overlay80InitializeContact.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_080_F000011C_18CE9E4` | `src/overlays/o080/overlay80UpdateContact.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_083_F000053C_18CFCFC` | `src/overlays/o083/overlay83BuildBatch.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_083_F0000850_18D0010` | `src/overlays/o083/overlay83DrawStrip.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_084_F0000314_18D07F4` | `src/overlays/o084/func_overlay_084_F0000314_18D07F4.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_084_F0001060_18D1540` | `src/overlays/o084/overlay84ActivateCurrent.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_084_F0000DD0_18D12B0` | `src/overlays/o084/overlay84AdvanceCurrent.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_084_F0000048_18D0528` | `src/overlays/o084/overlay84InitializeAndUpdate.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_084_F0000C9C_18D117C` | `src/overlays/o084/overlay84LoadCurrent.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_086_F0000474_18D22AC` | `src/overlays/o086/func_overlay_086_F0000474_18D22AC.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_086_F0000158_18D1F90` | `src/overlays/o086/overlay86BuildTransform.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_087_F0000128_18D3090` | `src/overlays/o087/func_overlay_087_F0000128_18D3090.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_088_F00001A4_18D3C2C` | `src/overlays/o088/overlay88DrawSortedGeometry.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_089_F0000270_18D44A0` | `src/overlays/o089/overlay89InitializeEffect.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_089_F00005A4_18D47D4` | `src/overlays/o089/overlay89UpdateStateAndParticles.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_090_F00000FC_18D4BF4` | `src/overlays/o090/overlay_090.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_092_F0000308_18D6228` | `src/overlays/o092/func_overlay_092_F0000308_18D6228.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_092_F0000068_18D5F88` | `src/overlays/o092/overlay92FindNearestCourse.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_094_F0000110_18D6CB0` | `src/overlays/o094/overlay94UpdateController.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_096_F0000070_18D76A8` | `src/overlays/o096/overlay96Unregister.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_097_F0000508_18D83A0` | `src/overlays/o097/overlay97InitScale.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_098_F0000000_18D89C0` | `src/overlays/o098/overlay98CollectUniqueY.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_098_F0000234_18D8BF4` | `src/overlays/o098/overlay98RenderReflections.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_099_F00002A0_18D9850` | `src/overlays/o099/overlay99ApplySegment.c` | shared-rodata | overlay 99 private table | Move the private table into the retained overlay 99 data owner. |
| `func_overlay_099_F0000638_18D9BE8` | `src/overlays/o099/overlay99BuildHeightGrid.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_099_F0000064_18D9614` | `src/overlays/o099/overlay99InitializeEntries.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_099_F0000BA4_18DA154` | `src/overlays/o099/overlay99RenderSegments.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_099_F0000800_18D9DB0` | `src/overlays/o099/overlay99RenderSortedEntries.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_100_F0000580_18DB2A8` | `src/overlays/o100/overlay100DrawMotion.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_100_F0000000_18DAD28` | `src/overlays/o100/overlay100InitializeMotion.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_100_F000038C_18DB0B4` | `src/overlays/o100/overlay100UpdateMotion.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F0002510_18DDD30` | `src/overlays/o101/func_overlay_101_F0002510_18DDD30.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F0003A58_18DF278` | `src/overlays/o101/func_overlay_101_F0003A58_18DF278.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000512C_18E094C` | `src/overlays/o101/func_overlay_101_F000512C_18E094C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000571C_18E0F3C` | `src/overlays/o101/func_overlay_101_F000571C_18E0F3C.c` | tu-boundary | overlay 101 local data | Consolidate the builder with its overlay-local data owner. |
| `func_overlay_101_F0005E08_18E1628` | `src/overlays/o101/func_overlay_101_F0005E08_18E1628.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F00063F8_18E1C18` | `src/overlays/o101/func_overlay_101_F00063F8_18E1C18.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F00069E8_18E2208` | `src/overlays/o101/func_overlay_101_F00069E8_18E2208.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F00078F4_18E3114` | `src/overlays/o101/func_overlay_101_F00078F4_18E3114.c` | tu-boundary | overlay 101 local data | Consolidate the builder with its overlay-local data owner. |
| `func_overlay_101_F0008128_18E3948` | `src/overlays/o101/func_overlay_101_F0008128_18E3948.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000895C_18E417C` | `src/overlays/o101/func_overlay_101_F000895C_18E417C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F0009190_18E49B0` | `src/overlays/o101/func_overlay_101_F0009190_18E49B0.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F0002DC0_18DE5E0` | `src/overlays/o101/overlay101BuildBorder.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F00099C4_18E51E4` | `src/overlays/o101/overlay101BuildPresentationA.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F0009D04_18E5524` | `src/overlays/o101/overlay101BuildPresentationB.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000A044_18E5864` | `src/overlays/o101/overlay101BuildPresentationC.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000A384_18E5BA4` | `src/overlays/o101/overlay101BuildPresentationD.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000332C_18DEB4C` | `src/overlays/o101/overlay101DrawClock.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F0002EFC_18DE71C` | `src/overlays/o101/overlay101DrawPanel.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F00029A4_18DE1C4` | `src/overlays/o101/overlay101DrawTransformed.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000A6BC_18E5EDC` | `src/overlays/o101/overlay101TailA6BC.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000AB4C_18E636C` | `src/overlays/o101/overlay101TailAB4C.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000B544_18E6D64` | `src/overlays/o101/overlay101TailB544.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000BA34_18E7254` | `src/overlays/o101/overlay101TailBA34.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000C144_18E7964` | `src/overlays/o101/overlay101TailC144.c` | none | — | No ownership change; continue source/codegen work. |
| `func_overlay_101_F000C6E8_18E7F08` | `src/overlays/o101/overlay101TailC6E8.c` | none | — | No ownership change; continue source/codegen work. |
