#include "30010_io.h" // Input/output library for this course
#include "world.h"
#include "ansi.h"
#include "art.h"
#include "tank.h"

void drawTank(object_t tank, const char *sprite)
{
    int16_t x = tank.position_x;
    int16_t y = tank.position_y;

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
    for (uint16_t row = 0; row < TANK_HEIGHT; row++) {
        for (uint16_t col = 0; col < TANK_WIDTH; col++) {
            gotoxy(pos.x + col - TANK_WIDTH << 1, pos.y + row - TANK_HEIGHT << 1);
            printf(" ");
        }
    }
}

// Choose sprite based on velocity
const char *selectTankSprite(object_t tank)
{
    if (tank.position_x == 0 && tank.position_y == 0) return getTankUp;
    
    int16_t absX = tank.position_x < 0 ? -tank.position_x : tank.position_x;
    int16_t absY = tank.position_y < 0 ? -tank.position_y : tank.position_y;
    
    // Determine primary direction based on which component is larger    
    // If absX is larger it's horizontal
    // If absY is larger it's vertical  
    // Otherwise it's diagonal
    
    // absX > 2*absY means very horizontal (within ±22.5° of horizontal)
    // absY > 2*absX means very vertical (within ±22.5° of vertical)
    
    if (tank.position_y < 0) {  // Upper half
        if (tank.position_x > 0) {  // Upper right quadrant
            if (absX > absY * 2) return getTankRight();
            if (absY > absX * 2) return getTankUp();
            return getTankUpRight();
        } else if (tank.position_x < 0) {  // Upper left quadrant
            if (absX > absY * 2) return getTankLeft();
            if (absY > absX * 2) return getTankUp();
            return getTankUpLeft();
        } else {
            return getTankUp();
        }
    } else {  // Lower half
        if (tank.position_x > 0) {  // Lower right quadrant
            if (absX > absY * 2) return getTankRight();
            if (absY > absX * 2) return getTankDown();
            return getTankDownRight();
        } else if (tank.position_x < 0) {  // Lower left quadrant
            if (absX > absY * 2) return getTankLeft();
            if (absY > absX * 2) return getTankDown();
            return getTankDownLeft();
        } else {
            return getTankDown();
        }
    }
}