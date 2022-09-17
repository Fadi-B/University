#ifndef __SEG7_DISPLAY_H_
#define __SEG7_DISPLAY_H_

#include "xgpio.h"		// Added for XGpio object definitions

// Definitions for 7-segment BCD codes
#define DIGIT_BLANK		0xFF
#define DIGIT_ZERO 		0xC0
#define DIGIT_ONE  		0xF9
#define DIGIT_TWO  		0xA4
#define DIGIT_THREE  	0xB0
#define DIGIT_FOUR  	0x99
#define DIGIT_FIVE  	0x92
#define DIGIT_SIX  		0x82
#define DIGIT_SEVEN  	0xF8
#define DIGIT_EIGHT  	0x80
#define DIGIT_NINE  	0x90
#define DIGIT_DASH  	0xBF

//Extension for values 10 - 15 as HEX values A - F
#define A	0x88
#define B	0x83
#define C	0xC6
#define D	0xA1
#define E	0x86
#define F	0x8E

//Extension for the decimal point DP (it is  0 111 1111 in binary to turn it on due to active low configuration)
#define DP	0x7F

#define NUMBER_BLANK  	16 	// Note: since 16 cannot be a digit or hex value,
 						   	//       it is used to represent a blank digit
#define NUMBER_DASH  	17 	// Note: since 17 cannot be a digit or hex value,
 						   	//       it is used to represent "dash"

#define DECIMAL_POINT 	18	// Note: since 18 cannot be a digit or hex value,
							//		 it is used to represent the decimal point



/* Definitions for digit selection codes that determine which digit on the 7-segment display to use
 *
 * The digit selection codes follow an active low configuration
 */
#define EN_FIRST_SEG	0b0111
#define EN_SECOND_SEG	0b1011
#define EN_THIRD_SEG	0b1101
#define EN_FOURTH_SEG	0b1110

void print(char *str);

/* Function prototypes for the interrupt framework */
int set_up_interrupt_system();
void hw_timer_ISR(void *CallbackRef);

/* Function prototypes for the 7-segment display framework */
void display_number(float number);
void calculate_digits(float number);
void display_digit();

/* Utility functions */
int is_floating_point(float number);

#endif
