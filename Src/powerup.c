/*
 * powerup.c
 *
 *  Created on: 21. jan. 2026
 *      Author: adambenali
 */
#include "powerup.h"

void spawnPowerup(object_t* enemy, object_t *objecthandler){
	enemy->type = powerup;
	enemy->a = 0; // needs to be changed
}
void updatePowerup(object_t* powerUp, object_t *objecthandler, World world){
	for (uint8_t i; i < 64; i++){
		if (objecthandler[i].type != player) continue;
		if (objecthandler[i].type == enemy && HitboxOverlap(getTankHitbox(objecthandler[i]),getBulletHitbox(powerUp))){
			//add erase powerup sprite
			powerUp.type = empty;
			return;
		}
}
