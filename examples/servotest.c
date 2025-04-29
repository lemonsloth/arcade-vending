/*
 * prize dispensor.c
 *
 * Created: 4/25/2025 9:08:25 AM
 * Author : rollf
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void INIT_PORT(void);
void INIT_PWM(void);
    
// Servo pulse range is 500us to 2500us
//#define servo_open    	OCR1A = 500
//#define servo_close    	OCR1A = 2500

void INIT_PWM(void)
{
	TCCR1A |= (1 << COM1A1)	// Clear OC1A on compare match
	| (1 << WGM11);        	// Fast PWM w/ ICR1 as TOP
    
	TCCR1A |= (1 << COM1B1);// Clear OC1B on compare match
    
	TCCR1B |= (1 << WGM13) | (1 << WGM12)	// Fast PWM w/ ICR1 as TOP
	| (1 << CS11);                        	// Prescaler: 8
    
	// ICR1 = (FCPU/(f*prescaler))-1
	ICR1 = 19999;        	// ICR1 = (8,000,000/(50*8))-1
	TCNT1 = 0;            	// TCNT1 starts at 0
}

void INIT_PORT(void)
{
	DDRB |= (1 << PORTB1)	// PB1 = OC1A; OC1A -> Servo 1 input
    	| (1 << PORTB2);	// PB2 = OC1B; OC1B -> Servo 2 input
}

void servo_open(uint8_t servo)
{
	switch(servo)
	{
    	case 1:
        	OCR1A = 500;
        	break;
    	case 2:
        	OCR1B = 500;
        	break;
    	default:
        	break;
        	//servo error?
	}
}

void servo_close(uint8_t servo)
{
	switch(servo)
	{
    	case 1:
        	OCR1A = 2500;
        	break;
    	case 2:
        	OCR1B = 2500;
        	break;
    	default:
        	;
    	//servo error?
	}
}

int main (void)
{
	INIT_PWM();
	INIT_PORT();
    
	//run forever
	while(1)
	{
    	servo_open(1);
    	_delay_ms(1000);
    	servo_open(2);
    	_delay_ms(1000);
    	servo_close(2);
    	_delay_ms(1000);
    	servo_close(1);
    	_delay_ms(1000);
	}
}



