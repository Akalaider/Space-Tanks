#include "level1.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"
#include "timer.h"
#include "aimovement.h"

static uint8_t playerIsDead(object_t *objects) {
    // Player is always index 0
    return getTankHealth(&objects[0]) == 0;
}

static uint8_t allEnemiesGone(object_t *objects) {
    for (uint16_t i = 0; i < OBJECTHANDLER_SIZE; i++) {
        if (objects[i].type == enemy) {
            return 0;   // still at least one enemy
        }
    }
    return 1;           // no enemies left
}
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


uint8_t level1(void) {
    clrscr();
    
    // Draw game arena
    Point outerWall[] = {
        {0, 0},
        {240, 0},
        {240, 72},
        {0, 72}
    };

    Point box[] = {
        {50, 20},
        {70, 20},
        {70, 30},
        {50, 30}
    };
    
    World world = {0};
    
    drawWalls(outerWall, 4, 1, &world);
    drawObstacle((Point){150, 45}, getAsteroid(), 21, 7, &world);
    drawObstacle((Point){40, 15}, getOumuamua(), 8, 9, &world);
    drawObstacle((Point){100, 35}, getAsteroid2(), 14, 9, &world);
    drawObstacle((Point){155, 20}, getAsteroid3(), 20, 4, &world);
    drawObstacle((Point){45, 45}, getAsteroid4(), 20, 9, &world);
    drawObstacle((Point){90, 7}, getAsteroid5(), 15, 8, &world);

    object_t objecthandler[OBJECTHANDLER_SIZE];
    initObjecthandler(objecthandler);

    initTank(&objecthandler[0]);
    timer_attachPlayerTank(&objecthandler[0]); // For cooldowns

    push_info_lcd(&objecthandler[0]);
    initAITank(&objecthandler[1], 200, 10);    // enemy 1

    setTankUpdateInterval(50); // 10 ms → 100 Hz
    TIM15->CR1 |= 0x0001;   // enable counter

    char buf[32];
    // Game loop
    
    while (1) {
        readKeyboardBuffer(buf);

        // Pause key pressed?
        if (buf[0] == 'p' || buf[0] == 'P') {
            stTimer();  // stop timer
            buf[0] = '\0';

            uint8_t choice = showPauseScreen();

            if (choice == 0) {   // Return to main menu
                stTimer();
                drawTitleScreen();
                return 2;
            }

            // Continue game
            stTimer();  
            buf[0] = '\0';
        }

        if (tankUpdateDue()) {
            updateObject(objecthandler, &world, buf);

            if (playerIsDead(objecthandler)) return 0;
            if (allEnemiesGone(objecthandler)) return 1;
        }
    }

}





