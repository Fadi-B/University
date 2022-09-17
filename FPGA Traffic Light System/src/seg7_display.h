#ifndef __SEG7_DISPLAY_H_
#define __SEG7_DISPLAY_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: seg7_display.h
 *
 * Project Name: BrittishTrafficLight
 *
 * Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA
 * Tool Version: Xilinx SDK 2015.2
 *
 * Name: Fadi Barazi
 * Company: University of Edinburgh
 * Creation Date and Time: 14/11/2021; 15:39
 *
 * Description: This header file contains the function declarations that will be defined
 * 				in the seg7_display.h file. More importantly, it contains macro
 * 				definitions that are used to enhance code readability when writing
 * 				to the 7-Segment Display GPIO ports.
 *
 * 				The 7-Segment Display on the Basys3 board follows an active low configuration.
 *
 * 				The SEG7_HEX_OUT GPIO is an 8-bit port, in which the bits corresponds to
 * 				the value of segments CP, CG, CF , CE, CD, CC, CB, CA. Hence, in order to display different numbers
 * 				and symbols when writing to the SEG7_HEX_OUT object we have to set the bits for the
 * 				segments we want to show as 0 and otherwise 1.
 *
 * 					-> This is how the macro constants for the different digits and symbols have been constructed
 *
 * 				The SEG7_SEL_OUT GPIO is a 4 bit port, in which the bits correspond to which
 * 				digit of the 7-segment display to select. This also follows an active low
 * 				configuration and so the corresponding bit of the digit to be selected should be set
 *				0 otherwise 1. Only 1 digit can be selected at a time, which is why a strobing mechanism
 *				will be used to display all the required digits.
 *
 *					-> This is how the macro constant for the 7 segment digit selection codes have been constructed
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "xgpio.h"		// Added for xgpio object definitions

/* ################################################### Macro constants follow below ################################################# */

/* Definitions for 7-segment BCD codes that will be
 * used in writing the SEG7_HEX_OUT GPIO object */
#define DIGIT_BLANK			0xFF
#define DIGIT_ZERO 			0xC0
#define DIGIT_ONE  			0xF9
#define DIGIT_TWO  			0xA4
#define DIGIT_THREE  		0xB0
#define DIGIT_FOUR  		0x99
#define DIGIT_FIVE  		0x92
#define DIGIT_SIX  			0x82
#define DIGIT_SEVEN  		0xF8
#define DIGIT_EIGHT  		0x80
#define DIGIT_NINE  		0x90
#define DIGIT_DASH  		0xBF
#define DIGIT_DECIMAL_POINT	0x7F

/* Integer encodings for special symbols */
#define NUMBER_BLANK  	16 	// Note: since 16 cannot be a digit,
 						   	//       it is used to represent a "blank" digit
#define NUMBER_DASH  	17 	// Note: since 17 cannot be a digit,
 						   	//       it is used to represent "dash"
#define DECIMAL_POINT 	18	// Note: since 18 cannot be a digit,
							//		 it is used to represent the decimal point

/* Definition for the number base of the
 * number that will be displayed */
#define BASE_10 10


#define NUMBER_LIGHT_A 19
#define NUMBER_LIGHT_B 20
#define NUMBER_LIGHT_C 21
#define PEDESTRIAN_LIGHT 22

#define A	0x88
#define B	0x83
#define C	0xC6
#define P   0x8C

/* Definitions for digit selection codes */
#define EN_FIRST_SEG	0b0111
#define EN_SECOND_SEG	0b1011
#define EN_THIRD_SEG	0b1101
#define EN_FOURTH_SEG	0b1110

extern u8 SHOW_LIGHT;

/* ################################################### Function declarations follow below ############################################ */

void displayNumber(float number);
void calculateDigits(float number);
void displayDigit();
u8 probeShowLightSwitch();

#endif /* SEG7_DISPLAY_H_ */
