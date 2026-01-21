#include "info_lines.h"
#include "movement.h"
#include "object.h"

void push_info_lcd(const object_t *player) { 
    uint8_t heart_count = getTankHealth(player);
    uint8_t bullet_count = getTankBullets(player);
    uint8_t homing_count = getTankHomings(player);

    char line0[128];
    char line1[128];
    char line2[128];
    char line3[128];
    const char *hearts;
    const char *bullets;
    const char *homings;

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
        case 3: homings = " Homing missiles: 3"; break;
        case 2: homings = " Homing missiles: 2"; break;
        case 1: homings = " Homing missiles: 1"; break;
        default: homings = " Homing missiles: 0"; break;
    }

    uint8_t lcd_buffer[512] = {0};
    snprintf(line0, sizeof(line0), "%s", bullets);
    snprintf(line1, sizeof(line1), "%s", homings);
    snprintf(line2, sizeof(line2), "%s", hearts);
    snprintf(line3, sizeof(line3), "%s", " Level 1");
    
    lcd_write_string(lcd_buffer, line0, 0);
    lcd_write_string(lcd_buffer, line1, 1);
    lcd_write_string(lcd_buffer, line2, 2);
    lcd_write_string(lcd_buffer, line3, 3);
    lcd_update(lcd_buffer);
}