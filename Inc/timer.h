#include "blink.h"

#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"
#include "object.h"


typedef struct{
	volatile int32_t hour;
	volatile int8_t minute;
	volatile int8_t second;
	volatile int16_t millisecond;
} stopwatch;
static volatile stopwatch sw = {0};

void timer_attachPlayerTank(object_t *tank);
void setTankUpdateInterval(uint16_t intervalMS);
uint8_t tankUpdateDue(void);
void timer_attachBlink(BlinkController *b);
void timer_detachBlink(BlinkController *b);
void initTimer(int16_t interruptMS, int8_t priority);
void everyInterrupt();
void TIM1_BRK_TIM15_IRQHandler();
void printTime(char *s);
uint8_t getlcdFlag();
void setlcdFlag(uint8_t x);
void resetTimer();



#endif /* TIMER_H_ */
