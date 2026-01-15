#include "menu.h"
#include "ansi.h"
#include "art.h"
#include "menu.h"
#include "blink.h"
#include "timer.h"
#include "io.h"


void blinkSelector(const BlinkController *b) {
    if (b->state)
        printCp850At(b->x, b->y, getSelector());
    else
        printCp850At(b->x, b->y, getBlankSelector());
}

void moveSelector(BlinkController *b, uint8_t joyState) {
    if (joyState & JOY_UP && b->y != 21) {
        printCp850At(b->x, b->y, getBlankSelector());
        b->y = 21;
        printCp850At(b->x, b->y, getSelector());
    }
    if (joyState & JOY_DOWN && b->y != 31) {
        printCp850At(b->x, b->y, getBlankSelector());
        b->y = 31;
        printCp850At(b->x, b->y, getSelector());
    }
}

void drawTitleScreen(void)
{
    // Setup timer
    initTimer(6400000, 0);

    BlinkController selectorBlink = { 
        .intervalMS = 700, 
        .counter = 0, 
        .state = 1, 
        .x = 16, 
        .y = 21 
    };

    timer_attachBlink(&selectorBlink);

    // Setup joystick
    setupClock();
    setupJoystick();
    int8_t joyState = readJoystick();
    setupLED();

    clrscr(); 

    printCp850At(72, 5, getTitleArt());
    printCp850At(28, 20, getPlayGame());
    printCp850At(30, 30, getHighscores());
    gotoxy(30, 40); 
    printf(" > Use joystick to navigate menu <"); 
    gotoxy(30, 42); 
    printf("   CENTER = Select  |  UP = ^  |  DOWN = v");

    uint8_t lastState = selectorBlink.state; 
    while(1) { 
        joyState = readJoystick();
        setLED(joyState);
        if (selectorBlink.state != lastState) { 
            blinkSelector(&selectorBlink); 
            lastState = selectorBlink.state; 
        }
        moveSelector(&selectorBlink, joyState);
    }
}