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

#define AI_STATE_MASK   0x03        // bit 0-1
#define GET_STATE(ai)   ((ai)->c & AI_STATE_MASK)
#define SET_STATE(ai,s) ((ai)->c = ((ai)->c & ~AI_STATE_MASK) | (s))

#define SPRITE_SHIFT    4
#define SPRITE_MASK     (0x07 << SPRITE_SHIFT)
#define SET_SPRITE(ai,idx) ((ai)->c = ((ai)->c & ~SPRITE_MASK) | ((idx) << SPRITE_SHIFT))

void initAITank(object_t *ai)
{
    ai->type       = enemy;
    ai->position_x = 120 << FP_SCALE;
    ai->position_y = 20  << FP_SCALE;

    ai->a = TANK_WIDTH;
    ai->b = TANK_HEIGHT;

    ai->c = 0;
    ai->c |= (AI_DIRECT);      // bit 0–1 = AI state
    ai->c |= (0 << 4);         // spriteIndex = 0

    const char *sprite = selectTankSprite(4); // ned
    drawTank(*ai, sprite);
}

static void moveAI(object_t *ai, World *world, int16_t dx, int16_t dy)
{
    // Husk sidste retning
    static int16_t lastDx = 0;
    static int16_t lastDy = 1;   // default: peger nedad

    // Gem gammel position
    Point oldPos = { ai->position_x >> FP_SCALE, ai->position_y >> FP_SCALE };

    // Gem gammel sprite
    uint8_t oldSpriteIndex = getTankSpriteIndex(ai);
    const char *oldSprite = selectTankSprite(oldSpriteIndex);

    int32_t nextX = ai->position_x + dx;
    int32_t nextY = ai->position_y + dy;

    object_t nextObj = *ai;
    nextObj.position_x = nextX >> FP_SCALE;
    nextObj.position_y = nextY >> FP_SCALE;

    // --- Fuld bevægelse ---
    if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
        ai->position_x = nextX;
        ai->position_y = nextY;
    }
    else {
        // --- X-only ---
        object_t tryX = *ai;
        tryX.position_x = (ai->position_x + dx) >> FP_SCALE;
        tryX.position_y = ai->position_y >> FP_SCALE;

        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE)
            ai->position_x += dx;

        // --- Y-only ---
        object_t tryY = *ai;
        tryY.position_x = ai->position_x >> FP_SCALE;
        tryY.position_y = (ai->position_y + dy) >> FP_SCALE;

        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE)
            ai->position_y += dy;
    }

    // --- Opdater sidste retning, hvis vi bevægede os ---
    if (dx != 0 || dy != 0) {
        lastDx = dx;
        lastDy = dy;
    }

    // --- Sprite-opdatering ---
    int16_t sdx = dx;
    int16_t sdy = dy;

    // Hvis AI står stille → brug sidste retning
    if (sdx == 0 && sdy == 0) {
        sdx = lastDx;
        sdy = lastDy;
    }

    uint8_t spriteIndex = 0;
    if (sdx == 0 && sdy < 0) spriteIndex = 0;
    else if (sdx > 0 && sdy < 0) spriteIndex = 1;
    else if (sdx > 0 && sdy == 0) spriteIndex = 2;
    else if (sdx > 0 && sdy > 0) spriteIndex = 3;
    else if (sdx == 0 && sdy > 0) spriteIndex = 4;
    else if (sdx < 0 && sdy > 0) spriteIndex = 5;
    else if (sdx < 0 && sdy == 0) spriteIndex = 6;
    else if (sdx < 0 && sdy < 0) spriteIndex = 7;

    SET_SPRITE(ai, spriteIndex);

    const char *sprite = selectTankSprite(spriteIndex);

    // Tegn ny tank
    drawTank(*ai, sprite);

    // Selektiv erase
    eraseTankSelective(oldPos, *ai, oldSprite);
}

void controlAITank(object_t *ai, object_t *objecthandler, World *world)
{
    // Hug-retning gemmes mellem frames
    static int16_t hugDx = 0;
    static int16_t hugDy = 0;

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

    // Normaliser diagonal
    if (dx && dy) {
        dx = (dx * DIAG_SCALE) >> FP_SCALE;
        dy = (dy * DIAG_SCALE) >> FP_SCALE;
    }
    // --- HOLD AFSTAND FRA SPILLEREN (10 pixels) ---
    int16_t distX = aiX - playerX;
    if (distX < 0) distX = -distX;

    int16_t distY = aiY - playerY;
    if (distY < 0) distY = -distY;

    // Hvis vi er tættere end 10 pixels i begge akser → stop fremad bevægelse
    if (distX < 10 && distY < 10) {
        dx = 0;
        dy = 0;
    }

    // --- MODE: DIRECT ----------------------------------------------------
    if (GET_STATE(ai) == AI_DIRECT) {

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
            int32_t altX = ai->position_x + dx;
            object_t posX = *ai;
            posX.position_x = altX >> FP_SCALE;
            posX.position_y = ai->position_y >> FP_SCALE;
            if (checkWallCollisionAABB(posX, world) == COLLISION_NONE) {
                moveAI(ai, world, dx, 0);
                return;
            }

            // Kun Y
            int32_t altY = ai->position_y + dy;
            object_t posY = *ai;
            posY.position_x = ai->position_x >> FP_SCALE;
            posY.position_y = altY >> FP_SCALE;
            if (checkWallCollisionAABB(posY, world) == COLLISION_NONE) {
                moveAI(ai, world, 0, dy);
                return;
            }
        }

        // 3) Hvis både diagonal og akser er blokeret → start wall‑hugging
        if (dx != 0) {
            hugDx = 0;
            hugDy = (playerY > aiY) ? AI_SPEED_Y : -AI_SPEED_Y;
        } else {
            hugDy = 0;
            hugDx = (playerX > aiX) ? AI_SPEED_X : -AI_SPEED_X;
        }

        SET_STATE(ai, AI_HUG);
    }

    // --- MODE: HUG -------------------------------------------------------
    if (GET_STATE(ai) == AI_HUG) {

        int32_t oldX = ai->position_x;
        int32_t oldY = ai->position_y;

        moveAI(ai, world, hugDx, hugDy);

        // Hvis vi ikke flyttede os, vend hug-retning
        if (ai->position_x == oldX && ai->position_y == oldY) {
            hugDx = -hugDx;
            hugDy = -hugDy;
        }

        // Tjek om direkte vej er fri igen
        int32_t nextX = ai->position_x + dx;
        int32_t nextY = ai->position_y + dy;

        object_t nextObj = *ai;
        nextObj.position_x = nextX >> FP_SCALE;
        nextObj.position_y = nextY >> FP_SCALE;

        if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
            SET_STATE(ai, AI_DIRECT);
        }
    }
}
