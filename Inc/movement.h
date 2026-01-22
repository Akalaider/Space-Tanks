#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "world.h"
#include "object.h"
#include "tank.h"
#include "io.h"
#include "bullet.h"
// Fixed‑point scale factor (128 = 7 bits)
#define FP_SCALE 7

// Tunable speeds

#define SPEED_X 400 // horizontal speed
#define SPEED_Y 240 // vertical speed (rows are taller)


#define DIAG_SCALE 91 // ≈ 1/sqrt(2) * 128

void setTankBullets(object_t *tank, uint8_t bullets);
void initTank(object_t *tank);
void controlTank(object_t *objecthandler, World *world, object_t *tank, char *buf);
uint8_t readController(const object_t *tank, char *buf);
int16_t getTankX(const object_t *tank);
int16_t getTankY(const object_t *tank);
uint8_t getPlayer(const object_t *tank);
uint8_t getTankHealth(const object_t *tank);
uint8_t getTankSpriteIndex(const object_t *tank);
uint8_t getTankBullets(const object_t *tank);
uint8_t getTankHomings(const object_t *tank);
uint8_t getTankPowerup(const object_t *tank);
uint16_t getTankCooldown(const object_t *tank);

#endif
