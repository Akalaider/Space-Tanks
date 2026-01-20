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
#define DIAG_SCALE 91   // ≈ 1/sqrt(2) * 128

void initAITank(object_t *ai)
{
    ai->type       = enemy;
    ai->position_x = 120 << FP_SCALE;
    ai->position_y = 20  << FP_SCALE;

    // a = mode, b = hugDx, c = hugDy
    ai->a = AI_DIRECT;
    ai->b = 0;
    ai->c = 0;

    // Start med at kigge nedad (samme retningstabel som spilleren)
    uint8_t spriteIndex = 4; // ned
    const char *sprite = selectTankSprite(spriteIndex);
    drawTank(*ai, sprite);
}

static void moveAI(object_t *ai, World *world, int16_t dx, int16_t dy)
{
    // Gem gammel position til selective erase
    Point oldPos = { ai->position_x >> FP_SCALE, ai->position_y >> FP_SCALE };

    int32_t nextX = ai->position_x + dx;
    int32_t nextY = ai->position_y + dy;

    object_t nextObj = *ai;
    nextObj.position_x = nextX >> FP_SCALE;
    nextObj.position_y = nextY >> FP_SCALE;

    if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
        ai->position_x = nextX;
        ai->position_y = nextY;
    } else {
        // Prøv X‑only
        object_t tryX = *ai;
        tryX.position_x = (ai->position_x + dx) >> FP_SCALE;
        tryX.position_y = ai->position_y >> FP_SCALE;
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE)
            ai->position_x += dx;

        // Prøv Y‑only
        object_t tryY = *ai;
        tryY.position_x = ai->position_x >> FP_SCALE;
        tryY.position_y = (ai->position_y + dy) >> FP_SCALE;
        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE)
            ai->position_y += dy;
    }

    // Retnings‑>spriteIndex (samme som i movement.c)
    uint8_t spriteIndex = 0;
    if (dx == 0 && dy < 0) spriteIndex = 0;
    else if (dx > 0 && dy < 0) spriteIndex = 1;
    else if (dx > 0 && dy == 0) spriteIndex = 2;
    else if (dx > 0 && dy > 0) spriteIndex = 3;
    else if (dx == 0 && dy > 0) spriteIndex = 4;
    else if (dx < 0 && dy > 0) spriteIndex = 5;
    else if (dx < 0 && dy == 0) spriteIndex = 6;
    else if (dx < 0 && dy < 0) spriteIndex = 7;

    const char *sprite = selectTankSprite(spriteIndex);

    drawTank(*ai, sprite);
    eraseTankSelective(oldPos, *ai, sprite);
}

void controlAITank(object_t *ai, World *world)
{
    int16_t aiX = getTankX(ai);
    int16_t aiY = getTankY(ai);

    int16_t playerX = getTankX(world->player);   // eller direkte pointer til player‑tank
    int16_t playerY = getTankY(world->player);

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
    if (ai->a == AI_DIRECT) {

        // 1) Prøv fuld bevægelse
        int32_t nextX = ai->position_x + dx;
        int32_t nextY = ai->position_y + dy;

        object_t nextObj = *ai;
        nextObj.position_x = nextX >> FP_SCALE;
        nextObj.position_y = nextY >> FP_SCALE;

        if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
            moveAI(ai, world, dx, dy);
            return;
        }

        // 2) Hvis diagonal → prøv akser separat (hurtig slide)
        if (dx && dy) {
            // Kun X
            object_t posX = *ai;
            posX.position_x = (ai->position_x + dx) >> FP_SCALE;
            posX.position_y = ai->position_y >> FP_SCALE;
            if (checkWallCollisionAABB(posX, world) == COLLISION_NONE) {
                moveAI(ai, world, dx, 0);
                return;
            }

            // Kun Y
            object_t posY = *ai;
            posY.position_x = ai->position_x >> FP_SCALE;
            posY.position_y = (ai->position_y + dy) >> FP_SCALE;
            if (checkWallCollisionAABB(posY, world) == COLLISION_NONE) {
                moveAI(ai, world, 0, dy);
                return;
            }
        }

        // 3) Hvis både diagonal og akser er blokeret → start wall‑hugging
        if (dx != 0) {
            ai->b = 0; // hugDx
            ai->c = (playerY > aiY) ? AI_SPEED_Y : -AI_SPEED_Y; // hugDy
        } else {
            ai->c = 0;
            ai->b = (playerX > aiX) ? AI_SPEED_X : -AI_SPEED_X;
        }

        ai->a = AI_HUG;
    }

    // --- MODE: HUG -------------------------------------------------------
    if (ai->a == AI_HUG) {

        int32_t oldX = ai->position_x;
        int32_t oldY = ai->position_y;

        moveAI(ai, world, ai->b, ai->c);

        // Hvis vi sidder fast → vend om
        if (ai->position_x == oldX && ai->position_y == oldY) {
            ai->b = -ai->b;
            ai->c = -ai->c;
        }

        // Tjek om direkte vej er fri igen
        int32_t nextX = ai->position_x + dx;
        int32_t nextY = ai->position_y + dy;

        object_t nextObj = *ai;
        nextObj.position_x = nextX >> FP_SCALE;
        nextObj.position_y = nextY >> FP_SCALE;

        if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
            ai->a = AI_DIRECT;
        }
    }
}
