#include "info_lines.h"

void push_health(const object_t *player) { 
    uint8_t health = (player->c >> 2) & 0x03;
    const char *hearts;
    char *lives = "              Lives:";

    if (health == 3) {
        hearts = "              {| {| {|";
    }
    else if (health == 2) {
        hearts = "              }~ {| {|";
    }
    else {
        hearts = "              }~ }~ {|";
    }

    uint8_t lcd_buffer[512] = {0};
    lcd_write_string(lcd_buffer, lives, 0);
    lcd_write_string(lcd_buffer, hearts, 1);
    lcd_update(lcd_buffer);
}
