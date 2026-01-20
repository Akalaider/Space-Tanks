#include "30010_io.h" // Input/output library for this course
#include "world.h"
#include "ansi.h"
#include "art.h"
#include "object.h"

#define TANK_WIDTH 8
#define TANK_HEIGHT 5
#define TANK_RADIUS 4   // square hitbox

void drawTank(object_t tank, const char *sprite);
void eraseTankSelective(Point oldPos, object_t newTank, const char *newSprite);
<<<<<<< HEAD
const char *selectTankSprite(object_t tank);
=======
const char *selectTankSprite(object_t tank);
>>>>>>> c7778283e15d4e0370bff9bb7da1a58e218cc7a6
