/*
 * cardReadTest.c
 * 
 * Copyright 2013 Shimon <shimon@monistit.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
#include "spi.h"
#include "mfrc522.h"
 
 
int main()
{
	uint8_t byte;
	uint8_t str[MAX_LEN];
	
	_delay_ms(50);
	lcd_init();
	lcd_str("RFID Reader");
	
	spi_init();
	_delay_ms(1000);
	lcd_cmd(0x01);
	
	//init reader
	mfrc522_init();
	
	_delay_ms(100);
	
	//check version of the reader
	byte = mfrc522_read(VersionReg);
	if(byte == 0x92)
	{
		lcd_str("MIFARE RC522v2");
		lcd_str("Detected");
	}
	else if(byte == 0x91 || byte==0x90)
	{
		lcd_str("MIFARE RC522v1");
		lcd_str("Detected");
	}
	else
	{
		lcd_str("No reader found");
	}
	
	byte = mfrc522_read(ComIEnReg);
	mfrc522_write(ComIEnReg,byte|0x20);
	byte = mfrc522_read(DivIEnReg);
	mfrc522_write(DivIEnReg,byte|0x80);
	
	_delay_ms(1500);
	lcd_cmd(0x01);
	
	while(1)
	{
		char byte_buffer[8];
		byte = mfrc522_request(PICC_REQALL,str);
		itoa(byte,byte_buffer,16);
		lcd_str(byte_buffer);
		
		if(byte == CARD_FOUND)
		{
			byte = mfrc522_get_card_serial(str);
			if(byte == CARD_FOUND)
			{
				for(int i=0;i<16;i++)
				{
					if(i == 15) lcd_cmd(0xC0);
					char str_buffer[4];
					itoa(str[i], str_buffer, 16);
					lcd_str(str_buffer);
				
				}
				
				
				_delay_ms(2500);
			}
			else
			{
				lcd_str("Error");
			}
		}
		
		_delay_ms(1000);
		lcd_cmd(0x01);
	}
}
