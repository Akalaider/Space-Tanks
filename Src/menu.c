#include "menu.h"
#include "ansi.h"
#include "art.h"
#include "blink.h"
#include "timer.h"
#include "io.h"
#include "level1.h"

typedef enum {
    MENU_MAIN = 0,
    MENU_GAMEMODE_LEFT = 1,
    MENU_GAMEMODE_RIGHT = 2,
    MENU_GAMEMODE_BACK = 3,
} MenuState;

typedef struct {
    uint8_t x;
    uint8_t y;
    const char *cursor;
    const char *blankCursor;
} MenuItem;

// Main menu positions
#define MAIN_MENU_X 16
#define MAIN_MENU_PLAY_Y 21
#define MAIN_MENU_HIGHSCORE_Y 31

// Gamemode menu positions
#define GAMEMODE_MENU_LEFT_X 109
#define GAMEMODE_MENU_RIGHT_X 119
#define GAMEMODE_MENU_Y 31

// Back arrow position
#define GAMEMODE_MENU_BACK_X 112
#define GAMEMODE_MENU_BACK_Y 33

// Back button position
#define BACK_BUTTON_X 96
#define BACK_BUTTON_Y 45

// Display Functions
static void blinkSelector(const BlinkController *b, const char *cursor, const char *blankCursor) {
    const char *displayCursor = b->state ? cursor : blankCursor;
    printCp850At(b->x, b->y, displayCursor);
}

static void drawMainMenu(void) {
    clrscr();
    printCp850At(76, 5, getTitleArt());
    printCp850At(28, 20, getPlayGame());
    printCp850At(30, 30, getHighscores());
    gotoxy(30, 40);
    printf(" > Use joystick to navigate menu <");
    gotoxy(30, 42);
    printf("   CENTER = Select  |  UP = ^  |  DOWN = v");
}

static void drawGamemodeMenu(void) {
    clrscr();
    printCp850At(76, 5, getTitleArt());
    printCp850At(3, 30, getSingleplayer());
    printCp850At(135, 30, getMultiplayer());
    printCp850At(BACK_BUTTON_X, BACK_BUTTON_Y, getBack());
}

// Menu Item Helpers
static MenuItem getMainMenuItem(void) {
    return (MenuItem){
        .x = MAIN_MENU_X,
        .y = MAIN_MENU_PLAY_Y,
        .cursor = getSelector(),
        .blankCursor = getBlankSelector()
    };
}

static MenuItem getGamemodeLeftMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_LEFT_X,
        .y = GAMEMODE_MENU_Y,
        .cursor = getArrowLeft(),
        .blankCursor = getBlankArrow()
    };
}

static MenuItem getGamemodeRightMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_RIGHT_X,
        .y = GAMEMODE_MENU_Y,
        .cursor = getArrowRight(),
        .blankCursor = getBlankArrow()
    };
}

static MenuItem getGamemodeBackMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_BACK_X,
        .y = GAMEMODE_MENU_BACK_Y,
        .cursor = getArrowDown(),
        .blankCursor = getBlankArrow()
    };
}

// Menu Navigation
static void updateSelectorPosition(BlinkController *b, uint8_t newX, uint8_t newY, 
                                   const char *cursor, const char *blankCursor) {
    if (b->x != newX || b->y != newY) {
        // Clear old position
        printCp850At(b->x, b->y, blankCursor);
        
        // Update position
        b->x = newX;
        b->y = newY;
        
        // Draw at new position immediately
        b->state = 1;
        b->counter = 0;
        printCp850At(b->x, b->y, cursor);
    }
}

static void handleMainMenuInput(BlinkController *b, uint8_t joyState, 
                                const char *cursor, const char *blankCursor,
                                MenuState *currentMenu) {
    // Navigate up to "Play Game"
    if ((joyState & JOY_UP) && b->y != MAIN_MENU_PLAY_Y) {
        updateSelectorPosition(b, MAIN_MENU_X, MAIN_MENU_PLAY_Y, cursor, blankCursor);
    }
    
    // Navigate down to "Highscores"
    if ((joyState & JOY_DOWN) && b->y != MAIN_MENU_HIGHSCORE_Y) {
        updateSelectorPosition(b, MAIN_MENU_X, MAIN_MENU_HIGHSCORE_Y, cursor, blankCursor);
    }
    
    // Select current option
    if (joyState & JOY_CENTER) {
        if (b->y == MAIN_MENU_PLAY_Y) {
            *currentMenu = MENU_GAMEMODE_LEFT;
            drawGamemodeMenu();
            MenuItem gamemodeItem = getGamemodeLeftMenuItem();
            b->x = gamemodeItem.x;
            b->y = gamemodeItem.y;
            b->state = 1;
            b->counter = 0;
            printCp850At(b->x, b->y, gamemodeItem.cursor);
        } else if (b->y == MAIN_MENU_HIGHSCORE_Y) {
            // TODO: Show highscores
        }
    }
}

