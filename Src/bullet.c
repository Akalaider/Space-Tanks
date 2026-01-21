/*
 * bullet.c
 *
 *  Created on: 20. jan. 2026
 *      Author: adambenali
 */
#include "bullet.h"
#include "movement.h"

void shootBullet(object_t player, object_t *objecthandler, uint8_t direction, uint8_t bullettype){
	uint8_t i = 0;
	for (; i<64;i++){
		if(objecthandler->type == empty) break;
	}

	objecthandler[i].type = bullet;
	objecthandler[i].c = bullettype; //bullettype: 0 -> homing -- 1 -> normal -- +2 -> ricochet (number of bounces = bullettype - 1)

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
	if (objecthandler[i].a != 0 && objecthandler[i].b != 0){
		objecthandler[i].position_x = (objecthandler[i].position_x*DIAG_SCALE) >> FP_SCALE;
		objecthandler[i].position_y = (objecthandler[i].position_y*DIAG_SCALE) >> FP_SCALE;
		objecthandler[i].a = (objecthandler[i].a*DIAG_SCALE) >> FP_SCALE;
		objecthandler[i].b = (objecthandler[i].b*DIAG_SCALE) >> FP_SCALE;
	}
	objecthandler[i].a *= 2;
	objecthandler[i].b *= 2;
}

void updateBullet(object_t bullet, object_t *objecthandler, World world){
	int32_t tmpX = bullet.position_x;
	int32_t tmpY = bullet.position_y;
	Point oldPos = {0};

	for (uint8_t i; i < 64; i++){
		if (objecthandler[i].type != player || objecthandler[i].type != enemy) continue;
		if (objecthandler[i].type == enemy && HitboxOverlap(getTankHitbox(objecthandler[i]),getBulletHitbox(bullet))){
			oldPos.x = objecthandler[i].position_x >> FP_SCALE;
			oldPos.y = objecthandler[i].position_y >> FP_SCALE;
			eraseTankSelective(oldPos, objecthandler[i], selectTankSprite((objecthandler[i].c & (0x07 << 4)) >> 4));
			objecthandler[i].type = empty;
		}
		if (objecthandler[i].type == player && HitboxOverlap(getTankHitbox(objecthandler[i]),getBulletHitbox(bullet))){
			//Add erase player function
			objecthandler[i].type = empty;
		}
		gotoxy(tmpX, tmpY);
		printf("%c", 32);
		bullet.type = empty;
		return;
	}

		if(bullet.c > 1){
			switch (checkWallCollisionAABB(bullet, &world)){
			case COLLISION_LEFT:
				bullet.a = -bullet.a;
				bullet.c--;
				break;
			case COLLISION_RIGHT:
				bullet.a = -bullet.a;
				bullet.c--;
				break;
			case COLLISION_TOP:
				bullet.b = -bullet.b;
				bullet.c--;
				break;
			case COLLISION_BOTTOM:
				bullet.b = -bullet.b;
				bullet.c--;
				break;
			case COLLISION_NONE:
				break;
			}
		} else if(checkWallCollisionAABB(bullet, &world) != COLLISION_NONE){
			gotoxy(tmpX, tmpY);
			printf("%c", 32);
			bullet.type = empty;
			return;
		}

	if (bullet.c == 0){
		for (uint8_t i; i < 64; i++){
			if (objecthandler[i].type != enemy) continue;
			if ((bullet.position_x < objecthandler[i].position_x && bullet.position_x > objecthandler[i].position_x - DISTANCE*TANK_WIDTH)
				||(bullet.position_x > objecthandler[i].position_x && bullet.position_x < objecthandler[i].position_x + DISTANCE*TANK_WIDTH)
				||(bullet.position_y < objecthandler[i].position_y && bullet.position_x > objecthandler[i].position_y - DISTANCE*TANK_HEIGHT)
				||(bullet.position_y > objecthandler[i].position_y && bullet.position_x < objecthandler[i].position_y + DISTANCE*TANK_HEIGHT)){
				//test algorithm
				bullet.a += (GRAVITY/(objecthandler[i].position_x - bullet.position_x) << FP_SCALE);
				bullet.b += (GRAVITY/(objecthandler[i].position_y - bullet.position_y) << FP_SCALE);
			}
		}
	}
	bullet.position_x += bullet.a >> FP_SCALE;
	bullet.position_y += bullet.b >> FP_SCALE;
	gotoxy(tmpX, tmpY);
	printf("%c", 32);
	gotoxy(bullet.position_x, bullet.position_y);
	printf("o");
}
