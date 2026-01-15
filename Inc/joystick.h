#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

#define up 1
#define down 2
#define left 4
#define right 8
#define center 16

#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define pink 5
#define yellow 6
#define white 7

void joystick_init(void);
void setupRight(void);
void setupLeft(void);
void setupUp(void);
void setupDown(void);
void setupCenter(void);
int8_t readJoy(void);
void LED_init(void);
void setLED(uint8_t value);
void controlLED(uint8_t direction);

void debug_joy_raw(void);