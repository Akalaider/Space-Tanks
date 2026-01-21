/*
 * powerup.h
 *
 *  Created on: 21. jan. 2026
 *      Author: adambenali
 */

#ifndef POWERUP_H_
#define POWERUP_H_
#include "object.h"
#include "world.h"

void spawnPowerup(object_t* enemy, object_t *objecthandler);
void updatePowerup(object_t* powerUp, object_t *objecthandler, World world);

#endif /* POWERUP_H_ */
