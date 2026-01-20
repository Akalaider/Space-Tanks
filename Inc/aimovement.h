#include "world.h"
#include "tank.h"

#define AI_DIRECT 0
#define AI_HUG 1
// AI state machine

// AI object containing all state
/*typedef struct {
    object_t tank;      // position_x, position_y, a, b, type        // DIRECT eller HUG
    int16_t hugDx;      // wall-hug retning X
    int16_t hugDy;      // wall-hug retning Y
    const char *sprite; // nuværende sprite
} ai_t;
*/

// Public API
void initAITank(object_t *ai);
void controlAITank(object_t *ai, object_t *objecthandler, World *world);




//void moveTankWithVelocity(World *world, int *posX, int *posY, int dx, int dy);

