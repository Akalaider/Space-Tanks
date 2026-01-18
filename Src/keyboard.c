/*
 * keyboard.c
 *
 *  Created on: 15. jan. 2026
 *      Author: adambenali
 */


#include "keyboard.h"

void readkeys(char *buf){
	uint8_t t = uart_get_count();
	for (uint8_t i = 0; i < t; i++){
		buf[i] = uart_get_char();
	}
	buf[t] = '\0';
}

uint8_t charInString(char *buf, char n){
	for(uint8_t i = 0; i < 250; i++){
		if (buf[i] == n){
			return 1;
		}else if (buf[i] == '\0'){
			return 0;
		}
	}
}
