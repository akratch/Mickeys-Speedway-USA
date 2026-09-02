#include "PR/ultratypes.h"
#include "PR/os_gbpak.h"

extern s32 func_8006A5A0(OSMesgQueue *queue, OSPfs *pfs, s32 channel);
extern s32 func_8006AC60(OSPfs *pfs, s32 flag);
extern s32 func_8006B020(OSPfs *pfs, u16 flag, u16 address, u8 *buffer, u16 size);
extern OSMesgQueue *joyMessageQ(void);
extern OSPfs D_800D7830;
extern OSGbpakId D_800D77E0;
extern OSGbpakId *D_8007F7A0;
extern s8 D_8007F7A4;
extern s32 D_8007A1CC;
extern s32 D_800D789C;
extern s32 D_800D78A4;
extern s32 D_800D78AC;
extern s32 D_800D7898;
extern u8 D_800D789A[];
extern u8 *D_800D78A0[];
extern s32 D_800D78A8[];

void func_80058010(void) {
    u8 status[5];

    if (func_8006A5A0(joyMessageQ(), &D_800D7830, 0) == 0) {
        if (osGbpakCheckConnector(&D_800D7830, &status[3]) == 0) {
            D_8007F7A4 = 1;
            return;
        }
        func_8006AC60(&D_800D7830, 0);
    }
}
#ifdef NON_MATCHING
void func_8005807C(void) {
    u8 status;

    if (D_8007F7A4 != 0) {
        if (osGbpakReadId(&D_800D7830, &D_800D77E0, &status) == 0) {
            D_8007F7A0 = &D_800D77E0;
        } else {
            D_8007F7A0 = NULL;
        }
        func_8006AC60(&D_800D7830, 0);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58C10/func_8005807C.s")
#endif
#ifdef NON_MATCHING
void func_800580F0(s32 arg0) {
    u8 status;

    if (D_8007F7A4 != 0) {
        osGbpakGetStatus(&D_800D7830, &status);
        if (func_8006AC60(&D_800D7830, 1) == 0) {
            osGbpakCheckConnector(&D_800D7830, &status);
            func_8006B020(&D_800D7830, (u16)arg0,
                          *(u16 *)(&D_800D789A[arg0 * 4]),
                          D_800D78A0[arg0], D_800D78A8[arg0]);
            func_8006AC60(&D_800D7830, 0);
            if (osGbpakGetStatus(&D_800D7830, &status) != 0) {
                    func_80058010();
                }
            }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58C10/func_800580F0.s")
#endif
void func_800581BC(void) {
    D_8007A1CC |= 0x04000000;
}
void func_800581D8(s32 arg0, s32 arg1, s32 arg2) {
    D_8007A1CC |= 0x02000000;
    D_800D789C = arg0;
    D_800D78A4 = arg1;
    D_800D78AC = arg2;
}
void func_8005820C(s32 arg0, s32 arg1, s32 arg2) {
    D_8007A1CC |= 0x01000000;
    D_800D7898 = arg0;
    D_800D78A0[0] = (u8 *)arg1;
    D_800D78A8[0] = arg2;
}

OSGbpakId *func_80058240(void) {
    return D_8007F7A0;
}

/* PLATEAU-HANDOFF:func_8005807C:start
 * symbol: func_8005807C
 * score: 27/29 words
 * frame: 0x20
 * relocations: 16
 * first-mismatch: +0x6C
 * summary: C body is exact through 0x6C; target carries an unreferenced 8-byte duplicate-return tail that normal -O2 IDO does not emit.
 * PLATEAU-HANDOFF:func_8005807C:end
 */

/* PLATEAU-HANDOFF:func_800580F0:start
 * symbol: func_800580F0
 * score: 49/51 words
 * frame: 0x30
 * relocations: 17
 * first-mismatch: +0xC
 * summary: IDO -O2 hoists D_800D7830 into s0; target reloads it at each call, so the frame and relocation surface differ.
 * PLATEAU-HANDOFF:func_800580F0:end
 */
