/* ################################################### File Description follows below ################################################# */

/*
 * File Name: seg7_display.c
 *
 * Project Name: BrittishTrafficLight
 *
 * Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
 * Tool Version: Xilinx SDK 2015.2
 *
 * Name: Fadi Barazi
 * Company: University of Edinburgh
 * Creation Date and Time: 14/11/2021; 15:39
 *
 * Description: This source file contains the function definitions for the function
 * 				declarations in the corresponding header file.
 *
 *				The functions contained in this source file will be used to drive the 7-Segment display.
 *				There are three functions in this file.
 *
 *				1. The displayNumber() function receives an unsigned 16-bit integer and it is used to assign the
 *				   digit number and the value to be displayed to the required variables.
 *
 *			 	2. The calculateDigits() function is used to extract the digits (of which a maximum of four can be displayed)
 *			 	   from the number to be displayed.
 *
 *			 	3. The displayDigit() function selects the segments to be displayed and displays
 *			 	   the digits on the 7-segment display. Note that the displayDigit() function operates
 *			 	   by being called by the hwTimerISR() (which s defined in the xinterruptES3.c file)
 *			 	   in order to achieve the strobing mechanism and display each digit every 4ms.
 *
 *
 *				Note: The current configuration of this file is to always display the required number
 *					  up to one decimal point. This is because the traffic light and pedestrian mechanism
 *					  will operate at 1 second and 5 seconds respectively and so it is useful to be able
 *					  to show the count to 1 decimal place.
 *
 *					  However, this can be adjusted as desired by altering the functions in this file.
 *
 *					  Furthermore, this module does not support displaying negative numbers as we are only
 *					  interested in using it for a timer functionality, which does not use negative numbers.
 */

/* ################################################### Module imports follow below ################################################# */

#include <stdio.h>
#include "xil_types.h"		// Added for integer type definitions
#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"
#include "traffic_light_mechanism.h"
#include "pedestrian_mechanism.h"
#include "trace_light.h"
#include "emergency_stop.h"
#include "input_extract.h"

/* ################################################### Global variables follow below ############################################### */

/* Defining global variables */
u8 digitDisplayed = FALSE;		//Will be updated by the displayDigit() function which is called by the hwTimerISR().
								//Hence, it is used to halt code execution until the digit has been displayed.

u8 digits[4];					//Will be used to hold the extracted digits of the number to be displayed
u8 numOfDigits;					//Will hold the length of the number in terms of digits
u8 digitToDisplay;				//Will hold the next digit in line to be displayed
u8 digitNumber;					//Will hold the digit position of the next digit that should be displayed

u8 SHOW_LIGHT = FALSE;

/* ################################################### Function definitions follow below ############################################ */

/*
 * This function displays the specified number on the 7-segment display by
 * utilising the calculateDigits() and displayDigit() functions.
 *
 * Its main purpose is to assign the digit to be displayed to the digitToDisplay
 * variable and the digit number to the digitNumber variable. This is then
 * used by the displayDigit() function to display the required number.
 *
 *
 *	Note: This function should not be used to display negative numbers.
 *		  It only deals with positive numbers as we are only interested
 *		  in being able to display a timer functionality.
 *
 * @Param
 * 	-> number - floating point number that should be shown on the 7-segment display
 *
 * @Return : None
 */
void displayNumber(float number)
{
	u8 count;

	/* Note that 9999 is the maximum number that can be displayed
	 *
	 * Therefore, to display number need to check that,
	 *
	 * 		-> number is less than or equal to 9999
	 *
	 * Otherwise, display dashes in all four segments if not in range
	 */

	if (number <= 9999)
	{
		/* Call the calculateDigits method to determine the digits of the number */
		calculateDigits(number);

		/* Do not display leading zeros in a number,
		 * but if the entire number is a zero, it should be displayed.
		 * By displaying the number from the last digit, it is easier
		 * to avoid displaying leading zeros by using the numOfDigits variable
		 */
		count = 4;

		while (count > 4 - numOfDigits)
		{
			/* We have displayed the first digit at count = 4 and hence if a decimal point is required
			 * it should be placed in the second digit display at count = 3
			 *
			 * Note: We will display a decimal point as long as the number is not 4 digits long
			 */
			if (number < 1000 && count == 3) {
				digitToDisplay = DECIMAL_POINT;
				digitNumber = count;

				/* Wait for timer interrupt to occur and ISR to finish
				 * executing digit display instructions
				 */
				while (digitDisplayed == FALSE);
				digitDisplayed = FALSE;
			}

			digitToDisplay = digits[count-1];
			digitNumber = count;
			count--;

			/* Wait for timer interrupt to occur and ISR to finish
			 * executing digit display instructions
			 */
			while (digitDisplayed == FALSE);
			digitDisplayed = FALSE;
		}

	}
	else
	{
		/* Display "----" to indicate that the number is out of range */
		count = 1;
		while (count < 5)
		{
			digitToDisplay = NUMBER_DASH;
			digitNumber = count;
			count++;

			/* Wait for timer interrupt to occur and ISR to finish
			 * executing digit display instructions
			 */

			while (digitDisplayed == FALSE);
			digitDisplayed = FALSE;
		}
	}
}

