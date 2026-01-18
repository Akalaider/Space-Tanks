/*
 * keyboard.h
 *
 *  Created on: 15. jan. 2026
 *      Author: adambenali
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

void readkeys(char *buf);
uint8_t charInString(char *buf, char n);

#endif /* KEYBOARD_H_ */
