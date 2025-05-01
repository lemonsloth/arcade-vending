/*
 * lcd.h
 * 
 * 4-bit mode for a 16x2 LCD
 *
 * Created: 4/29/2025 9:43:38 AM
 * Author : caleb
 */ 

#ifndef LCD_H_
#define LCD_H_

#ifndef F_CPU
	#define F_CPU 16000000UL	// Default 
#endif

#include <avr/io.h>				
#include <util/delay.h>			// For delays

// LCD wire connections
#define rs PC0    	// Command mode (0) data mode (1)
#define rw PC1    	// Read mode (1) write mode (0)
#define en PC2    	// Enables data/command transmission
#define data_reg PORTD	// AVR PD4-7 <-> LCD DB4-7

// LCD control pins
#define set_rs    	PORTC |= (1 << rs)
#define set_rw    	PORTC |= (1 << rw)
#define set_en    	PORTC |= (1 << en)

#define clear_rs	PORTC &= ~(1 << rs)
#define clear_rw	PORTC &= ~(1 << rw)
#define clear_en	PORTC &= ~(1 << en)

// LCD
void lcd_init(void);    	// To initialize lcd.
void lcd_cmd(char cmd_out); // To send command to lcd.
void lcd_data(char data_out); // To send data to lcd.
void lcd_str(char *str);	// To send string, basically stripping each character and sending.
void nybble();            	// Toggles en
void display_two_lines();	// Displays two lines
void greetings(void);    	// Displays all greeting lines
void welcome_greeting(void);// Blinks welcome
void animation(void);    	// Animation!

#endif

