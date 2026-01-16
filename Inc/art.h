#ifndef ART_H
#define ART_H

#include <stdint.h>

typedef struct {
    const char *utf8;
    uint8_t cp850;
} Utf8ToCp850;

// Print UTF‑8 string using CP850 conversion
void printCp850At(uint16_t x, uint16_t y, const char *s);

// Accessors for tank art (no globals)
const char *getTankUp(void);
const char *getTankDown(void);
const char *getTankLeft(void);
const char *getTankRight(void);
const char *getTankUpLeft(void);
const char *getTankUpRight(void);
const char *getTankDownLeft(void);
const char *getTankDownRight(void);

// Accessor for title art
const char *getTitleArt(void);
const char *getSelector(void);
const char *getBlankSelector(void);
const char *getPlayGame(void);
const char *getHighscores(void);
const char *getBack(void);
const char *getSingleplayer(void);
const char *getMultiplayer(void);
const char *getArrowLeft(void);
const char *getArrowRight(void);
const char *getArrowDown(void);
const char *getBlankArrow(void);

#endif
