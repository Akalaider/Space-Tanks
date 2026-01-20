#include "lcd.h"
#include "charset.h"
#include "timer.h"

void lcd_write_string(uint8_t *buffer, char *str, uint8_t line){
    uint16_t index = 0;
    uint16_t offset;

    switch (line) {
        case 1: offset = 1 * 128; break;
        case 2: offset = 2 * 128; break;
        case 3: offset = 3 * 128; break;
        default: offset = 0; break;
    }

    uint16_t pos = 0;   // index*6

    while (str[index] != '\0' && (pos + offset) < 505){

        char c = str[index];

        // characters without spacing
        uint8_t tight = (c == '{' || c == '}' || c == '|' || c == '~');

        for (uint8_t j = 0; j < 5; j++){
            buffer[offset + pos + j] = character_data[c - 0x20][j];
        }

        // add spacing only for normal characters
        if (!tight) {
            buffer[offset + pos + 5] = 0;
            pos += 6;
        } else {
            pos += 5;
        }

        index++;
    }
}


void lcd_update(uint8_t *lcd){
	uint8_t tmp = getlcdFlag();
	if (tmp == 0) return;
	lcd_shift_left(lcd);
	lcd_push_buffer(lcd);
	setlcdFlag(0);
}

void lcd_shift_left(uint8_t *lcd){
	uint8_t tmp[4] = {lcd[128*0], lcd[128*1], lcd[128*2], lcd[128*3]};

	for (int16_t i = 0; i < 127; i++){
		for (int16_t j = 0; j < 4; j++){
			lcd[i + 128*j] = lcd[i + 128*j + 1];
		}
	}

	for (int16_t i = 0; i < 4; i++){
		lcd[128*(1+i) - 1] = tmp[i];
	}
}
