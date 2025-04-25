/*
 * lab8_1.c
 *
 * Created: 3/28/2025 8:34:37 AM
 * Author : rollf
 */

#define F_CPU 1000000UL
#include<avr/io.h>
#include<util/delay.h>

//definition for the wire connections
#define rs PB0    	// command mode (0) data mode (1)
#define rw PB1    	// read mode (1) write mode (0)
#define en PB2    	// enables data/command transmission
#define data	PORTC	// Extracts lower 4 bits

#define set_rs    	PORTB |= (1 << rs)
#define set_rw    	PORTB |= (1 << rw)
#define set_en    	PORTB |= (1 << en)

#define clear_rs	PORTB &= ~(1 << rs)
#define clear_rw	PORTB &= ~(1 << rw)
#define clear_en	PORTB &= ~(1 << en)

// function prototypes
void lcd_init(); //to initialize lcd.
void lcd_cmd(char cmd_out); //to send command to lcd.
void lcd_data(char data_out); //to send data to lcd.
void lcd_str(char *str); //to send string, basically stripping each character and sending.
void nybble();

void INIT_PORT(void);

void INIT_PORT()
{
	DDRB |= (1 << rs)
	| (1 << rw)
	| (1 << en);
    
	DDRC = 0x0F;	// lower 4 bits for data
}

int main()
{
	INIT_PORT();
    
	lcd_init();
	lcd_str("Rollf, Caleb");
	lcd_cmd(0xC0);
	lcd_str("CPE301L: Lab 6");
    
	while(1)
	{

	}
}
void nybble()
{	// Toggle en
	set_en;
	_delay_ms(10);
	clear_en;
}
void lcd_init()
{
	lcd_cmd(0x38);	// Initializing to 2 lines & 5x7 font.
	lcd_cmd(0xE);	// Display on, cursor on
	lcd_cmd(0x1);	// Clear LCD
	lcd_cmd(0x80);	// Set cursor position to top row 0x80
}
void lcd_cmd(char cmd_out)
{
	data = cmd_out; //send the cmd_out to data
	clear_rs;            	// Send instruction
	clear_rw;            	// Write
	nybble();            	// Toggle en
	cmd_out = cmd_out << 4;	// Shift over by 4 bits
	data = cmd_out;        	// Put data on output port
	nybble();            	// Toggle en
}
void lcd_data(char data_out)
{
    
	data = data_out;    	//send the data_out to data
	set_rs;                	// Send data
	clear_rw;            	// Write
	nybble();            	// Toggle en
	data_out = data_out << 4;	// Shift over by 4 bits
	data = data_out;    	// Put data on output port
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

