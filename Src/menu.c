#include "menu.h"
#include "ansi.h"
#include "art.h"
#include "blink.h"
#include "timer.h"
#include "io.h"
#include "level1.h"

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
#define BACK_BUTTON_Y 55

// Display Functions
static void blinkSelector(const BlinkController *b, const char *cursor, const char *blankCursor) {
    const char *displayCursor = b->state ? cursor : blankCursor;
    printCp850At(b->x, b->y, displayCursor);
}

static void drawMainMenu(void) {
    bgcolor(0); // Set background color to black
    clrscr();
    printCp850At(76, 5, getTitleArt());
    printCp850At(28, 20, getPlayGame());
    printCp850At(30, 30, getHighscores());
    gotoxy(30, 65);
    fgcolor(13);
    printf("Use joystick to navigate menu");
    gotoxy(30, 67);
    printf("CENTER = Select");
    fgcolor(15);
}

static void drawGamemodeMenu(void) {
    clrscr();
    printCp850At(76, 5, getTitleArt());
    printCp850At(3, 30, getSingleplayer());
    printCp850At(135, 30, getMultiplayer());
    printCp850At(BACK_BUTTON_X, BACK_BUTTON_Y, getBack());
    gotoxy(30, 65);
    fgcolor(13);
    printf("Use joystick to navigate menu");
    gotoxy(30, 67);
    printf("CENTER = Select");
    fgcolor(15);
}

static void drawMultiplayerMenu(void) {
    clrscr();
    printCp850At(76, 5, getTitleArt());
    printCp850At(45, 30, getCoop());
    printCp850At(145, 30, getVersus());
    printCp850At(BACK_BUTTON_X, BACK_BUTTON_Y, getBack());
    gotoxy(30, 65);
    fgcolor(13);
    printf("Use joystick to navigate menu");
    gotoxy(30, 67);
    printf("CENTER = Select");
    fgcolor(15);
}

static void drawHighscoresMenu(void) {
    clrscr();
    printCp850At(76, 5, getTitleArt());

    gotoxy(10, 20);
    fgcolor(10);
    printf("Player 1");
    printCp850At(10, 25, getController());
    gotoxy(20, 20);
    fgcolor(12);
    printf("Player 2 - Press SPACE for normal bullets");
    printCp850At(130, 17, getKeyboard());
    gotoxy(40, 67);
    fgcolor(13);
    printf("CENTER = Go back");
    fgcolor(15);

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

static MenuItem getMultiplayerLeftMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_LEFT_X,
        .y = GAMEMODE_MENU_Y,
        .cursor = getArrowLeft(),
        .blankCursor = getBlankArrow()
    };
}

static MenuItem getMultiplayerRightMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_RIGHT_X,
        .y = GAMEMODE_MENU_Y,
        .cursor = getArrowRight(),
        .blankCursor = getBlankArrow()
    };
}

static MenuItem getMultiplayerBackMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_BACK_X,
        .y = GAMEMODE_MENU_BACK_Y,
        .cursor = getArrowDown(),
        .blankCursor = getBlankArrow()
    };
}

static MenuItem getHighscoresBackMenuItem(void) {
    return (MenuItem){
        .x = GAMEMODE_MENU_BACK_X,
        .y = GAMEMODE_MENU_BACK_Y,
        .cursor = getBlankSelector(),
        .blankCursor = getBlankSelector()
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
            *currentMenu = MENU_HIGHSCORES;
            drawHighscoresMenu();
            MenuItem highscoresItem = getHighscoresBackMenuItem();
            b->x = highscoresItem.x;
            b->y = highscoresItem.y;
            b->state = 1;
            b->counter = 0;
        }
    }
}

static void handleHighscoresMenuInput(BlinkController *b, uint8_t joyState,
                                      const char *cursor, const char *blankCursor,
                                      MenuState *currentMenu) {
    if (*currentMenu == MENU_HIGHSCORES) {
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
    }
    return;
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
        timer_detachBlink(b);
        *currentMenu = MENU_START_SINGLEPLAYER;
        return;
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
    
    // Select multiplayer to go to multiplayer submenu
    if ((joyState & JOY_CENTER) && b->x == GAMEMODE_MENU_RIGHT_X) {
        *currentMenu = MENU_MULTIPLAYER_LEFT;
        drawMultiplayerMenu();
        MenuItem multiplayerItem = getMultiplayerLeftMenuItem();
        b->x = multiplayerItem.x;
        b->y = multiplayerItem.y;
        b->state = 1;
        b->counter = 0;
        printCp850At(b->x, b->y, multiplayerItem.cursor);
    }
}

