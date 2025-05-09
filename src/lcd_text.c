/*
 * lcd_text.c
 *
 * Created: 5/7/2025 4:39:50 PM
 *  Author: caleb
 * 
 * To support LCD functionality for prize dispenser operations.
 * Contains strings + polling that greets users, displays prize 
 * options, and displays the user and balance.
 */ 

#include "avr/io.h"
#include "lcd.h"		// LCD functions
#include "lcd_text.h"	// inc file
#include "mfrc522.h"	// For poll_for_card()

uint8_t scanned = 0;	// Flag for poll_for_card()

//== ASCII text for LCD ======================================
const char empty_str[] = "                ";
//-------------------------------
const char *redeem_flash[] =
{
	"? GOT TICKETS ? ",
	"REDEEM HERE NOW!"
};
//-------------------------------
const char *scroll_messages[] =
{
	">> PLUSHIES!   ",
	">> CANDY!      ",
	">> KEYCHAINS!  ",
	">> TOYS & FUN! ",
	">> PRIZES AWAIT!"
};
//-------------------------------
const char *art_frames[][2] =
{
	{"   *  *   *", " *     *   *     "},
	{" *    *   *   ", "   *    *   * "},
	{"   *  *   *", " *   *     *   * "}
};
//-------------------------------
const char *prize_select[] =
{
	"500   750   1000",
	">A1<  >A2<  >A3<",
};
//-------------------------------
const char *selected_prize[] =
{
	"    DISPENSING  ",
	">A1<            ",
	"      >A2<      ",
	"            >A3<",
};
//-------------------------------
const char *goodbye[] =
{
	"!! THANK YOU* !!",
	"   COME AGAIN   "
};
const char *welcome[] =
{
	"!THE PRIZE ZONE!",
	empty_str,
	"** WELCOME TO **"
};
// ===============================================================

//== lcd_text ======================================

// Call this in main to greet users until flag set
void greetings()
{
	lcd_cmd(0x01);    	// Clear screen
	lcd_cmd(0x80);    	// Reset cursor
	
	welcome_greeting();	// Blinks "!THE PRIZE ZONE!"
	
	if(poll_for_card())	// Attempt to find card
	{
		scanned = 1;	// Set flag
		return;			// Exit loop
	}
	
	animation();		// Pacman-style animation eats
						// "** WELCOME TO **"
	
	if(poll_for_card())	// Attempt to find card
	{
		scanned = 1;	// Set flag
		return;			// Exit loop
	}
	// Redeem string
	for (int i = 0; i < 2; i++)
	{
		if(poll_for_card())	// Attempt to find card
		{
			scanned = 1;	// Set flag
			return;			// Exit loop
		}
		
		display_two_lines(redeem_flash[i%2],
		empty_str);
		_delay_ms(300);
		
		// Art animation
		for (int i = 0; i < 3; i++)
		{
			if(poll_for_card())	// Attempt to find card
			{
				scanned = 1;	// Set flag
				return;			// Exit loop
			}
			display_two_lines(art_frames[i][0],
			art_frames[i][1]);
			_delay_ms(10);
		}
	}

	// Scrolling messages
	lcd_cmd(0xC0);          // Put next message on second line
	lcd_str("<< TAP TO CLAIM!");
	for (int i = 0; i < 5; i++)
	{
		if(poll_for_card())	// Attempt to find card
		{
			scanned = 1;	// Set flag
			return;			// Exit loop
		}
		
		lcd_cmd(0x80);                	// Reset cursor to first line
		lcd_str(scroll_messages[i]);	// Scroll through messages
		_delay_ms(350);
	}
}
// ---------------------------------------------------------------------
void welcome_greeting()
{
	lcd_str(welcome[2]);
	for(int i = 0; i <= 6; i++)
	{
		lcd_cmd(0xC0);    	// Second line
		lcd_str(welcome[i%2]);
		_delay_ms(150);
	}
}
// ---------------------------------------------------------------------
void animation()
{
	const char message[] = "** WELCOME TO **";
	uint8_t length = 16;

	for (uint8_t i = 0; i <= length; i++)
	{
		lcd_cmd(0x80); // First row start

		// Spaces where text has been "eaten"
		for (uint8_t j = 0; j < i; j++)
		{
			lcd_data(' ');
		}

		lcd_data('<');	// Little guy

		// Remaining characters
		for (uint8_t j = i + 1; j <= length; j++)
		{
			lcd_data(message[j]);
		}

		// Fill rest of line with spaces
		for (uint8_t j = length + 2; j < 16; j++)
		{
			lcd_data(' ');
		}

		_delay_ms(30);
	}

	_delay_ms(100);
}
// ---------------------------------------------------------------------
void say_goodbye()
{
	display_two_lines(goodbye[0], goodbye[1]);
}
// ---------------------------------------------------------------------
void prompt_user()
{
	display_two_lines(prize_select[0], prize_select[1]);
}

void show_selected_prize(uint8_t p)
{
	display_two_lines(selected_prize[0],
	selected_prize[p]);
}
// ====================================================================