static void handleGamemodeMenuInput(BlinkController *b, uint8_t joyState,
                                    const char *cursor, const char *blankCursor,
                                    MenuState *currentMenu) {
    // If on back arrow
    if (*currentMenu == MENU_GAMEMODE_BACK) {
        // Navigate up from back arrow
        if (joyState & JOY_LEFT) {
            MenuItem leftItem = getGamemodeLeftMenuItem();
            updateSelectorPosition(b, GAMEMODE_MENU_LEFT_X, GAMEMODE_MENU_Y,
                                  leftItem.cursor, leftItem.blankCursor);
            *currentMenu = MENU_GAMEMODE_LEFT;
        }
        if (joyState & JOY_RIGHT) {
            MenuItem rightItem = getGamemodeRightMenuItem();
            updateSelectorPosition(b, GAMEMODE_MENU_RIGHT_X, GAMEMODE_MENU_Y,
                                  rightItem.cursor, rightItem.blankCursor);
            *currentMenu = MENU_GAMEMODE_RIGHT;
        }

        // Center to go back
        if (joyState & JOY_CENTER) {
            *currentMenu = MENU_MAIN;
            drawMainMenu();
            MenuItem mainItem = getMainMenuItem();
            b->x = mainItem.x;
            b->y = mainItem.y;
            b->state = 1;
            b->counter = 0;
            printCp850At(b->x, b->y, mainItem.cursor);
        }
        return;
    }
    
    // If on left or right arrows
    // Navigate left to singleplayer
    if ((joyState & JOY_LEFT) && b->x != GAMEMODE_MENU_LEFT_X) {
        MenuItem leftItem = getGamemodeLeftMenuItem();
        updateSelectorPosition(b, GAMEMODE_MENU_LEFT_X, GAMEMODE_MENU_Y, 
                              leftItem.cursor, leftItem.blankCursor);
        *currentMenu = MENU_GAMEMODE_LEFT;
    }

    // Select singleplayer
    if ((joyState & JOY_CENTER) && b->x == GAMEMODE_MENU_LEFT_X) {
        level1();
    }
    
    // Navigate right to multiplayer
    if ((joyState & JOY_RIGHT) && b->x != GAMEMODE_MENU_RIGHT_X) {
        MenuItem rightItem = getGamemodeRightMenuItem();
        updateSelectorPosition(b, GAMEMODE_MENU_RIGHT_X, GAMEMODE_MENU_Y,
                              rightItem.cursor, rightItem.blankCursor);
        *currentMenu = MENU_GAMEMODE_RIGHT;
    }
    
    // Navigate down to back arrow
    if (joyState & JOY_DOWN) {
        MenuItem backItem = getGamemodeBackMenuItem();
        updateSelectorPosition(b, GAMEMODE_MENU_BACK_X, GAMEMODE_MENU_BACK_Y,
                              backItem.cursor, backItem.blankCursor);
        *currentMenu = MENU_GAMEMODE_BACK;
    }
    
    // Select current gamemode
    if (joyState & JOY_CENTER) {
        if (b->x == GAMEMODE_MENU_LEFT_X) {
            // TODO: Start singleplayer
        } else if (b->x == GAMEMODE_MENU_RIGHT_X) {
            // TODO: Start multiplayer
        }
    }
}

static void processMenuInput(BlinkController *b, uint8_t joyState, 
                             MenuItem *currentItem, MenuState *currentMenu) {
    switch (*currentMenu) {
        case MENU_MAIN:
            handleMainMenuInput(b, joyState, currentItem->cursor, 
                              currentItem->blankCursor, currentMenu);
            break;
            
        case MENU_GAMEMODE_LEFT:
        case MENU_GAMEMODE_RIGHT:
        case MENU_GAMEMODE_BACK:
            handleGamemodeMenuInput(b, joyState, currentItem->cursor,
                                   currentItem->blankCursor, currentMenu);
            break;
    }
}

// Menu Item Update
static MenuItem getCurrentMenuItem(MenuState currentMenu) {
    switch (currentMenu) {
        case MENU_MAIN:
            return getMainMenuItem();
        case MENU_GAMEMODE_LEFT:
            return getGamemodeLeftMenuItem();
        case MENU_GAMEMODE_RIGHT:
            return getGamemodeRightMenuItem();
        case MENU_GAMEMODE_BACK:
            return getGamemodeBackMenuItem();
        default:
            return getMainMenuItem();
    }
}

void drawTitleScreen(void) {
    // Initialize hardware
    initTimer(6400000, 0);
    setupClock();
    setupJoystick();
    setupLED();
    
    // Initialize menu state
    MenuState currentMenu = MENU_MAIN;
    MenuItem currentItem = getMainMenuItem();
    
    // Initialize blink controller
    BlinkController selectorBlink = {
        .cursor = currentItem.cursor,
        .blankCursor = currentItem.blankCursor,
        .intervalMS = 700,
        .counter = 0,
        .state = 1,
        .x = currentItem.x,
        .y = currentItem.y
    };
    
    timer_attachBlink(&selectorBlink);
    
    drawMainMenu();
    
    uint8_t lastBlinkState = selectorBlink.state;
    uint8_t lastJoyState = 0;
    
    while (1) {
        uint8_t joyState = readJoystick();
        setLED(joyState);
        
        // Update menu item if menu changed
        MenuItem newItem = getCurrentMenuItem(currentMenu);
        if (newItem.cursor != currentItem.cursor || newItem.blankCursor != currentItem.blankCursor) {
            currentItem = newItem;
        }
        
        // Handle blinking
        if (selectorBlink.state != lastBlinkState) {
            blinkSelector(&selectorBlink, currentItem.cursor, currentItem.blankCursor);
            lastBlinkState = selectorBlink.state;
        }
        
        // Process input
        uint8_t newPress = joyState & ~lastJoyState;
        if (newPress) {
            processMenuInput(&selectorBlink, newPress, &currentItem, &currentMenu);
        }
        
        lastJoyState = joyState;
    }
}