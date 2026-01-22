#include "movement.h"

void setTankBullets(object_t *tank, uint8_t bullets) {
    tank->c &= ~(0x07 << 8);        // clear bits
    tank->c |= ((bullets & 0x07) << 8);
}


void initTank(object_t *tank)
{
    tank->type = player;
    tank->position_x = 10 << FP_SCALE;
    tank->position_y = 61 << FP_SCALE;
    tank->a = TANK_WIDTH;
    tank->b = TANK_HEIGHT;

    tank->c = 0;                 // clear all bits
    tank->c |= HAT_CONTROL;      // control mode
    tank->c |= (3 << 2);         // health = 3
    tank->c |= (0 << 4);         // sprite index = 0
    tank->c |= (5 << 8);         // Set bullets to 5
    tank->c |= (3 << 11);        // Set homing bullets to 3
    tank->c |= (0 << 14);		 // PowerupMode: bx00 -> no powerUp -- bxX1 -> ricochet bullet -- bx1X -> speedBoost
    tank->c &= ~(0x3FF << 17);   // clear value 
    tank->c |= (500 << 17);      // store cooldown value

    const char *sprite = selectTankSprite(getTankSpriteIndex(tank));
    drawTank(*tank, sprite);
}


uint8_t readController(const object_t *tank, char *buf){
    if (charInString(buf, 'p'))
        return JOY_PAUSE;

    switch (tank->c & 0x03) {
		case JOYSTICK_CONTROL:
			return readJoystick();
		case KEYBOARD_CONTROL:
			return readKeysFromBuffer(buf);
		case HAT_CONTROL:
			return readHat();
		default:
			return readHat();
	}
}


void controlTank(object_t *objecthandler, World *world, object_t *tank, char *buf)
{
    uint8_t joy = readController(tank, buf);

    int16_t dx = 0, dy = 0;

    if (joy & JOY_UP)    dy = -SPEED_Y;
    if (joy & JOY_DOWN)  dy =  SPEED_Y;
    if (joy & JOY_LEFT)  dx = -SPEED_X;
    if (joy & JOY_RIGHT) dx =  SPEED_X;


    // Note: Use index of sprite determine direction
    if(joy & JOY_CENTER){
    	if((getTankPowerup(tank) & 1) == 0){
    		shootBullet(tank, objecthandler, (tank->c & (0x07 << 4)) >> 4, 2);
    	} else {
    		shootBullet(tank, objecthandler, (tank->c & (0x07 << 4)) >> 4, 3);
    	}
    }
    if(joy & JOY_BUTTON1){
        shootBullet(tank, objecthandler, (tank->c & (0x07 << 4)) >> 4, 0);
    }

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

    if (dx == 0 && dy < 0) spriteIndex = 0; //UP
    else if (dx > 0 && dy < 0) spriteIndex = 1; //RIGHT&UP
    else if (dx > 0 && dy == 0) spriteIndex = 2; //RIGHT
    else if (dx > 0 && dy > 0) spriteIndex = 3; //RIGHT&DOWN
    else if (dx == 0 && dy > 0) spriteIndex = 4; //DOWN
    else if (dx < 0 && dy > 0) spriteIndex = 5; //LEFT&DOWN
    else if (dx < 0 && dy == 0) spriteIndex = 6; //LEFT
    else if (dx < 0 && dy < 0) spriteIndex = 7; //LEFT&UP

    tank->c &= ~(0x07 << 4);
    tank->c |= (spriteIndex << 4);

    const char *sprite = selectTankSprite(spriteIndex);

    drawTank(*tank, sprite);
    eraseTankSelective(oldPos, *tank, sprite);
}


int16_t getTankX(const object_t *tank) { return tank->position_x >> FP_SCALE; }
int16_t getTankY(const object_t *tank) { return tank->position_y >> FP_SCALE; }
uint8_t getPlayer(const object_t *tank) { return (tank->c) & 0x03; }
uint8_t getTankHealth(const object_t *tank) { return (tank->c >> 2) & 0x03; }
uint8_t getTankSpriteIndex(const object_t *tank) { return (tank->c >> 4) & 0x07; }
uint8_t getTankBullets(const object_t *tank) { return (tank->c >> 8) & 0x07; }
uint8_t getTankHomings(const object_t *tank) { return (tank->c >> 11) & 0x07; }
uint8_t getTankPowerup(const object_t *tank) { return (tank->c >> 14) & 0x03; }
uint16_t getTankCooldown(const object_t *tank) { return (tank->c >> 17) & 0x3FF; }
