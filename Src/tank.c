#include "30010_io.h" // Input/output library for this course
#include "tank.h"
#include "movement.h"

#define FP_SCALE 7  // Must match the value in movement.c


static uint8_t spriteCovers(const char *sprite, int16_t sx, int16_t sy,
                         int16_t px, int16_t py)
{
    uint8_t row = 0;
    uint8_t col = 0;

    for (uint16_t i = 0; sprite[i] != '\0'; i++) {
        if (sprite[i] == '\n') {
            row++;
            col = 0;
            continue;
        }

        int16_t tx = sx + col - (TANK_WIDTH >> 1);
        int16_t ty = sy + row - (TANK_HEIGHT >> 1);

        if (tx == px && ty == py)
            return 1;

        col++;
    }
    return 0;
}

void drawTank(object_t tank, const char *sprite)
{
    int16_t x = tank.position_x >> FP_SCALE;
    int16_t y = tank.position_y >> FP_SCALE;

    uint8_t row = 0;
    uint8_t col = 0;

    switch (tank.type) {
        case player: {
            int p = getPlayer(&tank);
            if (p == 2) {
                fgcolor(12);
            } else {
                fgcolor(10);
            }
            break;
        }

        case enemy:
            fgcolor(9);
            break;
    }


    for (uint16_t i = 0; sprite[i] != '\0'; i++) {

        if (sprite[i] == '\n') {
            row++;
            col = 0;
            continue;
        }

        gotoxy(x + col - (TANK_WIDTH >> 1),
               y + row - (TANK_HEIGHT >> 1));

        printf("%c", sprite[i]);
        col++;
    }

    fgcolor(15);
}



void eraseTankSelective(Point oldPos, object_t newTank, const char *newSprite)
{
    int16_t newX = newTank.position_x >> FP_SCALE;
    int16_t newY = newTank.position_y >> FP_SCALE;

    for (uint16_t row = 0; row < TANK_HEIGHT; row++) {
        for (uint16_t col = 0; col < TANK_WIDTH; col++) {

            int16_t px = oldPos.x + col - (TANK_WIDTH >> 1);
            int16_t py = oldPos.y + row - (TANK_HEIGHT >> 1);

            // Skip erasing if the new tank already drew here
            if (spriteCovers(newSprite, newX, newY, px, py))
                continue;

            gotoxy(px, py);
            printf(" ");
        }
    }
}

// Select sprite based on 3-bit sprite index stored in tank.c bits 5–7
const char *selectTankSprite(uint8_t spriteIndex)
{
    spriteIndex &= 0x07;

    switch (spriteIndex) {
        case 0: return getTankUp();
        case 1: return getTankUpRight();
        case 2: return getTankRight();
        case 3: return getTankDownRight();
        case 4: return getTankDown();
        case 5: return getTankDownLeft();
        case 6: return getTankLeft();
        case 7: return getTankUpLeft();
        default: return getTankUp();
    }
}