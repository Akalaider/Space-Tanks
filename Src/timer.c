/*
 * timer.c
 *
 *  Created on: 8. jan. 2026
 *      Author: adambenali
 */
#include "timer.h"


void initTimer(int16_t interruptMS, int8_t priority){
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable clock line to timer 15;

	TIM15->CR1 &= ~(0x0B8F); // Configure timer 15 and disable counter
	TIM15->PSC = 0; // Set prescale value
	TIM15->ARR = interruptMS*64/(1+TIM15->PSC) - 1; // Set reload value
	TIM15->CR1 |= 0x0001; // Enable counter

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, priority); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); 			// Enable interrupt
}

void everyInterrupt(){
	setlcdFlag(1);
	TIM15->SR &= ~0x0001;
}

void TIM1_BRK_TIM15_IRQHandler() {
	if (sw.millisecond < 999){
		sw.millisecond++;
		everyInterrupt();
		return;
	} else {
		sw.millisecond = 0;
	}
	if (sw.second < 59){
		sw.second++;
		everyInterrupt();
		return;
	} else {
		sw.second = 0;
	}
	if (sw.minute < 59){
		sw.minute++;
		everyInterrupt();
		return;
	} else {
		sw.minute = 0;
	}

	sw.hour++;
	everyInterrupt();
	return;// Clear interrupt bit
}

void printTime(char *s){
	sprintf(s, "%02ld:%02ld:%02ld.%02ld\n", sw.hour, sw.minute, sw.second, (sw.millisecond >> 4) + (sw.millisecond >> 5) + (sw.millisecond >> 8) + (sw.millisecond >> 9) + (sw.millisecond >> 12) + (sw.millisecond >> 13) + (sw.millisecond >> 16)/*divides by ten*/);
}

uint8_t getlcdFlag(){
	return lcdFlag;
}

void setlcdFlag(uint8_t x){
	lcdFlag = x;
}

void resetTimer(){
	TIM15->CR1 &= ~(0x0001);
	sw.hour = 0;
	sw.minute = 0;
	sw.second = 0;
	sw.millisecond = 0;
}

void stTimer(){
	TIM15->CR1 ^= 0x0001;
}

