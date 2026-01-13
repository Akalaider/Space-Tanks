/*
 * io.h
 *
 *  Created on: 8. jan. 2026
 *      Author: adambenali
 */

#ifndef IO_H_
#define IO_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

void setupClock();

#define up 1
#define down 2
#define left 4
#define right 8
#define center 16
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


#endif /* IO_H_ */
