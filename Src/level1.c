#include "level1.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"


void level1(void) {
    clrscr();
    
    // Draw game arena
    Point outerWall[] = {
        {1, 1},
        {239, 1},
        {239, 71},
        {1, 71}
    };
    
    World world = {0};
    
    drawWalls(outerWall, 4, 1, &world);
    drawObstacle((Point){30, 40}, getArrowRight(), 10, 8, &world);
        
    initTank();
    // Game loop
    while (1) {
       controlTank(&world);
    }
}