#include "30010_io.h" // Input/output library for this course
#include "world.h"
#include "ansi.h"
#include "art.h"
#include "tank.h"

void drawTank(Point pos, const char *sprite)
{
    int16_t x = pos.x;
    int16_t y = pos.y;

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
            gotoxy(pos.x + col - TANK_WIDTH/2, pos.y + row - TANK_HEIGHT/2);
            printf(" ");
        }
    }
}

// Choose sprite based on velocity
const char *selectTankSprite(Point vel)
{
    if (vel.x == 0 && vel.y == 0) return tankUp;
    
    int16_t absX = vel.x < 0 ? -vel.x : vel.x;
    int16_t absY = vel.y < 0 ? -vel.y : vel.y;
    
    // Determine primary direction based on which component is larger
    // Use 8 sectors: each 45° wide
    
    // If absX is much larger, it's horizontal
    // If absY is much larger, it's vertical  
    // Otherwise it's diagonal
    
    // absX > 2*absY means very horizontal (within ±22.5° of horizontal)
    // absY > 2*absX means very vertical (within ±22.5° of vertical)
    
    if (vel.y < 0) {  // Upper half
        if (vel.x > 0) {  // Upper right quadrant
            if (absX > absY * 2) return tankRight;
            if (absY > absX * 2) return tankUp;
            return tankUpRight;
        } else if (vel.x < 0) {  // Upper left quadrant
            if (absX > absY * 2) return tankLeft;
            if (absY > absX * 2) return tankUp;
            return tankUpLeft;
        } else {
            return tankUp;
        }
    } else {  // Lower half
        if (vel.x > 0) {  // Lower right quadrant
            if (absX > absY * 2) return tankRight;
            if (absY > absX * 2) return tankDown;
            return tankDownRight;
        } else if (vel.x < 0) {  // Lower left quadrant
            if (absX > absY * 2) return tankLeft;
            if (absY > absX * 2) return tankDown;
            return tankDownLeft;
        } else {
            return tankDown;
        }
    }
}