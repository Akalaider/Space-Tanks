#include "level2.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "art.h"   // <- vigtigt for getAsteroid()
#include "aimovement.h"

ai_t enemy1;
void level2(void) {
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

    Point innerWall1[] = {
        {25, 10},
    	{25, 20},
    	{45, 20},
        {45, 30},
        {65, 30}
    };
    drawWalls(innerWall1, 5, 0, &world);

    Point innerWall2[] = {
    		{185, 10},
			{185, 20},
			{165, 20},
			{165, 30},
			{145, 30}
    };
    drawWalls(innerWall2, 5, 0, &world);

    Point innerWall3[] = {
    		{65, 40},
			{45, 40},
			{45, 50},
			{25, 50},
			{25, 60}
    };
    drawWalls(innerWall3, 5, 0, &world);

    Point innerWall4[] = {
    		{145, 40},
			{165, 40},
			{165, 50},
			{185, 50},
			{185, 60}
    };

    drawWalls(innerWall4, 5, 0, &world);

    drawObstacle((Point){95, 32}, getAsteroid3(), 20, 4, &world);
    drawObstacle((Point){130, 10}, getAsteroid6(), 17, 8, &world);
    drawObstacle((Point){60, 15}, getAsteroid4(), 20, 9, &world);
    drawObstacle((Point){57, 50}, getAsteroid2(), 14, 9, &world);
    drawObstacle((Point){125, 48}, getAsteroid(), 21, 7, &world);

    fgcolor(8);
   // printCp850At(90, 15, getStar1);
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
