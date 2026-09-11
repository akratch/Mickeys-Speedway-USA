typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct O64Image {
    u8 pad00[6];
    u16 width;
    u16 height;
    u8 pad0A[0x16];
    u16 pixels[1];
} O64Image;

extern volatile u8 gO64BufferSelect;
extern s32 gO64Initialized;
extern u8 *gO64BuffersA[];
extern u8 *gO64BuffersB[];
extern s32 o64RandomRange(s32 minimum, s32 maximum);

/* Declaration order set to the target's home ladder 2026-09-11, byte-inert
 * (401 masked, 94 aligned-exact, 190 naming, 23 immediate before and after).
 * Reading both objects' stack slots: the argument build, the `ra` save and the
 * 64-byte declaration block are identical on the two sides, and with this order
 * the six homed locals sit at the target's own offsets from its frame top
 * (-4 width, -12 height, -16 selected, -40 write, -56 x, -64 distance) -- which
 * needs `newSelect`, a u8, declared second so the following word aligns to -12.
 * The remaining 8 bytes are NOT a declaration: the candidate materialises five
 * stack temporaries where the target materialises four, reusing for the later
 * spill the slot its earlier one vacated. Do not shed a declaration to buy
 * them back -- that closes the frame and moves every home off the ladder. */
#ifdef NON_MATCHING
void func_overlay_064_F0000000_18C3B28(s32 index, O64Image *image,
                                        u8 *unused)
{
    register s32 width;
    u8 newSelect;
    register s32 height;
    u8 *selected;
    register u8 *source;
    register u8 *dest;
    register s32 innerWidth;
    register s32 innerHeight;
    register s32 y;
    register u8 *write;
    register s32 maximum;
    register s32 value;
    register u8 *below;
    register s32 x;
    register u16 *pixels;
    register s32 distance;

    (void)unused;

    newSelect = gO64BufferSelect;
    width = image->width;
    height = image->height;
    if (newSelect != 0) {
        selected = gO64BuffersA[index];
    } else {
        selected = gO64BuffersB[index];
    }

    innerWidth = width - 6;
    innerHeight = height - 6;
    write = selected + innerHeight * width + 3;
    if (gO64Initialized == 0) {
        x = innerWidth - 1;
        if (innerWidth != 0) {
            y = width >> 1;
            do {
                distance = y - x - 3;
                if (distance <= 0) {
                    distance = -distance;
                }
                *write++ = o64RandomRange(0, 0xFF - distance * 0x10);
            } while (x--);
        }
        gO64Initialized = 1;
    } else {
        x = innerWidth - 1;
        if (innerWidth != 0) {
            y = width >> 1;
            do {
                distance = y - x - 3;
                if (distance <= 0) {
                    distance = -distance;
                }
                maximum = 0xFF - distance * 0x10;
                value = o64RandomRange(-0x60, 0x60) + *write;
                if (value > maximum) {
                    value = maximum;
                }
                if (value < 0) {
                    value = 0;
                }
                *write++ = value;
            } while (x--);
        }
    }

    newSelect = gO64BufferSelect ^ 1;
    gO64BufferSelect = newSelect;
    if (newSelect != 0) {
        dest = gO64BuffersA[index];
    } else {
        dest = gO64BuffersB[index];
    }

    source = selected;
    write = dest;
    y = height - 7;
    if (innerHeight != 0) {
        do {
            *write++ = 0;
            source++;
            x = width - 3;
            if (width - 2 != 0) {
                below = source + width;
                do {
                    value = below[0] * 4 + source[0] * 2 + below[-1] + below[1];
                    source++;
                    below++;
                    value >>= 3;
                    if (value < 0xAA) {
                        value--;
                    }
                    if (value < 0) {
                        value = 0;
                    }
                    *write++ = value;
                } while (x--);
            }
            *write++ = 0;
            source++;
        } while (y--);
    }

    write += width;
    source += width;
    y = 4;
    do {
        *write++ = 0;
        source++;
        x = width - 3;
        if (width - 2 != 0) {
            below = source - width;
            do {
                value = below[0] + below[-1] + below[1];
                source++;
                below++;
                value >>= 2;
                if (value < 0xAA) {
                    value--;
                }
                if (value < 0) {
                    value = 0;
                }
                *write++ = value;
            } while (x--);
        }
        *write++ = 0;
        source++;
    } while (y--);

    write = dest + innerHeight * width;
    source = write - width;
    x = width - 1;
    if (width != 0) {
        do {
            *write++ = *source++;
        } while (x--);
    }

    source = dest;
    y = height - 1;
    if (height - 7 != -7) {
        pixels = image->pixels;
        do {
            if (y & 1) {
                x = width - 1;
                if (width != 0) {
                    do {
                        value = *source++;
                        distance = value;
                        maximum = value + 0x34;
                        innerHeight = value >> 4;
                        if (innerHeight != 0) {
                            distance = value * innerHeight;
                        }
                        if (maximum >= 0x100) {
                            maximum = 0xFF;
                        }
                        if (distance >= 0x100) {
                            distance = 0xFF;
                        }
                        *pixels++ = (maximum << 8) | distance;
                    } while (x--);
                }
            } else {
                x = 0;
                if (width > 0) {
                    do {
                        innerWidth = (x & 2) ? -2 : 2;
                        value = *source++;
                        distance = value;
                        maximum = value + 0x34;
                        innerHeight = value >> 4;
                        if (innerHeight != 0) {
                            distance = value * innerHeight;
                        }
                        if (maximum >= 0x100) {
                            maximum = 0xFF;
                        }
                        if (distance >= 0x100) {
                            distance = 0xFF;
                        }
                        *(u16 *)((u8 *)pixels + innerWidth * 2 + x * 2) =
                            (maximum << 8) | distance;
                        x++;
                    } while (x != width);
                }
                pixels = (u16 *)((u8 *)pixels + width * 2);
            }
        } while (y--);
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o064/overlay64GenerateTexture/func_overlay_064_F0000000_18C3B28.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_064_F0000000_18C3B28:start
 * symbol: func_overlay_064_F0000000_18C3B28
 * score: 401/420 words
 * frame: 0x78
 * relocations: 18
 * first-mismatch: +0x0
 * summary: Home ladder now matches the target's; the residual 8 bytes is one extra compiler temporary, not a declaration.
 * PLATEAU-HANDOFF:func_overlay_064_F0000000_18C3B28:end
 */