/*
 * This function is used to extract the digits of the number
 * and store them in the global digits array. This can then be used
 * by the displayNumber() function to assign the next digit to be displayed
 * which is used by the displayDigit() function.
 *
 * Note:
 *
 * 	1. The maximum number of digits that can be displayed are four.
 *  2. If the number is less than 4 digits long a decimal point
 *     will be included as well.
 *
 * @Param
 * 	-> number : floating point number that should be displayed
 *
 * @Return : None
 */
void calculateDigits(float number)
{
	u8 fourthDigit;
	u8 thirdDigit;
	u8 secondDigit;
	u8 firstDigit;

	u16 numberToInt = (u16) number;
	u8 symbol = (PEDESTRIAN_MODE == TRUE && traceFinished == TRUE) ? PEDESTRIAN_LIGHT : TRAFFIC_LIGHT;

	if (EMERGENCY_MODE) {
		symbol = NUMBER_DASH;
	}
	else if (SHOW_LIGHT == FALSE) {
		symbol = NUMBER_BLANK;
	}

	/* Check if number is up to four digits */
	if (number > 999)
	{

		fourthDigit  = numberToInt % 10;
		thirdDigit = (numberToInt / 10) % 10;
		secondDigit  = (numberToInt / 100) % 10;
		firstDigit = numberToInt / 1000;
	}

	/* Check if number is three-digits long */
	else if (number > 99 && number < 1000)
	{

		fourthDigit  = numberToInt % 10;
		thirdDigit = (numberToInt / 10) % 10;
		secondDigit  = (numberToInt / 100) % 10;
		firstDigit = TRAFFIC_LIGHT;
	}

	/* Check if number is two-digits long */
	else if (number > 9 && number < 100)
	{

		fourthDigit = numberToInt % 10;
		thirdDigit = (numberToInt / 10) % 100;
		secondDigit = 0;
		firstDigit  = TRAFFIC_LIGHT;

	}

	/* Check if number is one-digit long */
	else if (number >= 0 && number < 10)
	{

		fourthDigit = numberToInt % 10;
		thirdDigit = NUMBER_BLANK;
		secondDigit = symbol;
		firstDigit = 0;
	}

	numOfDigits = 4;

	digits[0] = firstDigit;
	digits[1] = secondDigit;
	digits[2] = thirdDigit;
	digits[3] = fourthDigit;

	/* Number less than 4 digits long then we want to display 1 decimal place as well */
	if (number < 1000) {

		digits[0] = digits[1];
		digits[1] = digits[2];
		digits[2] = digits[3];
		digits[3] = (u16) (number * BASE_10) % BASE_10; //Get first decimal point

		//numOfDigits = numOfDigits + 1;					//Decimal point implies an extra digit to display

	}

	return;
}

/*
 * This function selects the next digit in line to be displayed
 * and displays it on the 7-segment display.
 *
 * This is achieved using the macro constants defined in the
 * corresponding header file which allow us to write to the
 * segments such that the correct digit is shown.
 *
 *
 * Note: The hwTimerISR() function is used to display
 *		 the digits by calling this function every 4ms.
 *
 * @Param : None
 * @Return : None
 *
 */
void displayDigit()
{

	/* Write the digit to display to the 7-Segment display */
	switch (digitToDisplay)
	{
		case NUMBER_BLANK :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_BLANK);
			break;
		case 0 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ZERO);
			break;
		case 1 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ONE);
			break;
		case 2 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_TWO);
			break;
		case 3 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_THREE);
			break;
		case 4 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FOUR);
			break;
		case 5 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FIVE);
			break;
		case 6 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SIX);
			break;
		case 7 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SEVEN);
			break;
		case 8 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_EIGHT);
			break;
		case 9 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_NINE);
			break;
		case NUMBER_DASH :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_DASH);
			break;
		case DECIMAL_POINT :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_DECIMAL_POINT);
			break;
		case NUMBER_LIGHT_A :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, A);
			break;
		case NUMBER_LIGHT_B :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, B);
			break;
		case NUMBER_LIGHT_C :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, C);
			break;
		case PEDESTRIAN_LIGHT :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, P);
			break;
		default:
			break;
	}

	/* Select the appropriate digit of the 7-Segment display */
	if (digitNumber == 1) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FIRST_SEG);
	}
	else if (digitNumber == 2) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_SECOND_SEG);
	}
	else if (digitNumber == 3) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_THIRD_SEG);
	}
	else if (digitNumber == 4) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FOURTH_SEG);
	}

	digitDisplayed = TRUE;
	return;
}

u8 probeShowLightSwitch() {

	u16 slideSwitchIn = 0;

	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);

	u16 showLightSwitch = extractSwitches(slideSwitchIn, 2, 2);

	return showLightSwitch;

}
