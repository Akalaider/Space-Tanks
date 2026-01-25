#ifndef BULLET_H_
#define BULLET_H_
#include "object.h"
#include "world.h"
#include "tank.h"

#define bulletUpLeft 7
#define bulletUp 0
#define bulletUpRight 1
#define bulletLeft 6
#define bulletRight 2
#define bulletDownLeft 5
#define bulletDown 4
#define bulletDownRight 3
#define GRAVITY 10000
#define DISTANCE 2


void shootBullet(object_t *player, object_t *objecthandler, uint8_t direction, uint8_t bullettype);
void updateBullet(object_t *bullet, object_t *objecthandler, World *world);

#endif /* BULLET_H_ */
