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
static int posX = 10 * FP_SCALE; 
static int posY = 61 * FP_SCALE;

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

    // If no movement, skip everything
    if (dx == 0 && dy == 0)
        return;

    // Erase old tank
    Point oldPos = { posX / FP_SCALE, posY / FP_SCALE };
    eraseTank(oldPos);

    // Compute next fixed‑point position
    int nextX = posX + dx;
    int nextY = posY + dy;

    // Convert to integer for collision test
    Point nextPos = { nextX / FP_SCALE, nextY / FP_SCALE };

    // Collision detection
    CollisionSide col = checkWallCollision(nextPos, TANK_RADIUS, world);

    if (col == COLLISION_NONE) {
        // Move freely
        posX = nextX;
        posY = nextY;
    } else {
        // Sliding logic (integer‑safe)
        if (col == COLLISION_LEFT || col == COLLISION_RIGHT) {
            int testY = posY + dy;
            Point p = { posX / FP_SCALE, testY / FP_SCALE };
            if (checkWallCollision(p, TANK_RADIUS, world) == COLLISION_NONE)
                posY = testY;
        }
        if (col == COLLISION_TOP || col == COLLISION_BOTTOM) {
            int testX = posX + dx;
            Point p = { testX / FP_SCALE, posY / FP_SCALE };
            if (checkWallCollision(p, TANK_RADIUS, world) == COLLISION_NONE)
                posX = testX;
        }
    }

    // Draw new tank
    Point drawPos = { posX / FP_SCALE, posY / FP_SCALE };
    sprite = selectTankSprite((Point){ dx, dy });
    drawTank(drawPos, sprite);

    // (replace with timer later)
    for (volatile uint32_t i = 0; i < 100000; i++);
}