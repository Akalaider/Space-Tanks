#include "level1.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "aimovement.h"


ai_t enemy1;
void level1(void) {
    //push_health();
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
    drawObstacle((Point){150, 45}, getAsteroid(), 21, 7, &world);

    initTank();
        //initAITank();

    drawObstacle((Point){40, 15}, getOumuamua(), 8, 9, &world);


    drawObstacle((Point){100, 35}, getAsteroid2(), 14, 9, &world);


    drawObstacle((Point){155, 20}, getAsteroid3(), 20, 4, &world);


    drawObstacle((Point){45, 45}, getAsteroid4(), 20, 9, &world);


    drawObstacle((Point){90, 7}, getAsteroid5(), 15, 8, &world);

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
