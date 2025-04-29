/*
 * servo.h
 *
 * Created: 4/25/2025 9:08:25 AM
 * Author : rollf
 *
 * Driver for FS5115M servo for arcade dispensing
 */
#ifndef SERVO_H_
#define SERVO_H_

#ifndef F_CPU
	#define F_CPU 16000000UL	// Default 
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void servo_init(void);               // Initializes servo
void servo_open(uint8_t servo);      // Servo pulse 500us
void servo_close(uint8_t servo);     // Servo pulse 2500us

#endif
