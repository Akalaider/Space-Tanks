/*
 * ansi.c
 *
 *  Created on: 5. jan. 2026
 *      Author: adambenali
 */
#include "ansi.h"
#define ESC 0x1B



void fgcolor(uint8_t foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}



void bgcolor(uint8_t background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf("%c[%dm", ESC, background+40);
}

void color(uint8_t foreground, uint8_t background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
  printf("%c[m", ESC);
}

void clrscr() {
	printf("%c[2J", ESC);
}

void clreol() {
	printf("%c[2K", ESC);
}

void gotoxy(uint8_t x, uint8_t y) {
	printf("%c[%d;%dH", ESC, y, x);
}

void underline(uint8_t on) {
	uint8_t i;
	if (on) {
		i = 4;
	} else {
		i = 24;
	}
	printf("%c[%dm", ESC, i);
}

void blink(){
	printf("%c[%dm", ESC, 5);
}

void inverse(uint8_t on){
	uint8_t i;
		if (on) {
			i = 7;
		} else {
			i = 27;
		}
		printf("%c[%d%c",ESC,i,'m');

}
void window(uint8_t cords1[2], uint8_t cords2[2], char *str){
	gotoxy(cords1[0], cords1[1]);
	printf("%c",201);
	printf("%c",205);
	if (strlen(str) != 0){
		printf("%c",185);
		inverse(1);
		printf("%s",str);
		inverse(0);
		printf("%c", 206);
	} else {
		printf("%c",205);
		printf("%c",205);
	}
	printf("%c",205);
	for (uint8_t i=0; i<cords2[0]-cords1[0]-strlen(str)-4; i++){
		printf("%c",205);
	}
	printf("%c",187);
	for (uint8_t i=0; i<cords2[1]-cords1[1]-1; i++){
		printf("%c[%d%c", ESC, 1, 'D');
		printf("%c[%d%c", ESC, 1, 'B');
		printf("%c",186);
	}
	gotoxy(cords1[0], cords2[1]);
	printf("%c",200);
	for (uint8_t i=0; i<cords2[0]-cords1[0]; i++){
			printf("%c",205);
		}
	printf("%c",188);
	gotoxy(cords1[0], cords2[1]);
	for (uint8_t i=0; i<cords2[1]-cords1[1]-1; i++){
			printf("%c[%d%c", ESC, 1, 'A');
			printf("%c",186);
			printf("%c[%d%c", ESC, 1, 'D');
		}
}

void test(char *str) {
	printf("lenght is %d", strlen(str));
}

void startGame(){
	uint8_t C1[2] = {1,1};
	uint8_t C2[2] = {109,30};
	window(C1, C2, "");
}

