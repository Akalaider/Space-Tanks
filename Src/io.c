/*
 * io.c
 *
 *  Created on: 8. jan. 2026
 *      Author: adambenali
 */

#include "io.h"

#define JOY_MASK 0x1F   // UP, DOWN, LEFT, RIGHT, CENTER

uint8_t joyPressed(JoyStateTracker *js, uint8_t mask) { 
    uint8_t now = readJoystick() & JOY_MASK; 
    
    // Active-low: 1 (released) -> 0 (pressed) 
    uint8_t pressed = (js->prev & ~now) & mask; 
    js->prev = now; 
    return pressed; 
}

void setupClock(){
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C
}
void setupUp(){
	// Set pin PA4 to input
	GPIOA->MODER &= ~(0x00000003 << (4 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000000 << (4 * 2)); // Set mode register (0x00 –
		//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOA->PUPDR &= ~(0x00000003 << (4 * 2)); // Clear push/pull register
	GPIOA->PUPDR |= (0x00000002 << (4 * 2));
}

void setupDown(){
	// Set pin PB0 to input
	GPIOB->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –
		//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000002 << (0 * 2));
}

void setupRight(){
	// Set pin PC0 to input
	GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –
		//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (0 * 2));
}

void setupLeft(){
	// Set pin PC1 to input
	GPIOC->MODER &= ~(0x00000003 << (1 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (1 * 2)); // Set mode register (0x00 –
		//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (1 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (1 * 2));
}

void setupCenter(){
	// Set pin PB5 to input
	GPIOB->MODER &= ~(0x00000003 << (5 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000000 << (5 * 2)); // Set mode register (0x00 –
		//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR &= ~(0x00000003 << (5 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000002 << (5 * 2));
}

void setupJoystick(){
	setupUp();
	setupDown();
	setupRight();
	setupLeft();
	setupCenter();

}

int8_t readJoystick(){
	return 0x1F & (((GPIOA->IDR & (0x0001 << 4)) >> (-0+4))
				| ((GPIOB->IDR & (0x0001 << 0)) << (1-0))
				| ((GPIOC->IDR & (0x0001 << 1)) << (2-1))
				| ((GPIOC->IDR & (0x0001 << 0)) << (3-0))
				| ((GPIOB->IDR & (0x0001 << 5)) >> (-4+5)));
}

void setupRed(){
	// Set pin PB4 to output
	GPIOB->OSPEEDR &= ~(0x00000003 << (4 * 2)); // Clear speed register
	GPIOB->OSPEEDR |= (0x00000002 << (4 * 2)); // set speed register (0x01 - 10
	//MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER &= ~(0x0001 << (4 * 1)); // Clear output type register
	GPIOB->OTYPER |= (0x0000 << (4 * 1)); // Set output type register (0x00 -
	// Push pull, 0x01 - Open drain)
	GPIOB->MODER &= ~(0x00000003 << (4 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000001 << (4 * 2)); // Set mode register (0x00 –
	// Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
}

void setupGreen(){
	// Set pin PC7 to output
	GPIOC->OSPEEDR &= ~(0x00000003 << (7 * 2)); // Clear speed register
	GPIOC->OSPEEDR |= (0x00000002 << (7 * 2)); // set speed register (0x01 - 10
	//MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOC->OTYPER &= ~(0x0001 << (7 * 1)); // Clear output type register
	GPIOC->OTYPER |= (0x0000 << (7 * 1)); // Set output type register (0x00 -
	// Push pull, 0x01 - Open drain)
	GPIOC->MODER &= ~(0x00000003 << (7 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000001 << (7 * 2)); // Set mode register (0x00 –
	// Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
}

void setupBlue(){
	// Set pin PA9 to output
	GPIOA->OSPEEDR &= ~(0x00000003 << (9 * 2)); // Clear speed register
	GPIOA->OSPEEDR |= (0x00000002 << (9 * 2)); // set speed register (0x01 - 10
	//MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOA->OTYPER &= ~(0x0001 << (9 * 1)); // Clear output type register
	GPIOA->OTYPER |= (0x0000 << (9 * 1)); // Set output type register (0x00 -
	// Push pull, 0x01 - Open drain)
	GPIOA->MODER &= ~(0x00000003 << (9 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000001 << (9 * 2)); // Set mode register (0x00 –
	// Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
}

void setupLED(){
	setupRed();
	setupGreen();
	setupBlue();
	setLED(0b000);
}

void setLED(uint8_t RGB){
	if(0x1 & RGB){
		GPIOA->ODR &= (0x0000 << 9);
	} else {
		GPIOA->ODR |= (0x0001 << 9);
	}

	if((0x2 & RGB) >> 1){
		GPIOC->ODR &= (0x0000 << 7);
	} else {
		GPIOC->ODR |= (0x0001 << 7);
	}

	if((0x4 & RGB) >> 2){
		GPIOB->ODR &= (0x0000 << 4);
	} else {
		GPIOB->ODR |= (0x0001 << 4);
	}
}
