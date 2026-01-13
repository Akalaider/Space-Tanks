/*
 * timer.h
 *
 *  Created on: 8. jan. 2026
 *      Author: adambenali
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"

typedef struct{
	volatile int32_t hour;
	volatile int8_t minute;
	volatile int8_t second;
	volatile int16_t millisecond;
} stopwatch;
static volatile stopwatch sw = {0};
static uint8_t lcdFlag = 0;

void initTimer(int16_t interruptMS, int8_t priority);
void TIM1_BRK_TIM15_IRQHandler();
void printTime(char *s);
uint8_t getlcdFlag();
void setlcdFlag(uint8_t x);
void resetTimer();



#endif /* TIMER_H_ */
