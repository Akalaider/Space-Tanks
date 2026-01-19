#ifndef BLINK_H
#define BLINK_H

#include <stdint.h>

typedef struct {
    const char *cursor;
    const char *blankCursor;
    uint16_t intervalMS;
    uint16_t counter;
    uint8_t state;
    uint16_t x, y;
} BlinkController;

#endif
