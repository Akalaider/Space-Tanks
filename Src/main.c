#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"
#include "sinus.h"
#include "ball.h"
#include "io.h"
#include "timer.h"
#include "lcd.h"
#include "analogue.h"
#include "keyboard.h"
#include "analogue.h"

int main(void)
{
	char textbuffer[8];
	ball_t ball;
	uint16_t adc;
	uart_init( 9600 );
	clrscr();
	startGame();
	setupClock();
	initAnalogue();
	while (1){
		adc = readAnalogue(1);
		gotoxy(10,10);
		printf("ADC channel 1: %04d",adc);
		adc = readAnalogue(2);
		gotoxy(10,11);
		printf("ADC channel 2: %04d",adc);
	}


//	while (1){
//		readkeys(textbuffer);
//		gotoxy(ball.x, ball.y);
//		printf("%c", 32);
//		gotoxy(2,2);
//		if (charInString(textbuffer, 'w')){
//			ball.y -= 1;
//			printf("w");
//		}
//		if (charInString(textbuffer, 's')){
//			ball.y += 1;
//			printf("s");
//		}
//		if (charInString(textbuffer, 'a')){
//			ball.x -= 1;
//			printf("a");
//		}
//		if (charInString(textbuffer, 'd')){
//			ball.x += 1;
//			printf("d");
//		}
//		gotoxy(ball.x,ball.y);
//		printf("o");
//	}



//	uint8_t lcd[512] = {0};
//	char time[16];
//	uart_init( 9600 );
//	initTimer(1000, 1);
//	lcd_init();
//	lcd_write_string(&lcd, "Hello,", 0);
//	lcd_write_string(&lcd, "world!", 1);
//	lcd_push_buffer(lcd);
//	while(1){
//		setlcdFlag(1);
//		lcd_update(&lcd);
//		printTime(time);
//		printf(time);
//	}
//
//	printf("Value = %03d",50);

//	char time[16];
//	char splittime1[16];
//	char splittime2[16];
//	int8_t joystick = 0;
//	int8_t pastvalue = 0;
//	uart_init( 9600 );
//	clrscr();
//	setupClock();
//	setupLED();
//	setupJoystick();
//	initTimer(1000, 1);
//	while(1){
//		pastvalue = joystick;
//		joystick = readJoystick();
//		if (((center & joystick ) != 0) && ((center & pastvalue) == 0)) {
//			stTimer();
//		}
//		if (((down & joystick) != 0) && ((down & pastvalue) == 0)) {
//			resetTimer();
//		}
//		if (((left & joystick) != 0) && ((left & pastvalue) == 0)) {
//			if ((TIM15->CR1 & 0x0001) != 0) {
//				printTime(splittime1);
//			} else{
//				printf(splittime1);
//			}
//		}
//		if (((right & joystick) != 0) && ((right & pastvalue) == 0)) {
//			if ((TIM15->CR1 & 0x0001) != 0) {
//				printTime(splittime2);
//				setLED(blue);
//			} else{
//				printf(splittime2);
//				setLED(green);
//			}
//
//		}
//		printTime(time);
//		printf(time);
//	}
//	setupClock();
//	setupLED();
//	setupJoystick();
//	while(1){
//		val = readJoystick();
//		if ((down & val) != 0){
//			setLED(red);
//		} else if ((up & val) != 0){
//			setLED(blue);
//		} else if((right & val) != 0){
//			setLED(yellow);
//		} else if((left & val) != 0){
//			setLED(cyan);
//		} else if ((center & val) != 0) {
//			setLED(black);
//		}
//	}

//	while(1){
//		printf("%d",readJoystick());
//	}
	/*
	startGame();
	ball_t ball;
	int32_t score;
	score = startBallGame();
	initBall(&ball,10,10,3,1);
	while(1) {
		if(ballUpdate(&ball)){
			score++;
			scoreUpdate(score);
		}

	}*/


}
