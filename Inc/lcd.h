/*
 * lcd.h
 *
 *  Created on: 9. jan. 2026
 *      Author: adambenali
 */

#ifndef LCD_H_
#define LCD_H_
#include <stdint.h>
#include <stdio.h>

void lcd_write_string(uint8_t *buffer,char *str, uint8_t line);
void lcd_update(uint8_t *lcd);
void lcd_shift_left(uint8_t *lcd);


#endif /* LCD_H_ */
