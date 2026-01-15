#include "art.h"
#include <string.h>
#include <stdio.h>

// PRIVATE map — not visible outside this file
static const Utf8ToCp850 map[] = {
    {"█", 0xDB},
    {"╗", 0xBB},
    {"╔", 0xC9},
    {"╝", 0xBC},
    {"╚", 0xC8},
    {"═", 0xCD},
    {"║", 0xBA},
    {"╠", 0xCC},
    {"╣", 0xB9},
    {"╦", 0xCB},
    {"╩", 0xCA},
    {"╬", 0xCE},
};

static const uint16_t mapCount = sizeof(map) / sizeof(map[0]);

void printCp850At(uint16_t x, uint16_t y, const char *s) { 
    uint16_t row = 0; 
    gotoxy(x, y); 
    while (*s) { 
        if (*s == '\n') { 
            row++; 
            gotoxy(x, y + row); 
            s++; 
            continue; 
        } 
        uint16_t matched = 0; 
        for (uint16_t i = 0; i < mapCount; i++) { 
            uint16_t len = strlen(map[i].utf8); 
            if (strncmp(s, map[i].utf8, len) == 0) { 
                putchar(map[i].cp850); 
                s += len; 
                matched = 1; 
                break; 
            } 
        } 
        if (!matched) { 
            putchar(*s); 
            s++; 
        } 
    } 
}



static const char tankUp[] =
"   ||   \n"
"   ||   \n"
"  ^ ^ ^ \n"
"|::::::|\n"
" \\----/ \n";


static const char tankDown[] =
" /----\\ \n"
"|::::::|\n"
" V V V  \n"
"   ||   \n"
"   ||   \n";

static const char tankLeft[] =
"    __  \n"
"   <::\\ \n"
"===<:::|\n"
"   <::/ \n"
"    --  \n";

static const char tankRight[] =
"   __   \n"
" /::>   \n"
"|:::>===\n"
" \\::>   \n"
"   --   \n";

static const char tankUpLeft[] =
"\\\\      \n"
" \\\\     \n"
" ^ ^ ^  \n"
"|::::::|\n"
" \\----/ \n";
static const char tankUpRight[] =
"      //\n"
"     // \n"
"  ^ ^ ^ \n"
"|::::::|\n"
" \\----/ \n";

static const char tankDownLeft[] =
" /----\\ \n"
"|::::::|\n"
" V V V  \n"
" //     \n"
"//      \n";

static const char tankDownRight[] =
" /----\\ \n"
"|::::::|\n"
"  V V V \n"
"     \\\\\n"
"      \\\\\n";

static const char titleArt[] =
"███████╗██████╗  █████╗  ██████╗███████╗    ████████╗ █████╗ ███╗   ██╗██╗  ██╗███████╗\n"
"██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝    ╚══██╔══╝██╔══██╗████╗  ██║██║ ██╔╝██╔════╝\n"
"███████╗██████╔╝███████║██║     █████╗         ██║   ███████║██╔██╗ ██║█████╔╝ ███████╗\n"
"╚════██║██╔═══╝ ██╔══██║██║     ██╔══╝         ██║   ██╔══██║██║╚██╗██║██╔═██╗ ╚════██║\n"
"███████║██║     ██║  ██║╚██████╗███████╗       ██║   ██║  ██║██║ ╚████║██║  ██╗███████║\n"
"╚══════╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚══════╝       ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝\n";

static const char selector[] = 
" _______ \n"
"/\\______\\\n"
"\\/______/\n";

static const char blankSelector[] =
"         \n"
"         \n"
"         \n";

static const char playGame[] = 
"   ______   __         ______     __  __        ______     ______     __    __     ______   \n"
"  /\\  == \\ /\\ \\       /\\  __ \\   /\\ \\_\\ \\      /\\  ___\\   /\\  __ \\   /\\ \"-./  \\   /\\  ___\\  \n"
"  \\ \\  _-/ \\ \\ \\____  \\ \\  __ \\  \\ \\____ \\     \\ \\ \\__ \\  \\ \\  __ \\  \\ \\ \\-./\\ \\  \\ \\  __\\  \n"
"   \\ \\_\\    \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\     \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\ \\ \\_\\  \\ \\_____\\\n"
"    \\/_/     \\/_____/   \\/_/\\/_/   \\/_____/      \\/_____/   \\/_/\\/_/   \\/_/  \\/_/   \\/_____/\n";

static const char highscores[] = 
" __  __     __     ______     __  __     ______     ______     ______     ______     ______     ______   \n"
"/\\ \\_\\ \\   /\\ \\   /\\  ___\\   /\\ \\_\\ \\   /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\   /\\  ___\\   /\\  ___\\  \n"
"\\ \\  __ \\  \\ \\ \\  \\ \\ \\__ \\  \\ \\  __ \\  \\ \\___  \\  \\ \\ \\____  \\ \\ \\/\\ \\  \\ \\  __<   \\ \\  __\\   \\ \\___  \\ \n"
" \\ \\_\\ \\_\\  \\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\  \\ \\_____\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\/\\_____\\\n"
"  \\/_/\\/_/   \\/_/   \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/   \\/_____/   \\/_/ /_/   \\/_____/   \\/_____/\n";

const char *getTankUp(void)        { return tankUp; }
const char *getTankDown(void)      { return tankDown; }
const char *getTankLeft(void)      { return tankLeft; }
const char *getTankRight(void)     { return tankRight; }
const char *getTankUpLeft(void)    { return tankUpLeft; }
const char *getTankUpRight(void)   { return tankUpRight; }
const char *getTankDownLeft(void)  { return tankDownLeft; }
const char *getTankDownRight(void) { return tankDownRight; }

const char *getTitleArt(void)      { return titleArt; }
const char *getSelector(void)      { return selector; }
const char *getBlankSelector(void) { return blankSelector; }
const char *getPlayGame(void)      { return playGame; }
const char *getHighscores(void)    { return highscores; }