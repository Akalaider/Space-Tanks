/*
 * bullet.h
 *
 *  Created on: 20. jan. 2026
 *      Author: adambenali
 */

#ifndef BULLET_H_
#define BULLET_H_
#include "object.h"
#include "world.h"
#include "movement.h"
#include "tank.h"

#define bulletUpLeft 0
#define bulletUp 1
#define bulletUpRight 2
#define bulletLeft 3
#define bulletRight 4
#define bulletDownLeft 5
#define bulletDown 6
#define bulletDownRight 7


void shootBullet(object_t player, object_t *objecthandler, uint8_t direction);
void updateBullet(object_t bullet, object_t *objecthandler, World world);

#endif /* BULLET_H_ */
