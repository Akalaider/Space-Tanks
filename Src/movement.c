#include "movement.h"

void initTank(object_t tank) {
    tank.type = player;
    tank.position_x = 10 << FP_SCALE;
    tank.position_y = 61 << FP_SCALE;
    tank.a = TANK_WIDTH;
    tank.b = TANK_HEIGHT;
    tank.c = (0 << 0) + (0 << 2); //bit 0-1: controller type --- bit 2->: health
    
    // clean up
    object_t direction = {0};
    direction.position_x = 0;
    direction.position_y = -1;
    drawTank(tank);
    //clean up end
}

uint8_t readController(object_t tank){
	switch (tank.c & ((int32_t) 0x3)){
		case JOYSTICK_CONTROL:
			//add joystickcontroller return
			return;
		case KEYBOARD_CONTROL:
			//add joystickcontroller return
			return;
		case HAT_CONTROL:
			return readJoystick();
		default:
			return readJoystick();
	}
}

void controlTank(World *world, object_t tank) {
    uint8_t joyState = readController(tank);

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
    if (dx && dy) {
        dx = (dx * DIAG_SCALE) >> FP_SCALE;
        dy = (dy * DIAG_SCALE) >> FP_SCALE;
    }
    
    // If no movement
    if (dx == 0 && dy == 0)
        return;

    // Compute next fixed‑point position
    int32_t nextX = tank.position_x + dx;
    int32_t nextY = tank.position_y + dy;

    // Point for erasing old tank
    Point oldPos = { tank.position_x >> FP_SCALE, tank.position_y >> FP_SCALE };

    // Convert next position to integer for collision check
    object_t nextPos = tank;
    nextPos.position_x = nextX >> FP_SCALE;
    nextPos.position_y = nextY >> FP_SCALE;

    // Try full movement
    if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
        tank.position_x = nextX;
        tank.position_y = nextY;
    } else {
        // Try X-only movement
        object_t tryX = tank;
        tryX.position_x = (tank.position_x + dx) >> FP_SCALE;
        tryX.position_y = tank.position_y >> FP_SCALE;
        if (checkWallCollisionAABB(tryX, world) == COLLISION_NONE) {
            tank.position_x += dx;
        }

        // Try Y-only movement
        object_t tryY = tank;
        tryY.position_x = tank.position_x >> FP_SCALE;
        tryY.position_y = (tank.position_y + dy) >> FP_SCALE;
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
    // Draw new tank
    drawTank(tank);
    eraseTankSelective(oldPos, tank, sprite);  // erase leftovers

}

int16_t getPlayerX(void) {
    return tank.position_x >> FP_SCALE;
}

int16_t getPlayerY(void) {
    return tank.position_y >> FP_SCALE;
}
