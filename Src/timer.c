#include "timer.h"
#include "blink.h"

static BlinkController *blinkPtr = NULL;
static object_t *playerTankPtr = NULL;
static uint8_t lcdFlag = 0;

// Timer for limiting tank updates (control tank)
static volatile uint8_t tankUpdateFlag = 0; 
static volatile uint16_t tankCounterMS = 0; 
static volatile uint16_t tankIntervalMS = 20; // ≈ 50 Hz

void timer_attachPlayerTank(object_t *tank) {
    playerTankPtr = tank;
}

void setTankUpdateInterval(uint16_t intervalMS) {
    tankIntervalMS = intervalMS;
}

uint8_t tankUpdateDue(void) {
    if (tankUpdateFlag) {
        tankUpdateFlag = 0;
        return 1;
    }
    return 0;
}

void timer_attachBlink(BlinkController *b) {
    blinkPtr = b;
}

void timer_detachBlink(BlinkController *b) {
    blinkPtr = NULL;
}

void initTimer(int16_t interruptMS, int8_t priority){
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable clock line to timer 15;

	TIM15->CR1 &= ~(0x0B8F); // Configure timer 15 and disable counter
	TIM15->PSC = 0; // Set prescale value
	TIM15->ARR = interruptMS*64000/(1+TIM15->PSC) - 1; // Set reload value
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
	} 
	else {
		sw.millisecond = 0;
		if (sw.second < 59){
				sw.second++;
		} 
		else {
			sw.second = 0;
			if (sw.minute < 59){
				sw.minute++;
			} 
			else {
				sw.minute = 0;
				sw.hour++;
			}
		}
	}
	// Tank timing
	tankCounterMS++;
	if (tankCounterMS >= tankIntervalMS) {
		tankCounterMS = 0;
		tankUpdateFlag = 1;
	}

	if (playerTankPtr != NULL) {

		// Cooldown countdown
		uint16_t cd = getTankCooldown(playerTankPtr);
		if (cd > 0) {
			cd--;
			playerTankPtr->c &= ~(0x3FF << 17);
			playerTankPtr->c |= (cd << 17);
		}

		// Ammo regeneration
		if (cd == 0) {  // cooldown done
			uint8_t bullets = getTankBullets(playerTankPtr);
			if (bullets < 5) {
				setTankBullets(playerTankPtr, bullets + 1);

				// restart cooldown for next regen
				playerTankPtr->c &= ~(0x3FF << 17);
				playerTankPtr->c |= (500 << 17);
			}
		}
	}



	// Blink timing
	if (blinkPtr) { 
		blinkPtr->counter++; 
		if (blinkPtr->counter >= blinkPtr->intervalMS) { 
			blinkPtr->counter = 0; 
			blinkPtr->state ^= 1; 
		} 
	}
	
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