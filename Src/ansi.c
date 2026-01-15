#define ESC 0x1B
#include "ansi.h"


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

void gotoxy(uint16_t r,uint16_t c) {
	printf("%c[%d;%df",ESC, c, r); // yx
}

void underline(uint8_t on) {
	on ? printf("%c[4m", ESC) : printf("%c[24m", ESC);
}

void blink(){
	printf("%c[%dm", ESC, 5);
}

void blinkOff() { 
  printf("%c[%dm", ESC, 25); // ANSI blink OFF 
}

void inverse(uint8_t on) {
	on ? color(0 , 7) : color(7 , 0);
}

void window(uint8_t P1[2], uint8_t P2[2], char *title, uint8_t style)
{
    clrscr();
    printf("%c[36m", ESC);

	uint8_t hor_char;
	uint8_t ver_char;
	uint8_t tl_char;
	uint8_t tr_char;
	uint8_t bl_char;
	uint8_t br_char;
	uint8_t lpad;
	uint8_t rpad;

    if (style) {
    	hor_char = 205;
    	ver_char = 186;
    	tl_char = 201;
    	tr_char = 187;
    	bl_char = 200;
    	br_char = 188;
    	lpad = 185;
    	rpad = 204;
    }
    else {
    	hor_char = 196;
		ver_char = 179;
		tl_char = 218;
		tr_char = 191;
		bl_char = 192;
		br_char = 217;
		lpad = 180;
		rpad = 195;
    }


    int width  = P2[0] - P1[0] % 2 == 0 ? P2[0] - P1[0] : P2[0] - P1[0] + 1;
    int height = P2[1] - P1[1];
    int title_len = strlen(title);

    if (width < title_len + 4)
        width = title_len + 4;   // room for title and padding

    gotoxy(P1[0], P1[1]);

    int side = (width - title_len) / 2 - 1;
    int right = side;

    printf("%c", tl_char);

    for (int i = 0; i < side; i++)
        printf("%c", hor_char);

    printf("%c", lpad);
    printf("%s", title);
    printf("%c", rpad);

    for (int i = 0; i < right; i++)
        printf("%c", hor_char);
    printf("%c", tr_char);

    printf("\n");

    for (int row = 1; row < height - 1; row++) {
    	printf("%c[%dC", ESC, P1[0] - 1); // Go forward x-coordinate spaces
        printf("%c", ver_char);
        for (int col = 0; col < width; col++)
            printf(" ");
        printf("%c\n", ver_char);
    }
    printf("%c[%dC", ESC, P1[0] - 1); // Go forward x-coordinate spaces
    printf("%c", bl_char);
    for (int i = 0; i < width; i++) {
        printf("%c", hor_char);
    }
    printf("%c", br_char);
}
