#include "bullet.h"
#include "movement.h"
#include "ansi.h"

void shootBullet(object_t *player, object_t *objecthandler, uint8_t direction, uint8_t bullettype){
	if (getTankCooldown(player) != 0) return;

	// Check ammo
	uint8_t bullets = getTankBullets(player);
	// Consume one bullet
	player->c &= ~(0x07 << 8);
	player->c |= ((bullets - 1) << 8);

	// Reset cooldown to 50 and 300 ms
		player->c &= ~(0x3FF << 17);
		if (player->type == enemy)
		    player->c |= (50 << 17);
		else
		    player->c |= (300 << 17);

	uint8_t i = 0;
	for (; i < 64; i++) {
		if (objecthandler[i].type == empty)
			break;
	}

	objecthandler[i].type = bullet;
	objecthandler[i].c = bullettype; //bullettype: 0 -> homing -- 1 -> normal -- +2 -> ricochet (number of bounces = bullettype - 1)
	objecthandler[i].c |= 0 << 20; //if homing bullet then it picks a target

	switch (direction){
		case bulletUpLeft:
			objecthandler[i].position_x = player->position_x - (TANK_WIDTH << (1 + FP_SCALE));
			objecthandler[i].position_y = player->position_y - (TANK_HEIGHT << (1 + FP_SCALE));
			objecthandler[i].a = -SPEED_X;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletUp:
			objecthandler[i].position_x = player->position_x;
			objecthandler[i].position_y = player->position_y - (TANK_HEIGHT << (1 + FP_SCALE));
			objecthandler[i].a = 0;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletUpRight:
			objecthandler[i].position_x = player->position_x + (TANK_WIDTH << (1 + FP_SCALE));
			objecthandler[i].position_y = player->position_y - (TANK_HEIGHT << (1 + FP_SCALE));
			objecthandler[i].a = +SPEED_X;
			objecthandler[i].b = -SPEED_Y;
			break;
		case bulletLeft:
			objecthandler[i].position_x = player->position_x - (TANK_WIDTH << (1 + FP_SCALE));
			objecthandler[i].position_y = player->position_y;
			objecthandler[i].a = -SPEED_X;
			objecthandler[i].b = 0;
			break;
		case bulletRight:
			objecthandler[i].position_x = player->position_x + (TANK_WIDTH << (1 + FP_SCALE));
			objecthandler[i].position_y = player->position_y;
			objecthandler[i].a = +SPEED_X;
			objecthandler[i].b = 0;
			break;
		case bulletDownLeft:
			objecthandler[i].position_x = player->position_x - (TANK_WIDTH << (1 + FP_SCALE));
			objecthandler[i].position_y = player->position_y + (TANK_HEIGHT << (1 + FP_SCALE));
			objecthandler[i].a = -SPEED_X;
			objecthandler[i].b = +SPEED_Y;
			break;
		case bulletDown:
			objecthandler[i].position_x = player->position_x;
			objecthandler[i].position_y = player->position_y + (TANK_HEIGHT << (1 + FP_SCALE));
			objecthandler[i].a = 0;
			objecthandler[i].b = +SPEED_Y;
			break;
		case bulletDownRight:
			objecthandler[i].position_x = player->position_x + (TANK_WIDTH << (1 + FP_SCALE));
			objecthandler[i].position_y = player->position_y + (TANK_HEIGHT << (1 + FP_SCALE));
			objecthandler[i].a = +SPEED_X;
			objecthandler[i].b = +SPEED_Y;
			break;
	}

	objecthandler[i].a *= 2;
	objecthandler[i].b *= 2;
	int32_t tmpX = objecthandler[i].position_x >> FP_SCALE;
	int32_t tmpY = objecthandler[i].position_y >> FP_SCALE;
}

