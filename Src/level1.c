#include "level1.h"
#include "world.h"
#include "tank.h"
#include "ansi.h"
#include "movement.h"
#include "io.h"

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
    drawWalls(box, 4, 1, &world);

    object_t objecthandler[OBJECTHANDLER_SIZE];
    initObjecthandler(objecthandler);

    initTank(&objecthandler[0]);
    timer_attachPlayerTank(&objecthandler[0]); // For cooldowns

    push_info_lcd(&objecthandler[0]);
    initAITank(&objecthandler[1]);    // enemy 1

    setTankUpdateInterval(50); // 10 ms → 100 Hz
    // Game loop
    while (1) {
        if (tankUpdateDue()) {
            updateObject(objecthandler, &world);

            // Loss
            if (playerIsDead(objecthandler)) {
                return 0;
            }

            // Win
            if (allEnemiesGone(objecthandler)) {
                return 1;
            }
        }
    }
}