#include "level1.h"
<<<<<<< HEAD
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "aimovement.h"
=======
>>>>>>> c7778283e15d4e0370bff9bb7da1a58e218cc7a6

ai_t enemy1;
void level1(void) {
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
<<<<<<< HEAD

    initTank();
    initAITank(&enemy1);
=======
        
    initTank();
>>>>>>> c7778283e15d4e0370bff9bb7da1a58e218cc7a6
    setTankUpdateInterval(50); // 10 ms → 100 Hz
    // Game loop
    while (1) {
        if (tankUpdateDue()) {
            controlTank(&world);
<<<<<<< HEAD
            controlAITank(&enemy1, &world);
=======
>>>>>>> c7778283e15d4e0370bff9bb7da1a58e218cc7a6
        }
    }
}