void updateBullet(object_t *bullet, object_t *objecthandler, World *world){
	object_t localBullet;
	object_t Normalizedobject;
	localBullet.type = bullet->type;
	localBullet.position_x = bullet->position_x >> FP_SCALE;
	localBullet.position_y = bullet->position_y >> FP_SCALE;
	localBullet.a = bullet->a;
	localBullet.b = bullet->b;
	localBullet.c = bullet->c;
	int32_t tmpX = bullet->position_x;
	int32_t tmpY = bullet->position_y;
	Point oldPos = {0};

	for (uint8_t i = 0; i < 64; i++){
		if (objecthandler[i].type == enemy){
			Normalizedobject = objecthandler[i];
			Normalizedobject.position_x = Normalizedobject.position_x >> FP_SCALE;
			Normalizedobject.position_y = Normalizedobject.position_y >> FP_SCALE;
			if(HitboxOverlap(getTankHitbox(Normalizedobject),getBulletHitbox(localBullet))){
				oldPos.x = objecthandler[i].position_x >> FP_SCALE;
				oldPos.y = objecthandler[i].position_y >> FP_SCALE;
				uint8_t health = getTankHealth(&objecthandler[i]); // Health
				health--;
				objecthandler[i].c &= ~(0x03 << 2);
				objecthandler[i].c |= (health << 2);

				if (health == 0) {
					Point tankPos = {
                		objecthandler[i].position_x >> FP_SCALE,
                		objecthandler[i].position_y >> FP_SCALE
            		};
            
					// Create an empty object for eraseTankSelective (nothing to overlap with)
					object_t emptyTank;
					emptyTank.position_x = -100 << FP_SCALE; // Offscreen position
					emptyTank.position_y = -100 << FP_SCALE;
					
					eraseTankSelective(tankPos, emptyTank, "");
					objecthandler[i].type = empty; // Mark player as dead
				}

				gotoxy(tmpX >> FP_SCALE, tmpY >> FP_SCALE);
				printf("%c", 32);
				bullet->type = empty;
				return;
			}
		}
		if (objecthandler[i].type == player){
			Normalizedobject = objecthandler[i];
			Normalizedobject.position_x = Normalizedobject.position_x >> FP_SCALE;
			Normalizedobject.position_y = Normalizedobject.position_y >> FP_SCALE;
			if (HitboxOverlap(getTankHitbox(Normalizedobject),getBulletHitbox(localBullet))){
				//Add erase player function


				uint8_t health = getTankHealth(&objecthandler[i]); // Health
				health--;
				objecthandler[i].c &= ~(0x03 << 2);
				objecthandler[i].c |= (health << 2);

				gotoxy(tmpX >> FP_SCALE, tmpY >> FP_SCALE);
				printf("%c", 32);
				bullet->type = empty;
				return;
			}
		}
	}

	if(bullet->c > 1){
		switch (checkWallCollisionAABB(localBullet, world)){
		case COLLISION_LEFT:
			bullet->a = -bullet->a;
			bullet->c--;
			break;
		case COLLISION_RIGHT:
			bullet->a = -bullet->a;
			bullet->c--;
			break;
		case COLLISION_TOP:
			bullet->b = -bullet->b;
			bullet->c--;
			break;
		case COLLISION_BOTTOM:
			bullet->b = -bullet->b;
			bullet->c--;
			break;
		case COLLISION_NONE:
			break;
		}
	} else if(checkWallCollisionAABB(localBullet, world) != COLLISION_NONE){
		gotoxy(tmpX >> FP_SCALE, tmpY >> FP_SCALE);
		printf("%c", 32);
		bullet->type = empty;
		return;
	}

	if (bullet->c == 0){
		int32_t prevRSquare = 0xFFFFFFFF;
		int32_t rSquare;
		int8_t target;
		for (uint8_t i = 0; i < 64; i++){
			if (objecthandler[i].type != enemy) continue;

			if ((bullet->c != (i >> 20)) && ((bullet->c >> 20) != 0)) continue;

			rSquare = ((objecthandler[i].position_x - bullet->position_x) >> FP_SCALE)*((objecthandler[i].position_x - bullet->position_x) >> FP_SCALE)
					+ ((objecthandler[i].position_y - bullet->position_y) >> FP_SCALE)*((objecthandler[i].position_y - bullet->position_y) >> FP_SCALE);

			if(bullet->c >> 20 == 0){
				if (prevRSquare > rSquare){
					prevRSquare = rSquare;
					target = i;
					if(i != 63) continue;
					bullet->c |= target << 20;
					break;
				}
			}

			bullet->a += (GRAVITY * ((objecthandler[i].position_x - bullet->position_x) >> FP_SCALE))/rSquare;
			bullet->b += (GRAVITY * ((objecthandler[i].position_y - bullet->position_y) >> FP_SCALE))/rSquare;
			while(bullet->a > 1000 || bullet->b > 1000){
				bullet->a = bullet->a >> 2;
				bullet->b = bullet->b >> 2;
			}
		}
	}
	bullet->position_x += bullet->a;
	bullet->position_y += bullet->b;
	gotoxy(tmpX >> FP_SCALE, tmpY >> FP_SCALE);
	printf("%c", 32);
	gotoxy(bullet->position_x >> FP_SCALE, bullet->position_y >> FP_SCALE);
	fgcolor(15);
	printf("o");
}
