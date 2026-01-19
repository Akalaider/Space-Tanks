#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "io.h"

// Same fixed‑point scale as player
#define FP_SCALE 7

// AI speeds (slower than player)
#define AI_SPEED_X 120
#define AI_SPEED_Y 80
#define DIAG_SCALE 71

// AI state machine
typedef enum {
    AI_DIRECT,
    AI_HUG
} AIMode;

static AIMode aiMode = AI_DIRECT;
static int16_t hugDx = 0;
static int16_t hugDy = 0;

// AI tank object
static object_t aiTank;
static const char *aiSprite = NULL;

void initAITank(void) {
    aiTank.type = enemy;
    aiTank.position_x = 120 << FP_SCALE;
    aiTank.position_y = 20  << FP_SCALE;
    aiTank.a = 0;
    aiTank.b = 0;

    object_t dir = {0};
    dir.position_y = 1;
    aiSprite = selectTankSprite(dir);

    drawTank(aiTank, aiSprite);
}

static void moveAI(World *world, int16_t dx, int16_t dy) {
    // Erase old
    Point oldPos = { aiTank.position_x >> FP_SCALE, aiTank.position_y >> FP_SCALE };
    eraseTank(oldPos);

    int32_t nextX = aiTank.position_x + dx;
    int32_t nextY = aiTank.position_y + dy;

    Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

    if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
        aiTank.position_x = nextX;
        aiTank.position_y = nextY;
    } else {
        // Try X only
        Point tryX = { (aiTank.position_x + dx) >> FP_SCALE, aiTank.position_y >> FP_SCALE };
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE) {
            aiTank.position_x += dx;
        }

        // Try Y only
        Point tryY = { aiTank.position_x >> FP_SCALE, (aiTank.position_y + dy) >> FP_SCALE };
        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE) {
            aiTank.position_y += dy;
        }
    }

    // Update velocity
    aiTank.a = dx;
    aiTank.b = dy;

    // Update sprite
    object_t dir = {0};
    dir.position_x = dx;
    dir.position_y = dy;
    aiSprite = selectTankSprite(dir);

    drawTank(aiTank, aiSprite);
}

void controlAITank(World *world) {
    int aiX = aiTank.position_x >> FP_SCALE;
    int aiY = aiTank.position_y >> FP_SCALE;

    int playerX = getPlayerX();
    int playerY = getPlayerY();

    int16_t dx = 0;
    int16_t dy = 0;

    // Direct vector toward player
    if (aiX < playerX) dx =  AI_SPEED_X;
    else if (aiX > playerX) dx = -AI_SPEED_X;

    if (aiY < playerY) dy =  AI_SPEED_Y;
    else if (aiY > playerY) dy = -AI_SPEED_Y;

    // Normalize diagonal
    if (dx != 0 && dy != 0) {
        dx = (dx * DIAG_SCALE) >> 7;
        dy = (dy * DIAG_SCALE) >> 7;
    }

    // --- MODE: DIRECT ----------------------------------------------------
    if (aiMode == AI_DIRECT) {
        int32_t nextX = aiTank.position_x + dx;
        int32_t nextY = aiTank.position_y + dy;
        Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

        if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
            moveAI(world, dx, dy);
            return;
        }

        // Start wall hugging
        if (dx != 0) {
            hugDx = 0;
            hugDy = (playerY > aiY) ? AI_SPEED_Y : -AI_SPEED_Y;
        } else {
            hugDy = 0;
            hugDx = (playerX > aiX) ? AI_SPEED_X : -AI_SPEED_X;
        }

        aiMode = AI_HUG;
    }

    // --- MODE: HUG -------------------------------------------------------
    if (aiMode == AI_HUG) {
        int32_t oldX = aiTank.position_x;
        int32_t oldY = aiTank.position_y;

        moveAI(world, hugDx, hugDy);

        // If stuck, reverse hug direction
        if (aiTank.position_x == oldX && aiTank.position_y == oldY) {
            hugDx = -hugDx;
            hugDy = -hugDy;
        }

        // Check if direct path is open again
        int32_t nextX = aiTank.position_x + dx;
        int32_t nextY = aiTank.position_y + dy;
        Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

        if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
            aiMode = AI_DIRECT;
        }
    }
}