static void handleMultiplayerMenuInput(BlinkController *b, uint8_t joyState,
                                       const char *cursor, const char *blankCursor,
                                       MenuState *currentMenu) {
    // If on back arrow
    if (*currentMenu == MENU_MULTIPLAYER_BACK) {
        // Navigate up from back arrow
        if (joyState & JOY_LEFT) {
            MenuItem leftItem = getMultiplayerLeftMenuItem();
            updateSelectorPosition(b, GAMEMODE_MENU_LEFT_X, GAMEMODE_MENU_Y,
                                  leftItem.cursor, leftItem.blankCursor);
            *currentMenu = MENU_MULTIPLAYER_LEFT;
        }
        if (joyState & JOY_RIGHT) {
            MenuItem rightItem = getMultiplayerRightMenuItem();
            updateSelectorPosition(b, GAMEMODE_MENU_RIGHT_X, GAMEMODE_MENU_Y,
                                  rightItem.cursor, rightItem.blankCursor);
            *currentMenu = MENU_MULTIPLAYER_RIGHT;
        }

        // Center to go back to gamemode menu
        if (joyState & JOY_CENTER) {
            *currentMenu = MENU_GAMEMODE_LEFT;
            drawGamemodeMenu();
            MenuItem gamemodeItem = getGamemodeLeftMenuItem();
            b->x = gamemodeItem.x;
            b->y = gamemodeItem.y;
            b->state = 1;
            b->counter = 0;
            printCp850At(b->x, b->y, gamemodeItem.cursor);
        }
        return;
    }
    
    // If on left or right arrows
    // Navigate left to coop
    if ((joyState & JOY_LEFT) && b->x != GAMEMODE_MENU_LEFT_X) {
        MenuItem leftItem = getMultiplayerLeftMenuItem();
        updateSelectorPosition(b, GAMEMODE_MENU_LEFT_X, GAMEMODE_MENU_Y, 
                              leftItem.cursor, leftItem.blankCursor);
        *currentMenu = MENU_MULTIPLAYER_LEFT;
    }
    
    // Navigate right to versus
    if ((joyState & JOY_RIGHT) && b->x != GAMEMODE_MENU_RIGHT_X) {
        MenuItem rightItem = getMultiplayerRightMenuItem();
        updateSelectorPosition(b, GAMEMODE_MENU_RIGHT_X, GAMEMODE_MENU_Y,
                              rightItem.cursor, rightItem.blankCursor);
        *currentMenu = MENU_MULTIPLAYER_RIGHT;
    }
    
    // Navigate down to back arrow
    if (joyState & JOY_DOWN) {
        MenuItem backItem = getMultiplayerBackMenuItem();
        updateSelectorPosition(b, GAMEMODE_MENU_BACK_X, GAMEMODE_MENU_BACK_Y,
                              backItem.cursor, backItem.blankCursor);
        *currentMenu = MENU_MULTIPLAYER_BACK;
    }
    
    // Select current mode
    if (joyState & JOY_CENTER) {
        if (b->x == GAMEMODE_MENU_LEFT_X) {
            timer_detachBlink(b);
            // TODO: Start coop mode
        } else if (b->x == GAMEMODE_MENU_RIGHT_X) {
            timer_detachBlink(b);
            // TODO: Start versus mode
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
            
        case MENU_MULTIPLAYER_LEFT:
        case MENU_MULTIPLAYER_RIGHT:
        case MENU_MULTIPLAYER_BACK:
            handleMultiplayerMenuInput(b, joyState, currentItem->cursor,
                                      currentItem->blankCursor, currentMenu);
            break;
            
        case MENU_HIGHSCORES:
            handleHighscoresMenuInput(b, joyState, currentItem->cursor,
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
        case MENU_MULTIPLAYER_LEFT:
            return getMultiplayerLeftMenuItem();
        case MENU_MULTIPLAYER_RIGHT:
            return getMultiplayerRightMenuItem();
        case MENU_MULTIPLAYER_BACK:
            return getMultiplayerBackMenuItem();
        case MENU_HIGHSCORES:
            return getHighscoresBackMenuItem();
        default:
            return getMainMenuItem();
    }
}

MenuState drawTitleScreen(void) {  
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
        char buf[32]; 
        readKeyboardBuffer(buf); 
        uint8_t joyState = readMenuInput(buf); // joystick + hat + keyboard 
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
        // Detect only transitions from 0 → 1
        uint8_t newPress = joyState & ~lastJoyState;

        if (newPress & JOY_CENTER) {
            processMenuInput(&selectorBlink, JOY_CENTER, &currentItem, &currentMenu);
        }

        if (newPress & JOY_UP) {
            processMenuInput(&selectorBlink, JOY_UP, &currentItem, &currentMenu);
        }

        if (newPress & JOY_DOWN) {
            processMenuInput(&selectorBlink, JOY_DOWN, &currentItem, &currentMenu);
        }

        if (newPress & JOY_LEFT) {
            processMenuInput(&selectorBlink, JOY_LEFT, &currentItem, &currentMenu);
        }

        if (newPress & JOY_RIGHT) {
            processMenuInput(&selectorBlink, JOY_RIGHT, &currentItem, &currentMenu);
        }

        lastJoyState = joyState;

        if (currentMenu == MENU_START_SINGLEPLAYER) { 
            return MENU_START_SINGLEPLAYER;
        }
    }
}