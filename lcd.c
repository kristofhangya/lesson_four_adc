//LCD.c
#ifndef F_CPU
#define F_CPU (8000000UL)
#endif

#include "util/delay.h"
#include "LCD.h"

uint8_t customChar[8] = {
	0b11101,
	0b10001,
	0b10011,
	0b11010,
	0b10010,
	0b10111,
	0b10001,
	0b10001
};

extern void LCD_creatctg(){

	uint8_t i=0;
	LCD_cmd(0x40);
	while(i<8)	LCD_data(customChar[i++]);
	i=0;
	LCD_cmd(0x48);
	while(i<8) LCD_data(customChar[i++]);
	LCD_cmd(0x85);
	LCD_data(0);
}

extern void LCD_init(){

	DDRE |= 0xf0;
	DDRF |= (1<<LCD_RS) | (1<<LCD_RW) | (1<<LCD_EN);

	//iras
	PORTF &= ~(LCD_RW);
	//reset
	PORTF &= ~(1<<LCD_RS);
	PORTE = 0x20;
	LCD_clock();
	LCD_clock();
	LCD_clock();

	//4 bites uzemmod, 7x5pixel, 2soros
	LCD_cmd(0x28);
	LCD_cmd(0x28);
	LCD_cmd(0x28);
	//lcd alaphelyzetbe
	LCD_cmd(0x02);
	//ldc torlese
	LCD_cmd(0x01);
	//kurzor villog, alahuzas be, LCD be
	LCD_cmd(0x08 | (1<<0x2) | (0<<1) | (0<<0));
}

extern void LCD_busy(void){
	uint8_t busy;
	LCD_DATA_DDR &= ~(1<<PE7);
	LCD_CMD_PORT &= ~(1<<LCD_RS);
	LCD_CMD_PORT |= (1<<LCD_RW);
	do{
		busy = 0;
		LCD_CMD_PORT |= (1<<LCD_EN);
		_delay_us(1);
		busy=(LCD_DATA_PIN&(1<<PE7));
		LCD_CMD_PORT &= ~(1<<LCD_EN);
		_delay_us(1);

		LCD_CMD_PORT |= (1<<LCD_EN);
		_delay_us(1);
		LCD_CMD_PORT &= ~(1<<LCD_EN);
		_delay_us(1);

	}while(busy);

	LCD_CMD_PORT &= ~(1<<LCD_RW);
	LCD_DATA_DDR |= (1<<PE7);
}

extern void LCD_cmd(uint8_t cmd){
	LCD_busy();
	LCD_CMD_PORT &= ~(1<<LCD_RS);
	LCD_CMD_PORT &= ~(1<<LCD_RW);
	LCD_CMD_PORT &= ~(1<<LCD_EN);

	LCD_DATA_PORT &= ~(0xF0);
	LCD_DATA_PORT |= (cmd&0xF0);
	LCD_clock();

	LCD_DATA_PORT &= ~(0xF0);
	LCD_DATA_PORT |= ((cmd<<4)&0xF0);
	LCD_clock();
}

extern void LCD_data(uint8_t data){
	LCD_busy();
	LCD_CMD_PORT |= (1<<LCD_RS);
	LCD_CMD_PORT &= ~(1<<LCD_RW);
	LCD_CMD_PORT &= ~(1<<LCD_EN);

	LCD_DATA_PORT &= ~(0xF0);
	LCD_DATA_PORT |= (data&0xF0);
	LCD_clock();

	LCD_DATA_PORT &= ~(0xF0);
	LCD_DATA_PORT |= ((data<<4)&0xF0);
	LCD_clock();
}

void LCD_clock(){
	LCD_CMD_PORT |= (1<<LCD_EN);
	_delay_us(2);
	LCD_CMD_PORT &= ~(1<<LCD_EN);
	_delay_us(2);
}

extern void LCD_goto(unsigned char row, unsigned char col)
{
	if((col>=16) || (row>=4))return;
	if(row>=2)
	{
		row = row - 2;
		col = col + 16;
	}
	LCD_cmd((1<<7)|(row<<6)|col);
}

extern void LCD_Puts(char *s){
	while(*s && (*s!=13)){
		LCD_data(*s);
		s++;
	}
}

extern void LCD_shift(){
    LCD_cmd(0b00011000);
}


