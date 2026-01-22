#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"
#include "timer.h"
#include "LCD.h"
#include "art.h"
#include "world.h"
#include "tank.h"
#include "menu.h"
#include "level1.h"
#include "io.h"

#define clearCursor() printf("\033[?25l")
#define showCursor()  printf("\033[?25h")

int main(void)
{
    uart_init(1000000);
    clearCursor();

    // Initialize hardware
    initTimer(1, 0);
    setupClock(); // For joystick
    setupJoystick();
    setupLED();
    lcd_init();

    // Clear the LDC
    uint8_t lcd_buffer[512] = {0};
    char screen[] = "";
    lcd_write_string(lcd_buffer, screen, 0); 
    lcd_write_string(lcd_buffer, screen, 1); 
    lcd_write_string(lcd_buffer, screen, 2); 
    lcd_write_string(lcd_buffer, screen, 3); 
    lcd_update(lcd_buffer);

    // Start menu
    while (1) {
        MenuState result = drawTitleScreen();

        if (result == MENU_START_SINGLEPLAYER) {
            preLevelx();
        }
    }
}