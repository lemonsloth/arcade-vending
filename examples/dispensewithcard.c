/*
 * dispensewithcard.c
 *
 * Created: 4/25/2025 8:34:37 AM
 * Author : rollf
 */

#define F_CPU 16000000UL
#include <stdlib.h>
#include "mfrc522.h"
#include "lcd.h"
#include "servo.h"
#include "spi.h"

#define button_port PINC
#define b1 PCINT11    	// Prize one
#define b2 PCINT12    	// Prize two
#define b3 PCINT13    	// Prize three
//#define enter PD2    	// Enter/RFID interrupt

#define b1_mask (button_port & (1 << b1))
//#define b2_mask (button_port & (1 << b2))
//#define b3_mask (button_port & (1 << b3))

volatile uint8_t button_flag = 0;	// Indicates button pressed

char *prize_select[] =
{
	"!CHOOSE A PRIZE!",
	">A1<  >A2<  >A3<",
};

char *selected_prize[] =
{
	"    DISPENSING  ",
	">A1<            ",
	"      >A2<      ",
	"            >A3<",
};

char *goodbye[] =
{
	"!! THANK YOU* !!",
	"   COME AGAIN   "
};

void button_init(void);    	// Sets up buttons w/ PCI
uint8_t prize_dispensed();


void button_init(void)
{
// 	EICRA |= (1 << ISC00);	// INT0 triggers low
// 	EIMSK |= (1 << INT0);	// Enable INT0
	
	PCMSK1 |= (1 << b1);    	// Enable pin change interrupt for b1
	//| (1 << b2)        	// Enable pin change interrupt for b2
	//| (1 << b3);    	// Enable pin change interrupt for b3
	
	PCICR |= (1 << PCIE1);	// Enable interrupts for PCINT8-14
}



uint8_t prize_dispensed()
{
	// Blinks prize selection
	display_two_lines(prize_select[0], prize_select[1]);
	
	if(button_flag == 0)
	return 0;
	
	switch(button_flag)
	{
		case 1:
		display_two_lines(selected_prize[0], selected_prize[1]);
		servo_open(1);
		_delay_ms(500);
		servo_close(1);
		_delay_ms(500);
		break;
		case 2:
		display_two_lines(selected_prize[0], selected_prize[2]);
		servo_open(2);
		_delay_ms(500);
		servo_close(2);
		_delay_ms(500);
		break;
		case 3:
		display_two_lines(selected_prize[0], selected_prize[2]);
		servo_open(2);
		_delay_ms(500);
		servo_close(2);
		_delay_ms(500);
		break;
		default:
		return 0;
	}
	
	button_flag = 0;
	return 1;
}

ISR(PCINT1_vect)
{
	if(b1_mask)
	{
		button_flag = 1;	// Button 1 pressed
	}
// 	else if(b2_mask)
// 	{
// 		button_flag = 2;	// Button 2 pressed
// 	}
// 	else if(b3_mask)
// 	{
// 		button_flag = 3;	// Button 3 pressed
// 	}
}




int main(void)
{
	lcd_init();            // Initialize LCD
	spi_init();            // Initialize SPI for MFRC522
	servo_init();
	button_init();
	mfrc522_init();        // Initialize MFRC522 module

	sei();

	while (1)
	{
		greetings();
		
		if(scanned)
		{
			while(!prize_dispensed());
			scanned = 0;
			display_two_lines(goodbye[0], goodbye[1]);
			_delay_ms(1000);
		}
	}
}
