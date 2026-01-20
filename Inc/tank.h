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
const char *selectTankSprite(object_t tank);
