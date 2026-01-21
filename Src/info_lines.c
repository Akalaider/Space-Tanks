#include "info_lines.h"
#include "movement.h"
#include "object.h"

void push_info_lcd(const object_t *player) { 
    uint8_t heart_count = getTankHealth(player);
    uint8_t bullet_count = getTankBullets(player);
    uint8_t homing_count = getTankHomings(player);

    char final_string[512];
    const char *hearts;
    const char *bullets;

    switch (heart_count) {
        case 3: hearts = " Lives: {| {| {|"; break;
        case 2: hearts = " Lives: }~ {| {|"; break;
        case 1: hearts = " Lives: }~ }~ {|"; break;
        default: hearts = " Lives: }~ }~ }~"; break;
    }

    switch (bullet_count) {
        case 5: bullets = " Bullets: 5"; break;
        case 4: bullets = " Bullets: 4"; break;
        case 3: bullets = " Bullets: 3"; break;
        case 2: bullets = " Bullets: 2"; break;
        case 1: bullets = " Bullets: 1"; break;
        default: bullets = " Bullets: 0"; break;
    }

    switch (homing_count) {
        case 
    }

    uint8_t lcd_buffer[512] = {0};
    snprintf(final_string, sizeof(final_string), "%s          %s", bullets, hearts);
    lcd_write_string(lcd_buffer, final_string, 0);
    lcd_update(lcd_buffer);
}