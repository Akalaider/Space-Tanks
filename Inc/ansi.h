/*
 * ansi.h
 *
 *  Created on: 5. jan. 2026
 *      Author: adambenali
 */

#ifndef ANSI_H_
#define ANSI_H_
#include <stdint.h>
#include <stdio.h>

void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void clrscr();
void clreol();
void gotoxy(uint8_t x, uint8_t y);
void underline(uint8_t on);
void blink();
void inverse(uint8_t on);
void window(uint8_t cords1[2], uint8_t cords2[2], char *str);
void test(char *str);
void startGame();



#endif /* ANSI_H_ */
