//LCD.h
#ifndef _LCD_H_
#define _LCD_H_

#include <inttypes.h>
#include <avr/io.h>

#define LCD_CMD_DDR		(DDRF)
#define LCD_DATA_DDR	(DDRE)

#define LCD_CMD_PORT	(PORTF)
#define LCD_DATA_PORT	(PORTE)
#define LCD_DATA_PIN	(PINE)

#define LCD_RS			(PF1)
#define LCD_RW			(PF2)
#define LCD_EN			(PF3)

extern void LCD_init();
extern void LCD_cmd(uint8_t cmd);
void LCD_clock();
extern void LCD_data(uint8_t data);
extern void LCD_Puts(char *s);
extern void LCD_goto(unsigned char row, unsigned char col);
extern void LCD_shift();

#endif
