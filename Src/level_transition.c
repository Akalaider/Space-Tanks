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
    
    // Wait for release
    while (readHat() & JOY_CENTER) {}
    
    // Wait for press
    while (!(readHat() & JOY_CENTER)) {}
    
    // Wait for release again
    while (readHat() & JOY_CENTER) {}
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
    printf("Destroy all enemy space tanks, while maneuvering around asteroids and try to stay alive!");
    gotoxy(30, 32);
    printf("Collect powerups by flying over them, and advance to the next level when all enemies are destroyed!");
    gotoxy(30, 34);
    printf("Press CENTER to start!");

    // Wait for release
    while (readHat() & JOY_CENTER) {}

    uint8_t lastJoy = 0;

    while (1) {
        uint8_t joy = readHat();
        uint8_t newPress = joy & ~lastJoy;

        if (newPress & JOY_CENTER) {

            uint8_t result = 0;

            switch (level_num) {
                case 1: result = level1(); break;
                case 2: result = level2(); break;
                case 3: result = level3(); break;
            }

            if (result == 2) {
                // Pause -> Main Menu selected
                level_num = 1;  // Reset to level 1
                drawTitleScreen();
                return;
            } else if (result == 1) {
                // Win
                showVictoryScreen(level_num);
                if (level_num < 3) level_num++;   // advance
                preLevelx();
            } else {
                // Loss (result == 0)
                level_num = 1;  // restart from level 1
                drawTitleScreen();
            }

            return;
        }

        lastJoy = joy;
    }
}