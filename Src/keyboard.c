#include "keyboard.h"
#include "io.h"

uint8_t readKeysFromBuffer(char *buf) {
	uint8_t joy = 0;

    if (charInString(buf, 'w')) joy |= JOY_UP;
    if (charInString(buf, 'x')) joy |= JOY_DOWN;
    if (charInString(buf, 'a')) joy |= JOY_LEFT;
    if (charInString(buf, 'd')) joy |= JOY_RIGHT;
    if (charInString(buf, 'q')) joy |= (JOY_UP | JOY_LEFT);
    if (charInString(buf, 'e')) joy |= (JOY_UP | JOY_RIGHT);
    if (charInString(buf, 'z')) joy |= (JOY_DOWN | JOY_LEFT);
    if (charInString(buf, 'c')) joy |= (JOY_DOWN | JOY_RIGHT);
    if (charInString(buf, ' ')) joy |= JOY_BUTTON1;
    if (charInString(buf, 's')) joy |= JOY_CENTER;

    return joy;
}


void readKeyboardBuffer(char *buf) {
    uint8_t t = uart_get_count();
    if (t > 31) t = 31;

    // Only update buffer if we have new data
    if (t > 0) {
        for (uint8_t i = 0; i < t; i++) {
            buf[i] = uart_get_char();
            gotoxy(20,20);
            printf("%c", buf[i]);
        }
        buf[t] = '\0';
    }
    // If t == 0, keep the previous buffer contents
}

uint8_t charInString(char *buf, char n){
    for (uint8_t i = 0; buf[i] != '\0'; i++){
        if (buf[i] == n)
            return 1;
    }
    return 0;
}