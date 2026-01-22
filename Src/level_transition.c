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
    while (readJoystick() & JOY_CENTER) {}
    
    // Wait for press
    while (!(readJoystick() & JOY_CENTER)) {}
    
    // Wait for release again
    while (readJoystick() & JOY_CENTER) {}
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
    while (readJoystick() & JOY_CENTER) {}

    uint8_t lastJoy = 0;

    while (1) {
        uint8_t joy = readJoystick();
        uint8_t newPress = joy & ~lastJoy;

        if (newPress & JOY_CENTER) {

            uint8_t beaten = 0;

            switch (level_num) {
                case 1: beaten = level1(); break;
                case 2: beaten = level2(); break;
                case 3: beaten = level3(); break;
            }

            if (beaten) {
                showVictoryScreen(level_num);
                if (level_num < 3) level_num++;   // advance
                preLevelx();
            } else {

                level_num = 1;                    // restart from level 1
                preLevelx();
            }

            return;
        }

        lastJoy = joy;
    }
}
