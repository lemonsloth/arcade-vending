/*
 * balanceTest.c
 *
 * Created: 4/25/2025 8:34:37 AM
 * Author : rollf
 */

#define F_CPU 16000000UL
#include <stdlib.h>
#include <string.h>
#include "mfrc522.h"
#include "lcd.h"
#include "servo.h"
#include "spi.h"
#include "lcd_text.h"

// Button defs
#define button_port PINC
#define b1 PCINT11    	// Prize one
#define b2 PCINT12    	// Prize two
#define b3 PCINT13    	// Prize three
#define b1_mask (button_port & (1 << b1))
#define b2_mask (button_port & (1 << b2))
#define b3_mask (button_port & (1 << b3))
#define DEFAULT_BALANCE 1500

volatile uint8_t button_num = 0;	// Indicates button pressed

// UID defs
#define MAX_CARDS 15

uint8_t currentUID[4];
uint8_t card_index = 0;
uint16_t current_balance = 0;
uint8_t newCard = 0;

// Card structure
typedef struct
{
	uint8_t uid[4];		// Card UID
	uint16_t tickets;	// Num tickets
	
} ArcadeCard;

ArcadeCard knownCards[MAX_CARDS] = {0};	// Static user-defined array 

uint8_t num_cards = 0;		// Initially no cards

// Dispense functions
void button_init(void);    	// Sets up buttons w/ PCI
uint8_t dispensed();
void dispense_prize(uint8_t);

// UID functions
uint8_t is_known_card(uint8_t*);
void add_new_card(uint8_t *);
void update_balance(uint8_t *, int16_t, uint8_t);
void print_balance(void);
void print_UID(void);
uint8_t can_afford(uint16_t);

void button_init(void)
{
// 	EICRA |= (1 << ISC00);	// INT0 triggers low
// 	EIMSK |= (1 << INT0);	// Enable INT0
	
	PCMSK1 |= (1 << b1)    // Enable pin change interrupt for b1
	| (1 << b2)        	// Enable pin change interrupt for b2
	| (1 << b3);    		// Enable pin change interrupt for b3
	
	PCICR |= (1 << PCIE1);	// Enable interrupts for PCINT8-14
}

// Loops until button_num has some value
// Call with while(!prize_dispensed());
uint8_t dispensed()
{
	prompt_user();							// Blinks prize selection
	
	if(button_num == 0 || button_num > 3)	// Value should not
											// outside this range
		return 0;							// Return; Loop again
	
	uint16_t prize_cost = 500 + ((button_num - 1) * 250);
	
	if (!can_afford(prize_cost))
	{
		lcd_clear();
		lcd_str("Not enough");
		lcd_cmd(0xC0);
		lcd_str("tickets...");
		_delay_ms(1500);
		button_num = 0;
		scanned = 0;
		return 1;		// Wasn't dispensed
						// but we need to exit
	}
	
	dispense_prize(button_num);				// Dispense prize
	update_balance(currentUID, prize_cost, 0);
	
	button_num = 0;		// Reset value
	scanned = 0;		// Reset scanned
	return 1;			// Indicate dispensed
}

// Dispenses the prize, according to prize no.
void dispense_prize(uint8_t servo_num)
{
	show_selected_prize(servo_num);	// Displays selected prize
	servo_open(servo_num);			// Send 2500 us pulse
	_delay_ms(750);					// Delay for 500
	servo_close(servo_num);			// Send 500 us pulse
	_delay_ms(1000);
}

// Detects pin change
ISR(PCINT1_vect)
{
	if(b1_mask)
	{
		button_num = 1;	// Button 1 pressed
	}
	else if(b2_mask)
	{
		button_num = 2;	// Button 2 pressed
	}
	else if(b3_mask)
	{
		button_num = 3;	// Button 3 pressed
	}
}

uint8_t is_known_card(uint8_t *uid)
{
	for(uint8_t i = 0; i < num_cards; i++)
	{
		if(memcmp(uid, knownCards[i].uid, 4) == 0)	// Check if uid is known
		{
			current_balance = knownCards[i].tickets;// Retrieve it's card balance
			card_index = i;		// Set current card index
			return 1;			// Known card
		}
	}
	return 0;	// Card unknown
}
void add_new_card(uint8_t *uid)
{	
	if(num_cards >= MAX_CARDS)		// Check if DB is full
	{
		lcd_clear();
		lcd_str("Card DB Full!");
		_delay_ms(2000);
		return;						// Exit function
	}
	
	// Add new card to DB
	memcpy(knownCards[num_cards].uid, uid, 4);
	knownCards[num_cards].tickets = DEFAULT_BALANCE;
	
	// Set as current card
	memcpy(currentUID, uid, 4);
	current_balance = DEFAULT_BALANCE;
	
	lcd_clear();		// Clear LCD
	lcd_str("New card added!");
	_delay_ms(1000);	// Small delay
	
	num_cards++;		// Increment card count
	newCard = 1;
}
void update_balance(uint8_t *uid, int16_t amount, uint8_t op)
{		
	if(!is_known_card(uid))
	{
		lcd_clear();
		lcd_str("Card unknown");
		return;
	}
	if(!op)			// Subtract
	{
		if (amount > knownCards[card_index].tickets)	// Prevent underflow
			knownCards[card_index].tickets = 0;
		else
			knownCards[card_index].tickets -= amount;
	}
	else			// Addition
 	{
		knownCards[card_index].tickets += amount;
	}

}

// Prints scanned card's balance
void print_balance(void)
{
	char balance_str[6];
	itoa(current_balance, balance_str, 10);
	display_two_lines("Balance: ", balance_str);
	_delay_ms(1250);
}

uint8_t can_afford(uint16_t cost)
{
	if (!is_known_card(currentUID))	// Check valid card & get index
		return 0;
	
	if (knownCards[card_index].tickets >= cost)
		return 1;
	else
		return 0;
}

void print_UID()
{
	uint8_t str[16];	// 4 byte UID
	newCard = 0;		// Reset newCard flag
	
	lcd_clear();			// Clear LCD
	
	if(mfrc522_get_card_serial(str) == CARD_FOUND)		// Returns success/fail and puts UID in str
	{
		memcpy(currentUID, str, 4);		// Copy 4 bytes from str to currentUID

		if(!is_known_card(str))
		{
			add_new_card(str);	// Add new card to database
		}
		else
		{
			lcd_str("Welcome back!");
			_delay_ms(1000);	// Small delay
		}
		print_balance();		// Print card balance
	}
	else
	{
		lcd_str("Card Read Error");	// mfrc522_get_card_serial() returned error
		_delay_ms(1000);
	}
}

int main(void)
{
	lcd_init();		// Initialize LCD
	spi_init();     // Initialize SPI for MFRC522
	servo_init();	// Initialize servos
	button_init();	// Initialize buttons
	mfrc522_init(); // Initialize MFRC522 module

	sei();			// Enable global interrupts

	while (1)
	{
		greetings();// Greet users

		if(!scanned) continue;	// Greet again if nothing scanned
		
		print_UID();
		
		if(newCard) continue;	// Skip dispensing if new card registered
		
		while(!dispensed());	// Loop until prize dispensed
		
		say_goodbye();			// Say goodbye :(
		_delay_ms(1000);		// Delay
	}
}
