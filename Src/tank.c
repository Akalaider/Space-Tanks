#include "30010_io.h" // Input/output library for this course
#include "tank.h"

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
    // Convert from fixed-point to integer coordinates
    int16_t x = tank.position_x >> FP_SCALE;
    int16_t y = tank.position_y >> FP_SCALE;

    uint8_t row = 0;
    uint8_t col = 0;
    fgcolor(10);
    for (uint8_t i = 0; sprite[i] != '\0'; i++) {

        if (sprite[i] == '\n') {
            row++;
            col = 0;
            continue;
        }

        gotoxy(x + col - (TANK_WIDTH >> 1), y + row - (TANK_HEIGHT >> 1));
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


// Choose sprite based on direction (stored in position_x and position_y of the object)
const char *selectTankSprite(object_t direction)
{
    if (direction.position_x == 0 && direction.position_y == 0) return getTankUp();
    
    int16_t absX = direction.position_x < 0 ? -direction.position_x : direction.position_x;
    int16_t absY = direction.position_y < 0 ? -direction.position_y : direction.position_y;
    
    // Determine primary direction based on which component is larger    
    // If absX is larger it's horizontal
    // If absY is larger it's vertical  
    // Otherwise it's diagonal
    
    // absX > 2*absY means very horizontal (within ±22.5° of horizontal)
    // absY > 2*absX means very vertical (within ±22.5° of vertical)
    
    if (direction.position_y < 0) {  // Upper half
        if (direction.position_x > 0) {  // Upper right quadrant
            if (absX > absY * 2) return getTankRight();
            if (absY > absX * 2) return getTankUp();
            return getTankUpRight();
        } else if (direction.position_x < 0) {  // Upper left quadrant
            if (absX > absY * 2) return getTankLeft();
            if (absY > absX * 2) return getTankUp();
            return getTankUpLeft();
        } else {
            return getTankUp();
        }
    } else {  // Lower half
        if (direction.position_x > 0) {  // Lower right quadrant
            if (absX > absY * 2) return getTankRight();
            if (absY > absX * 2) return getTankDown();
            return getTankDownRight();
        } else if (direction.position_x < 0) {  // Lower left quadrant
            if (absX > absY * 2) return getTankLeft();
            if (absY > absX * 2) return getTankDown();
            return getTankDownLeft();
        } else {
            return getTankDown();
        }
    }
}