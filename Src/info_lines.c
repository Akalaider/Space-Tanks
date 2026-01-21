#include "info_lines.h"
#include "movement.h"
#include "object.h"

void push_info_lcd(const object_t *player) { 
    uint8_t health = getTankHealth(player);
    uint8_t ammo = getTankAmmo(player);

    const char *final_string;
    const char *hearts;
    const char *bullets;

    switch (health) {
        case 3: hearts = " Lives: {| {| {|"; break;
        case 2: hearts = " Lives: }~ {| {|"; break;
        case 1: hearts = " Lives: }~ }~ {|"; break;
        default: hearts = " Lives: }~ }~ }~"; break;
    }

    switch (ammo) {
        case 5: bullets = " 5"; break;
        case 4: bullets = " 4"; break;
        case 3: bullets = " 3"; break;
        case 2: bullets = " 2"; break;
        case 1: bullets = " 1"; break;
        default: bullets = " 0"; break;
    }

    uint8_t lcd_buffer[512] = {0};
    final_string = ("%c   -   %c", bullets, hearts);
    lcd_write_string(lcd_buffer, final_string, 0);
    lcd_update(lcd_buffer);
}