#include "PR/ultratypes.h"

typedef struct Overlay43Image {
    u8 pad0[0x20];
    u8 *pixels;
} Overlay43Image;

/* PROVENANCE: no code adapted. The pinned DKR/JFG exact-donor scan is negative.
 * A function-specific structural scan found JFG assembly-only
 * func_overlay_4_000015A8_1EF7898 as a 0.574 masked-skeleton relative; it
 * supplies no donor C. */
/* Matched 2026-09-11.  The 16-word residual was the colouring of the row,
 * column, sum and post-decrement scratch webs plus one schedule pair, and
 * one edit closed all of it: the outer and word loops are TOP-tested
 * `while (n--)` with the count one higher, not `do { } while (n--)`.  Only
 * the top-tested form numbers the dead loop-exit copy (the old value of the
 * post-decrement) below the counter (L93), and this function's colouring is
 * the caller-saved sweep in ascending web number, so that one web number
 * moves row to $a1, column to $a2, sum to $a3 and the scratch to $v1 -- and
 * the +0x7C/+0x80 constant pair follows.  The inner loop is inert either way;
 * it is written the same way for symmetry.  Both forms run the same
 * iterations (0x3E, 0x3E, 0x400): uopt peels the first test and the loop
 * enters with the decremented count, which is why the target still
 * materialises 0x3D and 0x3FF. */
void overlay43FilterImage(Overlay43Image *image) {
    u8 *pixel;
    u32 *word;
    s32 row;
    s32 column;
    u16 sum;

    pixel = image->pixels;
    row = 0x3E;
    while (row--) {
        column = 0x3E;
        while (column--) {
            sum = pixel[0];
            sum += pixel[1];
            sum += pixel[2];
            sum += pixel[0x40];
            sum += pixel[0x42];
            sum += pixel[0x80];
            sum += pixel[0x81];
            sum += pixel[0x82];
            pixel[0x41] = sum >> 3;
            pixel++;
        }
        pixel += 2;
    }

    word = (u32 *)image->pixels;
    row = 0x400;
    while (row--) {
        *word = (*word & 0xF0F0F0F0) >> 4;
        word++;
    }
}
