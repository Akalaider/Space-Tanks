#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "world.h"
#include "object.h"
#include "tank.h"
#include "io.h"

void initTank(object_t *tank);
void controlTank(World *world, object_t *tank);

uint8_t readController(const object_t *tank);

int16_t getTankX(const object_t *tank);
int16_t getTankY(const object_t *tank);
int16_t getTankHealth(const object_t *tank);
uint8_t getTankSpriteIndex(const object_t *tank);

#endif
