#include "world.h"

// AI state machine
typedef enum {
    AI_DIRECT,
    AI_HUG
} AIMode;

// AI object containing all state
typedef struct {
    object_t tank;      // position_x, position_y, a, b, type
    AIMode mode;        // DIRECT eller HUG
    int16_t hugDx;      // wall-hug retning X
    int16_t hugDy;      // wall-hug retning Y
    const char *sprite; // nuværende sprite
} ai_t;

// Public API
void initAITank(ai_t *ai);
void controlAITank(ai_t *ai, World *world);



//void moveTankWithVelocity(World *world, int *posX, int *posY, int dx, int dy);

