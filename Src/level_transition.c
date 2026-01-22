#include "level_transition.h"

void showVictoryScreen(uint8_t level_num) {
    fgcolor(11);
    printCp850At(90, 15, getVictory());
    fgcolor(15);
    gotoxy(90, 22);
    printf("Level %d Complete!", level_num);
    gotoxy(110, 22);
    printf("All enemy tanks destroyed!");
    gotoxy(90, 24);
    printf("Press CENTER to continue");

    char buf[32];

    // Wait for release
    while (1) {
        readKeyboardBuffer(buf);
        if (!(readMenuInput(buf) & JOY_CENTER))
            break;
    }

    // Wait for press
    while (1) {
        readKeyboardBuffer(buf);
        if (readMenuInput(buf) & JOY_CENTER)
            break;
    }

    // Wait for release again
    while (1) {
        readKeyboardBuffer(buf);
        if (!(readMenuInput(buf) & JOY_CENTER))
            break;
    }
}
void gameOverScreen(void) {
    //clrscr();
    fgcolor(1); // rød
    printCp850At(80, 28, getGameOver());
    fgcolor(15);
    char buf[32];
    // Wait for release
        while (1) {
            readKeyboardBuffer(buf);
            if (!(readMenuInput(buf) & JOY_CENTER))
                break;
        }

        // Wait for press
        while (1) {
            readKeyboardBuffer(buf);
            if (readMenuInput(buf) & JOY_CENTER)
                break;
        }

        // Wait for release again
        while (1) {
            readKeyboardBuffer(buf);
            if (!(readMenuInput(buf) & JOY_CENTER))
                break;
        }
        // Gå tilbage til title screen
        drawTitleScreen();
    }

uint8_t showPauseScreen(void) {
    gotoxy(109, 2);
    printf("GAME PAUSED");
    gotoxy(95, 3);
    printf("P:   Continue        R:   Main Menu");

    char buf[32];

    while (1) {
        readKeyboardBuffer(buf);

        if (buf[0] == 'p' || buf[0] == 'P') {
            gotoxy(109, 2);
            printf("           ");
            gotoxy(95, 3);
            printf("P:   Pause                         ");
            return 1;   // Continue
        }

        if (buf[0] == 'r' || buf[0] == 'R') {
            return 0;   // Main menu
        }
    }
}


void preLevelx(void) {
    static uint8_t level_num = 1;

    clrscr();
    char level = '0' + level_num;

    printCp850At(76, 5, getTitleArt());
    gotoxy(30, 28);
    printf("Level %c", level);
    gotoxy(30, 30);
    printf("Destroy all enemy space tanks...");
    gotoxy(30, 34);
    printf("Press CENTER to start!");

    char buf[32];

    // Wait for release
    while (1) {
        readKeyboardBuffer(buf);
        if (!(readMenuInput(buf) & JOY_CENTER))
            break;
    }

    uint8_t lastJoy = 0;

    while (1) {
        readKeyboardBuffer(buf);
        uint8_t joy = readMenuInput(buf);
        uint8_t newPress = joy & ~lastJoy;

        if (newPress & JOY_CENTER) {

            uint8_t result = 0;

            switch (level_num) {
                case 1: result = level1(); break;
                case 2: result = level2(); break;
                case 3: result = level3(); break;
            }

            if (result == 2) {
                level_num = 1;
                drawTitleScreen();
                return;
            } else if (result == 1) {
                showVictoryScreen(level_num);
                if (level_num < 3) level_num++;
                preLevelx();
            } else {
                level_num = 1;
                gameOverScreen();
            }

            return;
        }

        lastJoy = joy;
    }
}
