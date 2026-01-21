#include "world.h"
#include "tank.h"

#define AI_DIRECT 0
#define AI_HUG 1
void initAITank(object_t *ai);
void controlAITank(object_t *ai, object_t *objecthandler, World *world);
#define FP_SCALE 7

#define AI_SPEED_X 120
#define AI_SPEED_Y 80
#define DIAG_SCALE 91   // ≈ 1/sqrt(2) * 128

#define AI_STATE_MASK   0x03        // bit 0-1
#define GET_STATE(ai)   ((ai)->c & AI_STATE_MASK)
#define SET_STATE(ai,s) ((ai)->c = ((ai)->c & ~AI_STATE_MASK) | (s))

#define SPRITE_SHIFT    4
#define SPRITE_MASK     (0x07 << SPRITE_SHIFT)
#define SET_SPRITE(ai,idx) ((ai)->c = ((ai)->c & ~SPRITE_MASK) | ((idx) << SPRITE_SHIFT))
