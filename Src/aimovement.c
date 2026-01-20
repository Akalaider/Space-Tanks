#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "io.h"
#include "aimovement.h"

#define FP_SCALE 7

#define AI_SPEED_X 120
#define AI_SPEED_Y 80
#define DIAG_SCALE 91   // ≈ 1/sqrt(2) * 128

void initAITank(object_t *ai)
{
    ai->type       = enemy;
    ai->position_x = 120 << FP_SCALE;
    ai->position_y = 20  << FP_SCALE;

    ai->a = AI_DIRECT;   // mode
    ai->b = 0;           // hugDx
    ai->c = 0;           // hugDy

    const char *sprite = selectTankSprite(4); // ned
    drawTank(*ai, sprite);
}

static void moveAI(object_t *ai, World *world, int16_t dx, int16_t dy)
{
    // Gem gammel position
    Point oldPos = { ai->position_x >> FP_SCALE, ai->position_y >> FP_SCALE };

    // Gem gammel sprite
    uint8_t oldSpriteIndex = getTankSpriteIndex(ai);
    const char *oldSprite = selectTankSprite(oldSpriteIndex);

    // Beregn næste position
    int32_t nextX = ai->position_x + dx;
    int32_t nextY = ai->position_y + dy;

    object_t nextObj = *ai;
    nextObj.position_x = nextX >> FP_SCALE;
    nextObj.position_y = nextY >> FP_SCALE;

    // Fuldt move
    if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
        ai->position_x = nextX;
        ai->position_y = nextY;
    } else {
        // X-only
        object_t tryX = *ai;
        tryX.position_x = (ai->position_x + dx) >> FP_SCALE;
        tryX.position_y = ai->position_y >> FP_SCALE;
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE)
            ai->position_x += dx;

        // Y-only
        object_t tryY = *ai;
        tryY.position_x = ai->position_x >> FP_SCALE;
        tryY.position_y = (ai->position_y + dy) >> FP_SCALE;
        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE)
            ai->position_y += dy;
    }

    // Ny sprite
    uint8_t spriteIndex = 0;
    if (dx == 0 && dy < 0) spriteIndex = 0;
    else if (dx > 0 && dy < 0) spriteIndex = 1;
    else if (dx > 0 && dy == 0) spriteIndex = 2;
    else if (dx > 0 && dy > 0) spriteIndex = 3;
    else if (dx == 0 && dy > 0) spriteIndex = 4;
    else if (dx < 0 && dy > 0) spriteIndex = 5;
    else if (dx < 0 && dy == 0) spriteIndex = 6;
    else if (dx < 0 && dy < 0) spriteIndex = 7;

    ai->c &= ~(0x07 << 4);
    ai->c |= (spriteIndex << 4);

    const char *sprite = selectTankSprite(spriteIndex);

    // Tegn ny tank
    drawTank(*ai, sprite);

    // Slet gammel tank korrekt
    eraseTankSelective(oldPos, *ai, oldSprite);
}

void controlAITank(object_t *ai, object_t *objecthandler, World *world)
{
    // Find spilleren
    object_t *playerObj = NULL;

    for (uint8_t i = 0; i < OBJECTHANDLER_SIZE; i++) {
        if (objecthandler[i].type == player) {
            playerObj = &objecthandler[i];
            break;
        }
    }

    if (!playerObj)
        return;

    int16_t aiX = getTankX(ai);
    int16_t aiY = getTankY(ai);

    int16_t playerX = getTankX(playerObj);
    int16_t playerY = getTankY(playerObj);

    int16_t dx = 0, dy = 0;

    // Retning mod spiller
    if (aiX < playerX) dx =  AI_SPEED_X;
    else if (aiX > playerX) dx = -AI_SPEED_X;

    if (aiY < playerY) dy =  AI_SPEED_Y;
    else if (aiY > playerY) dy = -AI_SPEED_Y;

    // Diagonal normalisering
    if (dx && dy) {
        dx = (dx * DIAG_SCALE) >> FP_SCALE;
        dy = (dy * DIAG_SCALE) >> FP_SCALE;
    }

    // DIRECT MODE
    if (ai->a == AI_DIRECT) {

        int32_t nextX = ai->position_x + dx;
        int32_t nextY = ai->position_y + dy;

        object_t nextObj = *ai;
        nextObj.position_x = nextX >> FP_SCALE;
        nextObj.position_y = nextY >> FP_SCALE;

        if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
            moveAI(ai, world, dx, dy);
            return;
        }

        // X-only
        object_t posX = *ai;
        posX.position_x = (ai->position_x + dx) >> FP_SCALE;
        posX.position_y = ai->position_y >> FP_SCALE;
        if (checkWallCollisionAABB(posX, world) == COLLISION_NONE) {
            moveAI(ai, world, dx, 0);
            return;
        }

        // Y-only
        object_t posY = *ai;
        posY.position_x = ai->position_x >> FP_SCALE;
        posY.position_y = (ai->position_y + dy) >> FP_SCALE;
        if (checkWallCollisionAABB(posY, world) == COLLISION_NONE) {
            moveAI(ai, world, 0, dy);
            return;
        }

        // Skift til HUG mode
        ai->a = AI_HUG;
    }

    // HUG MODE
    if (ai->a == AI_HUG) {

        int32_t oldX = ai->position_x;
        int32_t oldY = ai->position_y;

        moveAI(ai, world, dx, dy);

        if (ai->position_x == oldX && ai->position_y == oldY) {
            dx = -dx;
            dy = -dy;
        }

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
