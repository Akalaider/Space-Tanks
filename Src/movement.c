#include "movement.h"

// Fixed‑point scale factor (128 = 7 bits)
#define FP_SCALE 7

// Tunable speeds 

#define SPEED_X 400 // horizontal speed
#define SPEED_Y 240 // vertical speed (rows are taller)


#define DIAG_SCALE 91 // ≈ 1/sqrt(2) * 128

void initTank(object_t *tank)
{
    tank->type = player;
    tank->position_x = 10 << FP_SCALE;
    tank->position_y = 61 << FP_SCALE;
    tank->a = TANK_WIDTH;
    tank->b = TANK_HEIGHT;

    tank->c = 0;                 // clear all bits
    tank->c |= HAT_CONTROL;      // control mode = hat
    tank->c |= (3 << 2);         // health = 3
    tank->c |= (0 << 4);         // sprite index = 0
    tank->c |= (5 << 8);         // Set ammo to 5

    const char *sprite = selectTankSprite(getTankSpriteIndex(tank));
    drawTank(*tank, sprite);
}


uint8_t readController(const object_t *tank){
    switch (tank->c & 0x03) {
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

void controlTank(World *world, object_t *tank)
{
    uint8_t joy = readController(tank);

    int16_t dx = 0, dy = 0;

    if (joy & JOY_UP)    dy = -SPEED_Y;
    if (joy & JOY_DOWN)  dy =  SPEED_Y;
    if (joy & JOY_LEFT)  dx = -SPEED_X;
    if (joy & JOY_RIGHT) dx =  SPEED_X;

    if (dx == 0 && dy == 0)
        return;

    int32_t nextX = tank->position_x + dx;
    int32_t nextY = tank->position_y + dy;

    // Point for erasing old tank
    Point oldPos = { tank->position_x >> FP_SCALE, tank->position_y >> FP_SCALE };

    // Convert next position to integer for collision check
    object_t nextPos = *tank;
    nextPos.position_x = nextX >> FP_SCALE;
    nextPos.position_y = nextY >> FP_SCALE;

    // Try full movement
    if (checkWallCollisionAABB(nextPos, world) == COLLISION_NONE) {
        tank->position_x = nextX;
        tank->position_y = nextY;
    }

    // update sprite bits
    uint8_t spriteIndex = 0;

    if (dx == 0 && dy < 0) spriteIndex = 0;
    else if (dx > 0 && dy < 0) spriteIndex = 1;
    else if (dx > 0 && dy == 0) spriteIndex = 2;
    else if (dx > 0 && dy > 0) spriteIndex = 3;
    else if (dx == 0 && dy > 0) spriteIndex = 4;
    else if (dx < 0 && dy > 0) spriteIndex = 5;
    else if (dx < 0 && dy == 0) spriteIndex = 6;
    else if (dx < 0 && dy < 0) spriteIndex = 7;

    tank->c &= ~(0x07 << 4);
    tank->c |= (spriteIndex << 4);

    const char *sprite = selectTankSprite(spriteIndex);

    drawTank(*tank, sprite);
    eraseTankSelective(oldPos, *tank, sprite);
}


int16_t getTankX(const object_t *tank) { return tank->position_x >> FP_SCALE; }
int16_t getTankY(const object_t *tank) { return tank->position_y >> FP_SCALE; }
uint8_t getTankHealth(const object_t *tank) { return (tank->c >> 2) & 0x03; }
uint8_t getTankSpriteIndex(const object_t *tank) { return (tank->c >> 4) & 0x07; }
uint8_t getTankAmmo(const object_t *tank) { return (tank->c >> 8) & 0x03; }