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
		char buff[4];
		byte = mfrc522_request(PICC_REQALL,str);
		itoa(byte, buff, 16);
		lcd_str(buff);
		_delay_ms(250);
		lcd_cmd(0x01);
	} 
}
