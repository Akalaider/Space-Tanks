#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "io.h"
#include "aimovement.h"

// Same fixed‑point scale as player
#define FP_SCALE 7

// AI speeds (slower than player)
#define AI_SPEED_X 120
#define AI_SPEED_Y 80
#define DIAG_SCALE 91   // samme som i movement.c

void initAITank(ai_t *ai) {
    ai->tank.type = enemy;
    ai->tank.position_x = 120 << FP_SCALE;
    ai->tank.position_y = 20  << FP_SCALE;
    ai->tank.a = 0;
    ai->tank.b = 0;

    ai->mode = AI_DIRECT;
    ai->hugDx = 0;
    ai->hugDy = 0;

    object_t dir = {0};
    dir.position_y = 1;
    ai->sprite = selectTankSprite(dir);

    drawTank(ai->tank, ai->sprite);
}

static void moveAI(ai_t *ai, World *world, int16_t dx, int16_t dy) {
    // Gem gammel position til selektiv erase
    Point oldPos = { ai->tank.position_x >> FP_SCALE, ai->tank.position_y >> FP_SCALE };

    int32_t nextX = ai->tank.position_x + dx;
    int32_t nextY = ai->tank.position_y + dy;

    Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

    if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
        ai->tank.position_x = nextX;
        ai->tank.position_y = nextY;
    } else {
        // Try X only
        Point tryX = { (ai->tank.position_x + dx) >> FP_SCALE, ai->tank.position_y >> FP_SCALE };
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE)
            ai->tank.position_x += dx;

        // Try Y only
        Point tryY = { ai->tank.position_x >> FP_SCALE, (ai->tank.position_y + dy) >> FP_SCALE };
        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE)
            ai->tank.position_y += dy;
    }

    // Opdater velocity
    ai->tank.a = dx;
    ai->tank.b = dy;

    // Opdater sprite
    object_t dir = {0};
    dir.position_x = dx;
    dir.position_y = dy;
    ai->sprite = selectTankSprite(dir);

    // Tegn ny tank
    drawTank(ai->tank, ai->sprite);

    // Selektiv erase af leftovers
    eraseTankSelective(oldPos, ai->tank, ai->sprite);
}

void controlAITank(ai_t *ai, World *world) {
    int aiX = ai->tank.position_x >> FP_SCALE;
    int aiY = ai->tank.position_y >> FP_SCALE;

    int16_t playerX = getPlayerX();
    int16_t playerY = getPlayerY();

    int16_t dx = 0, dy = 0;

    // Retning mod spiller
    if (aiX < playerX) dx =  AI_SPEED_X;
    else if (aiX > playerX) dx = -AI_SPEED_X;

    if (aiY < playerY) dy =  AI_SPEED_Y;
    else if (aiY > playerY) dy = -AI_SPEED_Y;

    // Normaliser diagonal
    if (dx && dy) {
        dx = (dx * DIAG_SCALE) >> FP_SCALE;
        dy = (dy * DIAG_SCALE) >> FP_SCALE;
    }

    // --- MODE: DIRECT ----------------------------------------------------
    if (ai->mode == AI_DIRECT) {
        int32_t nextX = ai->tank.position_x + dx;
        int32_t nextY = ai->tank.position_y + dy;
        Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

        if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
            moveAI(ai, world, dx, dy);
            return;
        }

        // Start wall hugging
        if (dx != 0) {
            ai->hugDx = 0;
            ai->hugDy = (playerY > aiY) ? AI_SPEED_Y : -AI_SPEED_Y;
        } else {
            ai->hugDy = 0;
            ai->hugDx = (playerX > aiX) ? AI_SPEED_X : -AI_SPEED_X;
        }

        ai->mode = AI_HUG;
    }

    // --- MODE: HUG -------------------------------------------------------
    if (ai->mode == AI_HUG) {
        int32_t oldX = ai->tank.position_x;
        int32_t oldY = ai->tank.position_y;

        moveAI(ai, world, ai->hugDx, ai->hugDy);

        // Hvis vi ikke flyttede os, vend hug-retning
        if (ai->tank.position_x == oldX && ai->tank.position_y == oldY) {
            ai->hugDx = -ai->hugDx;
            ai->hugDy = -ai->hugDy;
        }

        // Tjek om direkte vej er fri igen
        int32_t nextX = ai->tank.position_x + dx;
        int32_t nextY = ai->tank.position_y + dy;
        Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

        if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
            ai->mode = AI_DIRECT;
        }
    }
}
