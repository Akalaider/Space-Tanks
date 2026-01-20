#include "level3.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "art.h"   // <- vigtigt for getAsteroid()
#include "aimovement.h"

ai_t enemy1;
void level3(void) {
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
        {25, 20},
		{25, 10},
    	{85, 10}
    };
    drawWalls(innerWall1, 6, 0, &world);

    Point innerWall2[] = {
    		{105, 10},
			{105, 40},
    };
    drawWalls(innerWall2, 2, 0, &world);

    Point innerWall3[] = {
    		{105,25},
			{130,25}
    };
    drawWalls(innerWall3, 2, 0, &world);

    Point innerWall4[] = {
    		{25, 40},
			{85, 40}
    };

    drawWalls(innerWall4, 2, 0, &world);

    Point innerWall5[] = {
    		{55, 40},
			{55, 55}
    };

    drawWalls(innerWall5, 2, 0, &world);

    Point innerWall6[] = {
    		{105, 50},
			{155, 50}
    };

    drawWalls(innerWall6, 2, 0, &world);

    Point innerWall7[] = {
    		{135, 35},
			{135, 50}
    };

    drawWalls(innerWall7, 2, 0, &world);

    Point innerWall8[] = {
    		{155, 10},
			{155, 35},
			{185, 35},
			{185, 50}
    };

    drawWalls(innerWall8, 4, 0, &world);

    drawObstacle((Point){123, 60}, getAsteroid3(), 20, 4, &world);
    drawObstacle((Point){120, 10}, getAsteroid6(), 17, 8, &world);
    drawObstacle((Point){52, 20}, getAsteroid4(), 20, 9, &world);
    drawObstacle((Point){30, 53}, getAsteroid2(), 14, 9, &world);
    drawObstacle((Point){177, 14}, getAsteroid(), 21, 7, &world);



    initTank();
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
