#include "movement.h"
#include "aimovement.h"

void initAITank(object_t *ai, int16_t x, int16_t y)
{
    ai->type       = enemy;
    ai->position_x = x << FP_SCALE;
    ai->position_y = y << FP_SCALE; //Startpositionen i fixed-point format

    ai->a = TANK_WIDTH;
    ai->b = TANK_HEIGHT; //Hitbox bredde og højde

    ai->c = 0;
    ai->c |= AI_DIRECT;      // bit 0–1 = AI state
    ai->c |= (4 << 4);       // spriteIndex = 4
    ai->c |= (1 << 2);       // health = 3
    ai->c |= (5 << 8);       // 5 normal bullets
    ai->c &= ~(0x3FF << 17);
    ai->c |= (50 << 17);   // 50 ms cooldown


    const char *sprite = selectTankSprite(4); // ned
    drawTank(*ai, sprite);
}
static void aiTryShoot(object_t *ai, object_t *objecthandler) {

    if (getTankCooldown(ai) != 0) // Cooldown check
        return;

    uint8_t dir = getTankSpriteIndex(ai); // Find retning

    uint8_t bulletType = (getTankPowerup(ai) & 1) ? 3 : 2; // Vælg bullet-type

    shootBullet(ai, objecthandler, dir, bulletType); // Skyd
}

static void moveAI(object_t *ai, World *world, int16_t dx, int16_t dy)
{
    // Husk sidste retning
    static int16_t lastDx = 0;
    static int16_t lastDy = 1;

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

    // Bevægelse
    if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
        ai->position_x = nextX;
        ai->position_y = nextY; // Hvis der ikke er nogen væg, flyt tank
    }
    else {
        // Hvis der er en væg, prøv kun x retning
        object_t tryX = *ai;
        tryX.position_x = (ai->position_x + dx) >> FP_SCALE;
        tryX.position_y = ai->position_y >> FP_SCALE;

        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE)
            ai->position_x += dx;

        // Ellers prøv kun y retning
        object_t tryY = *ai;
        tryY.position_x = ai->position_x >> FP_SCALE;
        tryY.position_y = (ai->position_y + dy) >> FP_SCALE;

        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE)
            ai->position_y += dy;
    }

    // Opdater sidste retning, hvis den bevægede sig
    if (dx != 0 || dy != 0) {
        lastDx = dx;
        lastDy = dy;
    }

    // Sprite-opdatering
    int16_t sdx = dx;
    int16_t sdy = dy;

    // Hvis AI står stille, brug sidste retning
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
    else if (sdx < 0 && sdy < 0) spriteIndex = 7; // Bestem hvilken sprite

    SET_SPRITE(ai, spriteIndex);

    const char *sprite = selectTankSprite(spriteIndex);

    drawTank(*ai, sprite);

    eraseTankSelective(oldPos, *ai, oldSprite); // Selektiv slet
}

void controlAITank(object_t *ai, object_t *objecthandler, World *world)
{
	// Cooldown nedtælling
	uint16_t cd = getTankCooldown(ai);
	if (cd > 0) {
	    ai->c &= ~(0x3FF << 17);
	    ai->c |= ((cd - 1) << 17);
	}
	static int16_t hugDx = 0;
    static int16_t hugDy = 0; // Bruges til at komme rundt om vægge

    object_t *playerObj = NULL;
    for (uint8_t i = 0; i < OBJECTHANDLER_SIZE; i++) {
        if (objecthandler[i].type == player) {
            playerObj = &objecthandler[i]; // Find player objecthandler
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

    if (aiX < playerX) dx =  AI_SPEED_X;
    else if (aiX > playerX) dx = -AI_SPEED_X; // AI går mod player i x retning

    if (aiY < playerY) dy =  AI_SPEED_Y;
    else if (aiY > playerY) dy = -AI_SPEED_Y; // AI går mod player i y retning

    if (dx && dy) {
        dx = (dx * DIAG_SCALE) >> FP_SCALE;
        dy = (dy * DIAG_SCALE) >> FP_SCALE; // Gør diagonal ligeså hurtig som op/ned
    }
    int16_t distX = aiX - playerX;
    if (distX < 0) distX = -distX;

    int16_t distY = aiY - playerY;
    if (distY < 0) distY = -distY;

    if (distX < 20 && distY < 10) {
        dx = 0;
        dy = 0; // AI stopper hvis den kommer for tæt på player
    }
    aiTryShoot(ai, objecthandler);

    // MODE: Direct
    if (GET_STATE(ai) == AI_DIRECT) {

        // Prøv fuld bevægelse
        int32_t nextX = ai->position_x + dx;
        int32_t nextY = ai->position_y + dy;

        object_t nextObj = *ai;
        nextObj.position_x = nextX >> FP_SCALE;
        nextObj.position_y = nextY >> FP_SCALE;

        if (checkWallCollisionAABB(nextObj, world) == COLLISION_NONE) {
            moveAI(ai, world, dx, dy);
            return;
        }

        // Hvis diagonal blokeret, prøv akser separat
        if (dx && dy) {
            // Kun x
            int32_t altX = ai->position_x + dx;
            object_t posX = *ai;
            posX.position_x = altX >> FP_SCALE;
            posX.position_y = ai->position_y >> FP_SCALE;
            if (checkWallCollisionAABB(posX, world) == COLLISION_NONE) {
                moveAI(ai, world, dx, 0);
                return;
            }

            // Kun y
            int32_t altY = ai->position_y + dy;
            object_t posY = *ai;
            posY.position_x = ai->position_x >> FP_SCALE;
            posY.position_y = altY >> FP_SCALE;
            if (checkWallCollisionAABB(posY, world) == COLLISION_NONE) {
                moveAI(ai, world, 0, dy);
                return;
            }
        }

        // Hvis både diagonal og akser er blokeret, set state til AI_HUG
        if (dx != 0) {
            hugDx = 0;
            hugDy = (playerY > aiY) ? AI_SPEED_Y : -AI_SPEED_Y;
        } else {
            hugDy = 0;
            hugDx = (playerX > aiX) ? AI_SPEED_X : -AI_SPEED_X;
        }

        SET_STATE(ai, AI_HUG);
    }

    // MODE: Hug
    if (GET_STATE(ai) == AI_HUG) {

        int32_t oldX = ai->position_x;
        int32_t oldY = ai->position_y;

        moveAI(ai, world, hugDx, hugDy);

        // Hvis ai ikke flyttede sig, vend hug-retning
        if (ai->position_x == oldX && ai->position_y == oldY) {
            hugDx = -hugDx;
            hugDy = -hugDy;
        }

        // Tjek om direkte vej er fri igen, hvis fri, sæt til AI_DIRECT igen
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
