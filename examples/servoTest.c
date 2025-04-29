/*
 * servoTest.c
 *
 * Created: 4/25/2025 9:08:25 AM
 * Author : rollf
 *
 * Servo Test for arcade dispensing
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "servo.h"

int main (void)
{
	servo_init();
	//run forever
	while(1)
	{
    	servo_open(1);
    	_delay_ms(500);
    	servo_open(2);
    	_delay_ms(500);
    	servo_close(2);
    	_delay_ms(500);
    	servo_close(1);
    	_delay_ms(500);
	}
}
