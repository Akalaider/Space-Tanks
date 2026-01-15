#ifndef MENU_H
#define MENU_H

#include "blink.h"

void drawTitleScreen(void);
void blinkSelector(const BlinkController *b);
void moveSelector(BlinkController *b, uint8_t joy);

#endif