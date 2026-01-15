#ifndef ANSI_H_
#define ANSI_H_

#include <stdint.h>
#include <stdio.h>


void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void clrscr();
void clreol();
void gotoxy(uint16_t r,uint16_t c);
void underline(uint8_t on);
void blink();
void blinkOff();
void inverse(uint8_t on);
void window(uint8_t P1[2],uint8_t  P2[2], char *title, uint8_t style);


#endif /* ANSI_H_ */
