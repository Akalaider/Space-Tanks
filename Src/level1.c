#include "level1.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"

struct Star {
    Point pos;
    Point size;
    const char *(*sprite)(void);
};

// Liste af stjerner i level 2
static struct Star stars[] = {
    { { 5, 10 },  { 5, 3 }, getStar1 },
    { { 35, 12 }, { 8, 5 }, getStar2 },
    { { 50, 35 }, { 3, 3 }, getStar3 },
    { { 100, 20 },{10, 7 }, getStar4 },
    { { 90, 30 }, { 5, 3 }, getStar1 },
    { { 200, 60 },{ 3, 3 }, getStar3 },
    { { 15, 55 }, {10, 7 }, getStar4 },
    { { 140,47 }, { 8, 5 }, getStar2 },
    { { 180,15 }, { 5, 3 }, getStar1 },
    { { 170,45 }, { 3, 3 }, getStar3 },
    { { 120,62 }, { 5, 3 }, getStar1 },
    { { 85, 20 }, { 8, 5 }, getStar2 },
    { { 110,12 }, { 5, 3 }, getStar1 },
    { { 160,15 }, { 8, 5 }, getStar2 },
};

static const uint8_t starCount = sizeof(stars) / sizeof(stars[0]);
static uint8_t starOverlap(Point tankPos, struct Star s) {
    return !(tankPos.x + TANK_WIDTH  <= s.pos.x ||
             s.pos.x + s.size.x      <= tankPos.x ||
             tankPos.y + TANK_HEIGHT <= s.pos.y ||
             s.pos.y + s.size.y      <= tankPos.y);
}

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
    drawObstacle((Point){150, 45}, getAsteroid(), 21, 7, &world);
    drawObstacle((Point){40, 15}, getOumuamua(), 8, 9, &world);
    drawObstacle((Point){100, 35}, getAsteroid2(), 14, 9, &world);
    drawObstacle((Point){155, 20}, getAsteroid3(), 20, 4, &world);
    drawObstacle((Point){45, 45}, getAsteroid4(), 20, 9, &world);
    drawObstacle((Point){90, 7}, getAsteroid5(), 15, 8, &world);

    
    drawWalls(outerWall, 4, 1, &world);

    object_t objecthandler[OBJECTHANDLER_SIZE];
    initObjecthandler(objecthandler);

    initTank(&objecthandler[0]);
    timer_attachPlayerTank(&objecthandler[0]); // For cooldowns
    push_info_lcd(&objecthandler[0]);

    initAITank(&objecthandler[1], 200, 10);    // enemy 1

    
    setTankUpdateInterval(50); // 10 ms → 100 Hz
    // Game loop
    while (1) {
          if (tankUpdateDue()) {
             updateObject(objecthandler, &world);
          }


          Point tankPos = {
          getTankX(&objecthandler[0]),
          getTankY(&objecthandler[0])
          };

          // Restore stjerner
          for (uint8_t i = 0; i < starCount; i++) {
              if (!starOverlap(tankPos, stars[i])) {
                 fgcolor(11);
                 printCp850At(stars[i].pos.x, stars[i].pos.y, stars[i].sprite());
                 fgcolor(15);
              }
          }
    }
}
