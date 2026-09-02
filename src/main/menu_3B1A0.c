#include "PR/ultratypes.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A5A0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A680.s")
s32 func_8003A6B0(u8 arg0) {
    s32 temp_t6 = arg0;
    s32 temp_v1 = temp_t6;

    if (temp_t6 == 0x21) {
        goto return_1B;
    }
    if (temp_t6 == 0x2A) {
        goto return_1C;
    }
    if (temp_t6 != 0x3F) {
        goto return_default;
    }
    return 0x1A;

return_1B:
    return 0x1B;
return_1C:
    return 0x1C;
return_default:
    return (temp_v1 - 0x41) & 0xFF;
}
s32 func_8003A700(u8 arg0) {
    s32 temp_t6 = arg0;
    s32 temp_v1 = temp_t6;

    switch (temp_t6) {
    case 0x1A:
        return 0x3F;
    case 0x1B:
        return 0x21;
    case 0x1C:
        return 0x2A;
    default:
        return (temp_v1 + 0x41) & 0xFF;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A754.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A7D0.s")
