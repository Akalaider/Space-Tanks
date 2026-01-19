#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "io.h"


// Fixed‑point scale factor (100 = 2 decimal places)
#define FP_SCALE 100

// Tunable speeds (integer only)
#define SPEED_X 100 // horizontal speed
#define SPEED_Y 60 // vertical speed (rows are taller)
#define DIAG_SCALE 71 // ≈ 1/sqrt(2) * 100
// Fixed‑point tank position




static const char *sprite = NULL;

static int aiPosX = 120 * FP_SCALE;
static int aiPosY = 20 * FP_SCALE;



void initAITank(void) {
    Point p = { aiPosX / FP_SCALE, aiPosY / FP_SCALE };
    const char *sprite = selectTankSprite((Point){0,1});
    drawTank(p, sprite);
}

void moveTankWithVelocity(World *world, int *posX, int *posY, int dx, int dy) {
    if (dx == 0 && dy == 0)
        return;

    Point oldPos = { *posX / FP_SCALE, *posY / FP_SCALE };
    eraseTank(oldPos);

    int nextX = *posX + dx;
    int nextY = *posY + dy;

    Point nextPos = { nextX / FP_SCALE, nextY / FP_SCALE };

    CollisionSide col = checkWallCollision(nextPos, TANK_RADIUS, world);

    if (col == COLLISION_NONE) {
        *posX = nextX;
        *posY = nextY;
    } else {
        if (col == COLLISION_LEFT || col == COLLISION_RIGHT) {
            int testY = *posY + dy;
            Point p = { *posX / FP_SCALE, testY / FP_SCALE };
            if (checkWallCollision(p, TANK_RADIUS, world) == COLLISION_NONE)
                *posY = testY;
        }
        if (col == COLLISION_TOP || col == COLLISION_BOTTOM) {
            int testX = *posX + dx;
            Point p = { testX / FP_SCALE, *posY / FP_SCALE };
            if (checkWallCollision(p, TANK_RADIUS, world) == COLLISION_NONE)
                *posX = testX;
        }
    }

    Point drawPos = { *posX / FP_SCALE, *posY / FP_SCALE };
    const char *sprite = selectTankSprite((Point){ dx, dy });
    drawTank(drawPos, sprite);
}

void controlAITank(World *world) {
    int dx = 0, dy = 0;

    int aiX = aiPosX / FP_SCALE;
    int aiY = aiPosY / FP_SCALE;

    int playerX = getPlayerX();
    int playerY = getPlayerY();

    // Bevæg mod spilleren
    if (aiX < playerX) dx =  SPEED_X;
    else if (aiX > playerX) dx = -SPEED_X;

    if (aiY < playerY) dy =  SPEED_Y;
    else if (aiY > playerY) dy = -SPEED_Y;

    // Normaliser diagonaler
    if (dx != 0 && dy != 0) {
        dx = dx * DIAG_SCALE / 100;
        dy = dy * DIAG_SCALE / 100;
    }

    moveTankWithVelocity(world, &aiPosX, &aiPosY, dx, dy);
}
