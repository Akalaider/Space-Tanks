#include "powerup.h"

void spawnPowerup(object_t* enemy, object_t *objecthandler){
	enemy->type = powerup;
	enemy->a = 0; // needs to be changed
}
void updatePowerup(object_t* powerUp, object_t *objecthandler, World world){
	object_t localPowerup;
	localPowerup.position_x = powerUp->position_x;
	localPowerup.position_y = powerUp->position_y;
	localPowerup.a = powerUp->a;
	localPowerup.b = powerUp->b;
	localPowerup.c = powerUp->c;

	for (uint8_t i; i < 64; i++){
		if (objecthandler[i].type != player) continue;
		if (objecthandler[i].type == enemy && HitboxOverlap(getTankHitbox(objecthandler[i]),getBulletHitbox(localPowerup))){
			//add erase powerup sprite
			powerUp->type = empty;
			return;
		}
	}
}
