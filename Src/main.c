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
    uart_init(384000);
    clearCursor();

    drawTitleScreen();
}