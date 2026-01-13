#include "30010_io.h" // Input/output library for this course
#include "world.h"
#include "ansi.h"
#include "art.h"
#include "tank.h"

void drawTank(Point pos, const char *sprite)
{
    uint8_t x = pos.x;
    uint8_t y = pos.y;

    uint8_t row = 0;
    uint8_t col = 0;

    for (uint8_t i = 0; sprite[i] != '\0'; i++) {

        if (sprite[i] == '\n') {
            row++;
            col = 0;
            continue;
        }

        gotoxy(x + col - TANK_WIDTH/2, y + row - TANK_HEIGHT/2);
        printf("%c", sprite[i]);
        col++;
    }
}


void eraseTank(Point pos)
{
    for (uint8_t row = 0; row < TANK_HEIGHT; row++) {
        for (uint8_t col = 0; col < TANK_WIDTH; col++) {
            gotoxy(pos.x + col - TANK_WIDTH/2, pos.y + row - TANK_HEIGHT/2);
            printf(" ");
        }
    }
}

// Choose sprite based on velocity
const char *selectTankSprite(Point vel)
{
    if (vel.x == 0 && vel.y < 0) return tankUp;
    if (vel.x == 0 && vel.y > 0) return tankDown;
    if (vel.x > 0 && vel.y == 0) return tankRight;
    if (vel.x < 0 && vel.y == 0) return tankLeft;

    if (vel.x > 0 && vel.y < 0) return tankUpRight;
    if (vel.x < 0 && vel.y < 0) return tankUpLeft;
    if (vel.x > 0 && vel.y > 0) return tankDownRight;
    if (vel.x < 0 && vel.y > 0) return tankDownLeft;

    return tankUp;
}