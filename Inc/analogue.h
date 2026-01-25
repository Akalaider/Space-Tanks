#ifndef ANALOGUE_H_
#define ANALOGUE_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

void initADC();
uint16_t readAnalogue(uint8_t channel);
uint16_t readJoystickAxis(char axis);
uint8_t readJoystick();


#endif /* ANALOGUE_H_ */
