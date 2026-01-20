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
        {0, 0},
        {240, 0},
        {240, 72},
        {0, 72}
    };
    
    World world = {0};
    
    drawWalls(outerWall, 4, 1, &world);

    object_t objecthandler[OBJECTHANDLER_SIZE];
    initObjecthandler(&objecthandler);

    initTank(&objecthandler[0]);
    push_health(&objecthandler[0]);
    
    setTankUpdateInterval(50); // 10 ms → 100 Hz
    // Game loop
    while (1) {
        if (tankUpdateDue()) {
            updateObject(&objecthandler, &world);
        }
    }
}
