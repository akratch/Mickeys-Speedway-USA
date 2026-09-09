#include "PR/ultratypes.h"

typedef struct Overlay73Vertex {
    s16 x;
    s16 y;
    s16 z;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Overlay73Vertex;

typedef struct Overlay73State {
    Overlay73Vertex vertices[12];
    void *resource;
    u8 vertexBank;
    u8 active;
    s16 phase;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 timer;
    s16 phaseLimit;
    s16 status;
    s32 counter;
} Overlay73State;

typedef struct Overlay73Config {
    u8 pad00[0x22];
    s8 resourceCount;
} Overlay73Config;

typedef struct Overlay73Object {
    u8 pad00[0x08];
    f32 outputScale;
    u8 pad0C[0x14];
    f32 value;
    u8 pad24[0x1C];
    Overlay73Config *config;
    u8 pad44[0x08];
    f32 *output;
    u8 pad50[0x14];
    Overlay73State *state;
    void **resources;
} Overlay73Object;

typedef struct Overlay73Header {
    u8 pad00[4];
    s16 x;
    s16 y;
    s16 z;
    u16 scale;
    u8 resourceIndex;
    u8 outputScale;
} Overlay73Header;

extern s16 D_80[12][3];
extern f32 D_0;
extern f32 D_4;
extern f32 D_8;

/* DKR v77/v80 and JFG contain no exact donor for this initializer. */
/* Fresh configured full-TU and isolated canonical V0 objects are text-identical
 * at 90/100 raw and 92/100 runtime-relocation-normalized words, with exact
 * 100-word extent and frame 0x8. The first mismatch is +0x40; all eight
 * codegen residual words are one reciprocal a2/a3 allocation swap at
 * +0x40/+0x50/+0x80/+0x84/+0x94/+0x9C/+0xA8/+0xB0. All 119 flag-lattice
 * configurations were attempted: 53 compiled, thirteen O2/MIPS-II rows tied
 * with V0, and none was exact. A fidelity-clean procedure-0 trace shows the
 * loop/resource-index web selecting a2 while a3 has the same zero cost. The
 * diagnostic force to a3 clears every normalized site and cascades the paired
 * web back to a2, proving the cause but never a promotable result. Naturally
 * initializing the shared index before the preserve-state branch regressed to
 * 55/100 raw and normalized words and shifted the first LOCAL pair, so no
 * strict gain authorized a scoped form or batch. The 122-stock-build cap is
 * exhausted. Runtime table 2 resolves
 * D_80 through LOCAL base
 * +0xEB0/addend +0x80 (module +0xF30), while D_0/D_4/D_8 use LOCAL base
 * +0xF80 and addends 0/+4/+8. The 16 zero-immediate calls labelled with this
 * module-zero symbol in overlay_073.s are outgoing SYMBOL relocations, not
 * callers. ORT 1248 exports +0; resident relocation 90 at
 * func_8000AA38+0x42C is the sole inbound. Fresh relocation-surface comparison
 * reports target/candidate 8/8, offset/type 8/8, identity 8/8, and all eight
 * resolved. Owned +0x0..+0x190 / ROM 0x18CAAC0..0x18CAC50 has no target
 * padding; the next assembly owner starts at +0x190. Ordinary full-TU/link
 * equality remains fallback-only and no linked C proof exists. */
void func_overlay_073_F0000000_18CAAC0(Overlay73Object *object, Overlay73Header *header, s32 preserveState)
{
  s32 i;
  Overlay73State *state;
  Overlay73Vertex *vertex;
  s16 (*source)[3];
  state = object->state;
  if (preserveState == 0)
  {
    object->value = 0.0f;
    state->phase = 0;
    state->counter = 0;
    state->status = 0;
    state->active = preserveState * 0;
    state->phaseLimit = 0x180;
    state->timer = 0.0f;
    vertex = state->vertices;
    source = D_80;
    i = 11;
    do
    {
      vertex->x = (*source)[0];
      vertex->y = (*source)[1];
      vertex->z = (*source)[2];
      vertex->r = 0xFF;
      vertex->g = 0xFF;
      vertex->b = 0xFF;
      vertex->a = 0xFF;
      vertex++;
      source++;
    }
    while (i--);
    state->vertexBank = 1;
  }
  i = header->resourceIndex;
  if (i >= object->config->resourceCount)
  {
    i = 0;
  }
  state->resource = object->resources[i];
  state->x = header->x;
  state->y = header->y;
  state->z = header->z;
  state->scale = header->scale;
  object->outputScale = ((f32) header->outputScale) * D_0;
  if (object->output != ((void *) 0))
  {
    object->output[0] = object->outputScale * D_4;
    object->output[1] = object->outputScale * D_8;
  }
}

