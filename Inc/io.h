#ifndef IO_H_
#define IO_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

void setupClock();

#define JOY_UP (1 << 0) 
#define JOY_DOWN (1 << 1) 
#define JOY_LEFT (1 << 2) 
#define JOY_RIGHT (1 << 3) 
#define JOY_CENTER (1 << 4)
#define JOY_BUTTON1 (1 << 5)
#define JOY_BUTTON2 (1 << 5)

#define JOYSTICK_CONTROL 0 
#define KEYBOARD_CONTROL 1 
#define HAT_CONTROL 2

void setupUp();
void setupDown();
void setupRight();
void setupLeft();
void setupCenter();
void setupJoystick();
int8_t readJoystick();

#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define pink 5
#define yellow 6
#define white 7
void setupRed();
void setupGreen();
void setupBlue();
void setupLED();
void setLED(uint8_t RGB);

typedef struct {
    uint8_t prev;
} JoyStateTracker;

uint8_t joyPressed(JoyStateTracker *js, uint8_t mask);


#endif /* IO_H_ */
