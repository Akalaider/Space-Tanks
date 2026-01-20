#include "level2.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "timer.h"
static object_t playerTank;

typedef struct {
    Point pos;
    Point size;
    const char *(*sprite)(void);
} Star;

// Liste af stjerner i level 1
static Star stars[] = {
    { { 5, 10 }, {5, 3}, getStar1 },
    { { 35, 12 }, { 8, 5 }, getStar2 },
    { { 50, 35 }, { 3, 3 }, getStar3 },
    { { 100, 20 }, { 10, 7 }, getStar4 },
	{ { 90, 30 }, { 5, 3 }, getStar1 },
	{ { 200, 60 }, { 3, 3 }, getStar3 },
	{ { 15, 55 }, { 10, 7 }, getStar4 },
	{ { 140, 47 }, { 8, 5 }, getStar2 },
	{ { 180, 15 },  { 5, 3 }, getStar1 },
	{ { 170, 45 }, { 3, 3 }, getStar3 },
	{ { 120, 62 },  { 5, 3 }, getStar1 },
	{ { 85, 20 }, { 8, 5 }, getStar2 },
	{ { 110, 12 },  { 5, 3 }, getStar1 },
	{ { 160, 15 }, { 8, 5 }, getStar2 },

};
static const uint8_t starCount = sizeof(stars);

//ai_t enemy1;
void level2(void) {
	initTank(&playerTank);

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

    object_t player;

        initTank(&player);
        push_health(&player);

        setTankUpdateInterval(50); // 10 ms → 100 Hz
        // Game loop
        // Game loop
        while (1) {
            if (tankUpdateDue()) {
                controlTank(&world, &playerTank);
                //controlAITank(&enemy1, &world);

                Point tankPos = (Point){ getTankX(&playerTank), getTankY(&playerTank) };
                Rect tankRect = getTankRect(tankPos);

                for (uint8_t i = 0; i < starCount; i++) {
                    Rect starRect = getObstacleRect(stars[i].pos, stars[i].size);

                    if (!rectOverlap(tankRect, starRect)) {
                        fgcolor(11);
                        printCp850At(stars[i].pos.x, stars[i].pos.y, stars[i].sprite());
                        fgcolor(15);
                    }
                }
            }
        }
    }
