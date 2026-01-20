#include "level1.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "aimovement.h"


ai_t enemy1;
void level1(void) {
    push_health();
    clrscr();
    
    // Draw game arena
    Point outerWall[] = {
        {0, 0},
        {240, 0},
        {240, 72},
        {0, 72}
    };
    
    World world = {0};
    
    drawWalls(outerWall, 4, 1, &world);
    fgcolor(1);
    drawObstacle((Point){30, 40}, getArrowRight(), 10, 8, &world);
    fgcolor(15);

    initAITank(&enemy1);

        
    initTank();

    setTankUpdateInterval(50); // 10 ms → 100 Hz
    // Game loop
    while (1) {
        if (tankUpdateDue()) {
            controlTank(&world);
            controlAITank(&enemy1, &world);

        }
    }
}
