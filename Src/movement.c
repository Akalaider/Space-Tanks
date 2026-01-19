#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "io.h"

// Fixed‑point scale factor (100 = 2 decimal places) 
#define FP_SCALE 100 

// Tunable speeds 
#define SPEED_X 200 // horizontal speed 
#define SPEED_Y 120 // vertical speed (rows are taller) 
#define DIAG_SCALE 71 // ≈ 1/sqrt(2) * 100

// Fixed‑point tank position 
static int16_t posX = 10 * FP_SCALE; 
static int16_t posY = 61 * FP_SCALE;

static const char *sprite = NULL;

void initTank(void) {
    sprite = selectTankSprite((Point){0, -1});
    Point p = { posX / FP_SCALE, posY / FP_SCALE };
    drawTank(p, sprite);
}

void controlTank(World *world) {
    uint8_t joyState = readJoystick();

    int dx = 0;
    int dy = 0;

    // Build velocity in fixed‑point units
    if (joyState & JOY_UP)    dy = -SPEED_Y;
    if (joyState & JOY_DOWN)  dy =  SPEED_Y;
    if (joyState & JOY_LEFT)  dx = -SPEED_X;
    if (joyState & JOY_RIGHT) dx =  SPEED_X;

    if (joyState & JOY_CENTER)
        return;

    // LED feedback
    setLED(joyState);

    // Normalize diagonal movement
    if (dx != 0 && dy != 0) {
        dx = dx * DIAG_SCALE / 100;
        dy = dy * DIAG_SCALE / 100;
    }

    // If no movement
    if (dx == 0 && dy == 0)
        return;

    // Erase old tank
    Point oldPos = { posX / FP_SCALE, posY / FP_SCALE };
    eraseTank(oldPos);

    // Compute next fixed‑point position
    int16_t nextX = posX + dx;
    int16_t nextY = posY + dy;

    // Convert next position to integer
    Point nextPos = { nextX / FP_SCALE, nextY / FP_SCALE };

    // Try full movement
    if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
        posX = nextX;
        posY = nextY;
    } else {
        // Try X-only movement
        Point tryX = { (posX + dx) / FP_SCALE, posY / FP_SCALE };
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE) {
            posX += dx;
        }

        // Try Y-only movement
        Point tryY = { posX / FP_SCALE, (posY + dy) / FP_SCALE };
        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE) {
            posY += dy;
        }
    }


    // Draw new tank
    Point drawPos = { posX / FP_SCALE, posY / FP_SCALE };
    sprite = selectTankSprite((Point){ dx, dy });
    drawTank(drawPos, sprite);

    // (replace with timer later)
    for (volatile uint32_t i = 0; i < 100000; i++);
}