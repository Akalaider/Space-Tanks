#include "30010_io.h" // Input/output library for this course
#include "world.h"
#include "ansi.h"
#include "art.h"

#define TANK_WIDTH 8
#define TANK_HEIGHT 5
#define TANK_RADIUS 4   // square hitbox

void drawTank(Point pos, const char *sprite);
void eraseTank(Point pos);
const char *selectTankSprite(Point vel);