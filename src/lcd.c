/*
 * lcd.c
 *
 * 4-bit mode for 16x2 LCD
 * Created: 4/25/2025 8:34:37 AM
 * Author : rollf
 */

#ifndef F_CPU
  #define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "mfrc522.h"

void display_two_lines(const char *line1, const char *line2) 
{
	lcd_clear();
	lcd_cmd(0x80); 
	lcd_str(line1);
	lcd_cmd(0xC0); 
	lcd_str(line2);
	_delay_ms(100);
}

void nybble()
{	// Toggle en
	set_en;
	_delay_ms(1);
	clear_en;
}
void lcd_init()
{
	DDRC |= (1 << rs)
	| (1 << rw)
	| (1 << en);
	
	DDRD = 0xF0;	// lower 4 bits for data
	
	_delay_ms(200);
	
	data_reg = 0x30;	// Wakeup command
	_delay_ms(5);	// Toggle en
	nybble();    	// busy
	_delay_us(160);	// Toggle en
	nybble();    	// busy
	_delay_us(160);	// Toggle en
	nybble();    	// busy
	_delay_us(160);	// Toggle en
	
	data_reg = 0x20;	// Put this here for 4-bit init (per datasheet)
	nybble();    	// Toggle en
	
	lcd_cmd(0x28);	// Function set: 4-bit/2-line
	lcd_cmd(0x0E);	// Display ON; Cursor ON
	lcd_clear();	// Clear screen
	lcd_cmd(0x80);	// Set cursor position
	
	_delay_ms(10);
}
void lcd_cmd(char cmd_out)
{
	data_reg = (cmd_out & 0xF0); //send the cmd_out to data
	clear_rs;            	// Send instruction
	clear_rw;            	// Write
	nybble();            	// Toggle en
	cmd_out = cmd_out << 4;	// Shift over by 4 bits
	data_reg = (cmd_out & 0xF0);// Put data on output port
	nybble();            	// Toggle en
}
void lcd_data(char data_out)
{
	data_reg = (data_out & 0xF0);    	//send the data_out to data
	set_rs;                	// Send data
	clear_rw;            	// Write
	nybble();            	// Toggle en
	data_out = data_out << 4;	// Shift over by 4 bits
	data_reg = (data_out & 0xF0);    	// Put data on output port
	nybble();            	// Toggle en
}
void lcd_str(const char *str)
{
	unsigned int i=0;
	while(str[i]!='\0')
	{
		lcd_data(str[i]);
		i++;
	}
}
void lcd_clear()
{
	lcd_cmd(0x01);
}
