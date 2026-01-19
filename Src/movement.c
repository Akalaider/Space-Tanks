#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "io.h"

// Fixed‑point scale factor (128 = 7 bits)
#define FP_SCALE 7

// Tunable speeds 
#define SPEED_X 200 // horizontal speed 
#define SPEED_Y 120 // vertical speed (rows are taller) 
#define DIAG_SCALE 71 // ≈ 1/sqrt(2) * 100

// Tank object
static object_t tank;

static const char *sprite = NULL;

void initTank(void) {
    tank.type = player;
    tank.position_x = 10 << FP_SCALE;
    tank.position_y = 61 << FP_SCALE;
    tank.a = 0;  // velocity dx
    tank.b = 0;  // velocity dy
    
    object_t direction = {0};
    direction.position_x = 0;
    direction.position_y = -1;
    sprite = selectTankSprite(direction);
    drawTank(tank, sprite);
}

void controlTank(World *world) {
    uint8_t joyState = readJoystick();

    int16_t dx = 0;
    int16_t dy = 0;

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
        dx = ((dx * DIAG_SCALE) >> 7) + ((dx * DIAG_SCALE) >> 9) + ((dx * DIAG_SCALE) >> 13) + ((dx * DIAG_SCALE) >> 14) + ((dx * DIAG_SCALE) >> 15) + ((dx * DIAG_SCALE) >> 16);
        dy = ((dy * DIAG_SCALE) >> 7) + ((dy * DIAG_SCALE) >> 9) + ((dy * DIAG_SCALE) >> 13) + ((dy * DIAG_SCALE) >> 14) + ((dy * DIAG_SCALE) >> 15) + ((dy * DIAG_SCALE) >> 16); 
    }
    
    // If no movement
    if (dx == 0 && dy == 0)
        return;

    // Erase old tank (using Point for old position)
    Point oldPos = { tank.position_x >> FP_SCALE, tank.position_y >> FP_SCALE };
    eraseTank(oldPos);

    // Compute next fixed‑point position
    int32_t nextX = tank.position_x + dx;
    int32_t nextY = tank.position_y + dy;

    // Convert next position to integer for collision check
    Point nextPos = { nextX >> FP_SCALE, nextY >> FP_SCALE };

    // Try full movement
    if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
        tank.position_x = nextX;
        tank.position_y = nextY;
    } else {
        // Try X-only movement
        Point tryX = { (tank.position_x + dx) >> FP_SCALE, tank.position_y >> FP_SCALE };
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE) {
            tank.position_x += dx;
        }

        // Try Y-only movement
        Point tryY = { tank.position_x >> FP_SCALE, (tank.position_y + dy) >> FP_SCALE };
        if (checkWallCollisionAABB(tryY, world) == COLLISION_NONE) {
            tank.position_y += dy;
        }
    }

    // Store velocity and select sprite
    tank.a = dx;
    tank.b = dy;
    
    object_t direction = {0};
    direction.position_x = dx;
    direction.position_y = dy;
    sprite = selectTankSprite(direction);
    
    // Draw new tank
    drawTank(tank, sprite);
}