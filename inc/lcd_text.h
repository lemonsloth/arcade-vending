/*
 * lcd_text.h
 *
 * Created: 5/7/2025 4:39:03 PM
 *  Author: caleb
 *
 * To support LCD functionality for prize dispenser operations.
 * Contains strings + polling that greets users, displays prize
 * options, and displays the user and balance.
 */ 

#ifndef LCD_TEXT_H_
#define LCD_TEXT_H_

void greetings(void);    		// Displays all greeting lines
void welcome_greeting(void);	// Blinks "welcome"
void animation(void);    		// Welcome pacman-style animation
void say_goodbye(void);			// Say goodbye
void prompt_user(void);			// Prompt user to select prize
void show_selected_prize(uint8_t);// Display the selected prize

extern uint8_t scanned;			// Flag for scanning RFID

#endif /* LCD_TEXT_H_ */