// Liste af stjerner i level 2
static struct Star stars2[] = {
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

static const uint8_t starCount2 = sizeof(stars2) / sizeof(stars2[0]);
static uint8_t starOverlap2(Point tankPos, struct Star s) {
    return !(tankPos.x + TANK_WIDTH  <= s.pos.x ||
             s.pos.x + s.size.x      <= tankPos.x ||
             tankPos.y + TANK_HEIGHT <= s.pos.y ||
             s.pos.y + s.size.y      <= tankPos.y);
}


uint8_t level2(void) {
    clrscr();

    // Ydre væg
    Point outerWall[] = {
        {0, 0}, {240, 0}, {240, 72}, {0, 72}
    };

    World world = {0};
    drawWalls(outerWall, 4, 1, &world);

    // Indre vægge
    Point innerWall1[] = {
        {25,10},{25,20},{45,20},{45,30},{65,30}
    };
    drawWalls(innerWall1, 5, 0, &world);

    Point innerWall2[] = {
        {185,10},{185,20},{165,20},{165,30},{145,30}
    };
    drawWalls(innerWall2, 5, 0, &world);

    Point innerWall3[] = {
        {65,40},{45,40},{45,50},{25,50},{25,60}
    };
    drawWalls(innerWall3, 5, 0, &world);

    Point innerWall4[] = {
        {145,40},{165,40},{165,50},{185,50},{185,60}
    };
    drawWalls(innerWall4, 5, 0, &world);

    // Asteroider
    drawObstacle((Point){95, 32}, getAsteroid3(), 20, 4, &world);
    drawObstacle((Point){130,10}, getAsteroid6(), 17, 8, &world);
    drawObstacle((Point){60, 15}, getAsteroid4(), 20, 9, &world);
    drawObstacle((Point){57, 50}, getAsteroid2(), 14, 9, &world);
    drawObstacle((Point){125,48}, getAsteroid(),  21, 7, &world);


    object_t objecthandler[OBJECTHANDLER_SIZE];
    initObjecthandler(objecthandler);

    initTank(&objecthandler[0]);
    timer_attachPlayerTank(&objecthandler[0]); // For cooldowns

    push_info_lcd(&objecthandler[0]);
    initAITank(&objecthandler[1], 200, 10);    // enemy 1
    initAITank(&objecthandler[2], 200, 50);

    setTankUpdateInterval(50); // 10 ms → 100 Hz
    char buf[32];

    uint8_t lastJoy = 0;  // Track previous joystick state
    
    while (1) {
        readKeyboardBuffer(buf);

        // Pause key pressed?
        if (buf[0] == 'p' || buf[0] == 'P') {
            stTimer();  // stop timer
            buf[0] = '\0';

            uint8_t choice = showPauseScreen();

            if (choice == 0) {   // Return to main menu
                stTimer();
                drawTitleScreen();
                return 2;
            }

            // Continue game
            stTimer();  
            buf[0] = '\0';
        }

        if (tankUpdateDue()) {
            updateObject(objecthandler, &world, buf);

            if (playerIsDead(objecthandler)) return 0;
            if (allEnemiesGone(objecthandler)) return 1;
        }
    }
}

// Liste af stjerner i level 1
static struct Star stars3[] = {
    { { 5, 10 },  { 5, 3 }, getStar1 },
    { { 20, 15 }, { 8, 5 }, getStar2 },
    { { 50, 40 }, { 3, 3 }, getStar3 },
    { { 135, 35 }, { 10, 7 }, getStar4 },
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
static const uint8_t starCount3 = sizeof(stars3) / sizeof(stars3[0]);

static uint8_t starOverlap3(Point tankPos, struct Star s) {
    return !(tankPos.x + TANK_WIDTH  <= s.pos.x ||
             s.pos.x + s.size.x      <= tankPos.x ||
             tankPos.y + TANK_HEIGHT <= s.pos.y ||
             s.pos.y + s.size.y      <= tankPos.y);
}


//ai_t enemy1;
uint8_t level3(void) {
    clrscr();

    // Draw game arena
    Point outerWall[] = {
         {0, 0}, {240, 0}, {240, 72}, {0, 72}
    };

    World world = {0};

    drawWalls(outerWall, 4, 1, &world);

    Point innerWall1[] = {
        {25, 20}, {25, 10}, {85, 10}
    };
    drawWalls(innerWall1, 6, 0, &world);

    Point innerWall2[] = {
    		{105, 10}, {105, 40},
    };
    drawWalls(innerWall2, 2, 0, &world);

    Point innerWall3[] = {
    		{105,25}, {130,25}
    };
    drawWalls(innerWall3, 2, 0, &world);

    Point innerWall4[] = {
    		{25, 40}, {85, 40}
    };

    drawWalls(innerWall4, 2, 0, &world);

    Point innerWall5[] = {
    		{55, 40}, {55, 55}
    };

    drawWalls(innerWall5, 2, 0, &world);

    Point innerWall6[] = {
    		{105, 50}, {155, 50}
    };

    drawWalls(innerWall6, 2, 0, &world);

    Point innerWall7[] = {
    		{135, 35}, {135, 50}
    };

    drawWalls(innerWall7, 2, 0, &world);

    Point innerWall8[] = {
    		{155, 10}, {155, 35}, {185, 35}, {185, 50}
    };

    drawWalls(innerWall8, 4, 0, &world);

    drawObstacle((Point){123, 60}, getAsteroid3(), 20, 4, &world);
    drawObstacle((Point){120, 10}, getAsteroid6(), 17, 8, &world);
    drawObstacle((Point){52, 20}, getAsteroid4(), 20, 9, &world);
    drawObstacle((Point){30, 53}, getAsteroid2(), 14, 9, &world);
    drawObstacle((Point){177, 14}, getAsteroid(), 21, 7, &world);



    object_t objecthandler[OBJECTHANDLER_SIZE];
    initObjecthandler(objecthandler);

    initTank(&objecthandler[0]);
    timer_attachPlayerTank(&objecthandler[0]); // For cooldowns

    push_info_lcd(&objecthandler[0]);
    initAITank(&objecthandler[1], 200, 10);    // enemy 1
    initAITank(&objecthandler[2], 220, 35);
    initAITank(&objecthandler[3], 210, 65);
    setTankUpdateInterval(50); // 10 ms → 100 Hz

    // Game loop
    char buf[32];

    uint8_t lastJoy = 0;  // Track previous joystick state
    
    while (1) {
        readKeyboardBuffer(buf);

        // Pause key pressed?
        if (buf[0] == 'p' || buf[0] == 'P') {
            stTimer();  // stop timer
            buf[0] = '\0';

            uint8_t choice = showPauseScreen();

            if (choice == 0) {   // Return to main menu
                stTimer();
                drawTitleScreen();
                return 2;
            }

            // Continue game
            stTimer();  
            buf[0] = '\0';
        }

        if (tankUpdateDue()) {
            updateObject(objecthandler, &world, buf);

            if (playerIsDead(objecthandler)) return 0;
            if (allEnemiesGone(objecthandler)) return 1;
        }
    }
}