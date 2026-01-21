#include "bullet.h"
#define bulletUpLeft 0
#define bulletUp 1
#define bulletUpRight 2
#define bulletLeft 3
#define bulletRight 4
#define bulletDownLeft 5
#define bulletDown 6
#define bulletDownRight 7

void shootBullet(object_t player, object_t *objecthandler, uint8_t direction){
	uint8_t i = 0;
	for (; i<64;i++){
		if(objecthandler->type == empty) break;
	}

	objecthandler[i].type = bullet;
	switch (direction){
		case bulletUpLeft:
			objecthandler[i].position_x = player.position_x - TANK_WIDTH;
			objecthandler[i].position_y = player.position_y - TANK_HEIGHT;
			objecthandler[i].a = -SPEED_X;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletUp:
			objecthandler[i].position_x = player.position_x;
			objecthandler[i].position_y = player.position_y - TANK_HEIGHT;
			objecthandler[i].a = 0;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletUpRight:
			objecthandler[i].position_x = player.position_x + TANK_WIDTH;
			objecthandler[i].position_y = player.position_y - TANK_HEIGHT;
			objecthandler[i].a = +SPEED_X;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletLeft:
			objecthandler[i].position_x = player.position_x + TANK_WIDTH;
			objecthandler[i].position_y = player.position_y;
			objecthandler[i].a = -SPEED_X;
			objecthandler[i].b = 0;
			break;
		case bulletRight:
			objecthandler[i].position_x = player.position_x + TANK_WIDTH;
			objecthandler[i].position_y = player.position_y;
			objecthandler[i].a = +SPEED_X;
			objecthandler[i].b = 0;
			break;
		case bulletDownLeft:
			objecthandler[i].position_x = player.position_x - TANK_WIDTH;
			objecthandler[i].position_y = player.position_y + TANK_HEIGHT;
			objecthandler[i].a = -SPEED_X;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletDown:
			objecthandler[i].position_x = player.position_x;
			objecthandler[i].position_y = player.position_y + TANK_HEIGHT;
			objecthandler[i].a = 0;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletDownRight:
			objecthandler[i].position_x = player.position_x + TANK_WIDTH;
			objecthandler[i].position_y = player.position_y + TANK_HEIGHT;
			objecthandler[i].a = +SPEED_X;
			objecthandler[i].b = -SPEED_Y;
			break;
	}
	//normalize diagonals
	if (objecthandler[i].a =! 0 && objecthandler[i].b =! 0){
		objecthandler[i].position_x = (objecthandler[i].position_x*DIAG_SCALE) >> FP_SCALE;
		objecthandler[i].position_y = (objecthandler[i].position_y*DIAG_SCALE) >> FP_SCALE;
		objecthandler[i].a = (objecthandler[i].a*DIAG_SCALE) >> FP_SCALE;
		objecthandler[i].b = (objecthandler[i].b*DIAG_SCALE) >> FP_SCALE;
	}
}

void updateBullet(object_t bullet, object_t *objecthandler, World world){
	int32_t tmpX = bullet.position_x;
	int32_t tmpY = bullet.position_y;

	for (uint8_t i; i < 64; i++){
		if (objecthandler[i].type != player || objecthandler[i].type != enemy) continue;
		if (objecthandler[i].type == enemy && "enemycollion"){
			//Add erase enemy function
			objecthandler[i].type = empty;
		}
		if (objecthandler[i].type == enemy && "playercollion"){
			//Add erase player function
			objecthandler[i].type = empty;
		}
		gotoxy(tmpX, tmpY);
		printf("%c", 32);
		bullet.type = empty;
		return;
	}

	if("wall collision"){
		if("reco bullet"){
			switch (0){
			case HORZ:
				bullet.a = -bullet.a;
			case VERT:
				bullet.b = -bullet.b;
			}
		} else {
			gotoxy(tmpX, tmpY);
			printf("%c", 32);
			bullet.type = empty;
			return;
		}
	}

	if ("homing bullet"){
		for (uint8_t i; i < 64; i++){
			if (objecthandler[i].type != enemy) continue;
			if ((bullet.position_x < objecthandler[i].position_x && bullet.position_x > objecthandler[i].position_x - DISTANCE)
				||(bullet.position_x > objecthandler[i].position_x && bullet.position_x < objecthandler[i].position_x + DISTANCE)
				||(bullet.position_y < objecthandler[i].position_y && bullet.position_x > objecthandler[i].position_y - DISTANCE)
				||(bullet.position_y > objecthandler[i].position_y && bullet.position_x < objecthandler[i].position_y + DISTANCE)){
				//test algorithm
				bullet.a += ((objecthandler[i].position_x - bullet.position_x) >> 0);
				bullet.b += ((objecthandler[i].position_y - bullet.position_y) >> 0);
			}
		}
	}
	bullet.position_x += bullet.a;
	bullet.position_y += bullet.b;
	gotoxy(tmpX, tmpY);
	printf("%c", 32);
	gotoxy(bullet.position_x, bullet.position_y);
	printf("o");
}
