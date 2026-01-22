#ifndef MENU_H
#define MENU_H

#include <stdint.h>

typedef enum {
    MENU_MAIN = 0,
    MENU_GAMEMODE_LEFT = 1,
    MENU_GAMEMODE_RIGHT = 2,
    MENU_GAMEMODE_BACK = 3,
    MENU_HIGHSCORES = 4,
    MENU_MULTIPLAYER_LEFT = 5,
    MENU_MULTIPLAYER_RIGHT = 6,
    MENU_MULTIPLAYER_BACK = 7,
    MENU_START_SINGLEPLAYER = 8,
    MENU_START_COOP = 9
} MenuState;

MenuState drawTitleScreen(void);


#endif // MENU_H