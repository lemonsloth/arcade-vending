/*
 * lcd.c
 *
 * 4-bit mode for 16x2 LCD
 * Created: 4/25/2025 8:34:37 AM
 * Author : rollf
 */

#ifndef F_CPU
  #define F_CPU 16000000UL
  #warning "F_CPU not defined! Assuming 16MHz."
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

//== ASCII art for greetings() ======================================
char empty_str[] = "            	";
//-------------------------------
char *welcome[] = {
	"!THE PRIZE ZONE!",
	empty_str,
	"** WELCOME TO **"
};
//-------------------------------
char *redeem_flash[] = {
	"? GOT TICKETS ? ",
	"REDEEM HERE NOW!"
};
//-------------------------------
char *scroll_messages[] = {
	">> PLUSHIES! 	",
	">> CANDY!    	",
	">> KEYCHAINS!	",
	">> TOYS & FUN!   ",
	">> PRIZES AWAIT! "
};
//-------------------------------
char *art_frames[][2] = {
	{"   * 	* 	*", " * 	*   * 	"},
	{" * 	*   *	", "   * 	* 	* "},
	{"   * 	* 	*", " *   * 	*   * "}
};
// ====================================================================


// ====================================================================
void greetings()
{
	lcd_cmd(0x01);    	// Clear screen
	lcd_cmd(0x80);    	// Reset cursor
    
	welcome_greeting();
	animation();
    
	// Redeem string
	for (int i = 0; i < 2; i++)
	{
    	display_two_lines(redeem_flash[i%2],
     	empty_str);
    	_delay_ms(300);
   	 
    	// Art animation
    	for (int i = 0; i < 3; i++)
    	{
        	display_two_lines(art_frames[i][0],
         	art_frames[i][1]);
        	_delay_ms(10);
    	}
	}

	// Scrolling messages
	lcd_cmd(0xC0);                    	// Put next message on second line
	lcd_str("<< TAP TO CLAIM!");
	for (int i = 0; i < 5; i++)
	{
    	lcd_cmd(0x80);                	// Reset cursor to first line
    	lcd_str(scroll_messages[i]);	// Scroll through messages
    	_delay_ms(350);
	}
}
// ---------------------------------------------------------------------
void welcome_greeting()
{    
	lcd_str(welcome[2]);
	for(int i = 0; i <= 6; i++)
	{
    	lcd_cmd(0xC0);    	// Second line
    	lcd_str(welcome[i%2]);
    	_delay_ms(150);
	}
}
// ---------------------------------------------------------------------
void animation()
{
	const char message[] = "** WELCOME TO **";
	uint8_t length = 16;

	for (uint8_t i = 0; i <= length; i++)
	{
    	lcd_cmd(0x80); // First row start

    	// Spaces where text has been "eaten"
    	for (uint8_t j = 0; j < i; j++)
    	{
        	lcd_data(' ');
    	}

    	lcd_data('<');	// Little guy

    	// Remaining characters
    	for (uint8_t j = i + 1; j <= length; j++)
    	{
        	lcd_data(message[j]);
    	}

    	// Fill rest of line with spaces
    	for (uint8_t j = length + 2; j < 16; j++)
    	{
        	lcd_data(' ');
    	}

    	_delay_ms(30);
	}

	_delay_ms(100);
}
// ====================================================================

void display_two_lines(char *line1, char *line2) {
	lcd_cmd(0x01); // Clear
	lcd_cmd(0x80); lcd_str(line1);
	lcd_cmd(0xC0); lcd_str(line2);
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
	DDRB |= (1 << rs)
	| (1 << rw)
	| (1 << en);
	
	DDRD = 0xF0;	// lower 4 bits for data
	
	data = 0x30;	// Wakeup command
	_delay_ms(5);	// Toggle en
	nybble();    	// busy
	_delay_us(160);	// Toggle en
	nybble();    	// busy
	_delay_us(160);	// Toggle en
	nybble();    	// busy
	_delay_us(160);	// Toggle en
	
	data = 0x20;	// Put this here for 4-bit init (per datasheet)
	nybble();    	// Toggle en
	
	lcd_cmd(0x28);	// Function set: 4-bit/2-line
	lcd_cmd(0x0E);	// Display ON; Cursor ON
	lcd_cmd(0x01);	// Clear screen
	lcd_cmd(0x80);	// Set cursor position
	
	_delay_ms(10);
}
void lcd_cmd(char cmd_out)
{
	data = (cmd_out & 0xF0); //send the cmd_out to data
	clear_rs;            	// Send instruction
	clear_rw;            	// Write
	nybble();            	// Toggle en
	cmd_out = cmd_out << 4;	// Shift over by 4 bits
	data = (cmd_out & 0xF0);// Put data on output port
	nybble();            	// Toggle en
}
void lcd_data(char data_out)
{
	data = (data_out & 0xF0);    	//send the data_out to data
	set_rs;                	// Send data
	clear_rw;            	// Write
	nybble();            	// Toggle en
	data_out = data_out << 4;	// Shift over by 4 bits
	data = (data_out & 0xF0);    	// Put data on output port
	nybble();            	// Toggle en
}
void lcd_str(char *str)
{
	unsigned int i=0;
	while(str[i]!='\0')
	{
		lcd_data(str[i]);
		i++;
	}
}
