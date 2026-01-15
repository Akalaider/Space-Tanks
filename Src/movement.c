#include "movement.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "joystick.h"

Point tankPos = {10, 61};  // Bottom left
Point vel = {0, 0};  // Start 
static const char *sprite = NULL;

void initTank(void) {
    sprite = selectTankSprite((Point){0, -1});
    drawTank(tankPos, sprite);
}


void controlTank(World *world) {
    // Read joystick input
    uint8_t joyState = readJoy();

    // Update velocity based on joystick
    // Reset velocity first
    vel.x = 0;
    vel.y = 0;

    // Check each direction
    if (joyState & (1 << 0)) {  // UP
        vel.y = -1;
    }
    if (joyState & (1 << 1)) {  // DOWN
        vel.y = 1;
    }
    if (joyState & (1 << 2)) {  // LEFT
        vel.x = -1;
    }
    if (joyState & (1 << 3)) {  // RIGHT
        vel.x = 1;
    }
    if (joyState & (1 << 4)) {  // CENTER - exit to menu
        return;
    }

    // Visual feedback
    controlLED(joyState);

    // Only move and redraw if velocity changed or tank is moving
    if (vel.x != 0 || vel.y != 0) {
        eraseTank(tankPos);
        
        // Calculate next position
        Point nextPos = {tankPos.x + vel.x, tankPos.y + vel.y};
        
        // Check collision
        CollisionSide collision = checkWallCollision(nextPos, TANK_RADIUS, world);
        
        if (collision != COLLISION_NONE) {
            // Don't move if there's a collision
            // Optionally: allow sliding along walls
            if (collision == COLLISION_LEFT || collision == COLLISION_RIGHT) {
                // Can still move vertically
                Point testY = {tankPos.x, tankPos.y + vel.y};
                if (checkWallCollision(testY, TANK_RADIUS, world) == COLLISION_NONE) {
                    tankPos.y += vel.y;
                }
            }
            if (collision == COLLISION_TOP || collision == COLLISION_BOTTOM) {
                // Can still move horizontally
                Point testX = {tankPos.x + vel.x, tankPos.y};
                if (checkWallCollision(testX, TANK_RADIUS, world) == COLLISION_NONE) {
                    tankPos.x += vel.x;
                }
            }
        } else {
            // No collision, move freely
            tankPos = nextPos;
        }
        
        // Select sprite based on velocity
        sprite = selectTankSprite(vel);
        drawTank(tankPos, sprite);
    }

    for(volatile uint32_t i = 0; i < 100000; i++); // adjust for speed - change when implementing TIMER
}