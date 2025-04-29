/*
 * lcdTest.c
 *
 * Created: 4/25/2025 8:34:37 AM
 * Author : rollf
 *
 * Testing arcade LCD
 *
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int main()
{
	lcd_init();
	
	while(1)
	{
		greetings();
	}
}
