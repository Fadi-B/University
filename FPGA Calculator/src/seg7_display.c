#include <stdio.h>
#include "xil_types.h"		// Added for integer type definitions
#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"

#include "base.h"			// Added to represent different number bases
#include "mode.h"			// Added to determine different modes of operations such as hex or radian

u8 digitDisplayed = FALSE;  //Will be used to stop code progression until digit is displayed
u8 digits[4];				//Will store the individual digits that should be displayed
u8 numOfDigits;				//Will hold the number of digits to be displayed
u8 digitToDisplay;			//Will hold the digit that should be displayed
u8 digitNumber;				//Will hold the digit display that should be selected

/*
 * This function displays the specified number on the 7-segment display by
 * utilizing the calculateDigits and displayDigit functions.
 *
 * @Param
 * 	-> number - the number that should be shown on the 7-segment display
 */
void display_number(float number)
{
	u8 count;
	u8 isNegative = 0;			//This will be 1 if the specified number is negative else it will be 0
	u8 isFloat = is_floating_point(number);	//Will be used to determine if a decimal point should be displayed

	if (number < 0) {
		isNegative = 1;
	}

	/* Note that 9999 is the maximum number that can be displayed in decimal and -999 is the minimum number
	 * that can be displayed in decimal as the negative sign occupies last digit display.
	 *
	 * Similarly, FFFF is the maximum number that can be displayed in hex and -FFF is the minimum number that
	 * can be displayed in hex.
	 *
	 * Therefore, to display number need to check that either,
	 *
	 * 		-> number is less than or equal to 9999 and that it is larger than or equal to -999 if decimal mode
	 * 		or
	 * 		-> number is less than or equal to 0xFFFF and that it is larger than or equal to -0xFFF if hex mode
	 *
	 * Otherwise, display dashes in all four segments if not in range
	 */

	if ((number <= 9999 && number >= -999) || (number <= 0xFFFF && number >= -(0xFFF) && NUMBER_MODE == HEX))
	{
		// Call the calculateDigits method to determine the digits of the number

		calculate_digits(number);
		/* Do not display leading zeros in a number,
		 * but if the entire number is a zero, it should be displayed.
		 * By displaying the number from the last digit, it is easier
		 * to avoid displaying leading zeros by using the numOfDigits variable
		 */
		count = 4;

		/* Boolean values for determining if the decimal point is in range if the number is float */
		int isFloatDecimalInRange = (number < 1000) && (number >= -100) && isFloat;
		int isFloatHexInRange = (number < 0xFFF) && (number >= -(0xFF)) && (NUMBER_MODE == HEX) && isFloat;

		while (count > 4 - numOfDigits)
		{
			/* We have displayed the first digit at count = 4 and hence if a decimal point is required
			 * it should be placed in the second digit display at count = 3
			 */
			if ((isFloatDecimalInRange && count == 3) || (isFloatHexInRange && count == 3)) {
				digitToDisplay = DECIMAL_POINT;
				digitNumber = count;

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

		/* Negative number means we need a negative sign in front */
		if (isNegative && count >= 0) {
			digitToDisplay = NUMBER_DASH;
			digitNumber = count;
		}

		while(digitDisplayed == FALSE);
		digitDisplayed = FALSE;
	}
	else
	{
		// Display "----" to indicate that the number is out of range
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
 * This extracts the individual digits from the specified number
 * and stores them in the global digits[4] array. The digits[4] array
 * is set up such that
 *
 * 		digits[3] -> digits[0] <=> Least significant value -> Most significant value
 *
 * to aid displaying the number later on.
 *
 * Note: This function supports showing up to one decimal point if the specified number
 * 		 is float. Hence, a number is considered float if it has a non-zero value at the
 * 		 first decimal point and otherwise it is considered an integer. Determining if
 * 		 a number is float or not based on this definition is done using the utility
 * 		 function isFloatingPoint.
 *
 *
 * @Param
 * 	-> number - the number that should be shown on the 7-segment display
 */
void calculate_digits(float number)
{
	float absolute = number;

	if (number < 0) {
		absolute = (-1) * number;		//Dealing with the magnitude simplifies operations
	}

	struct base representation;			//Will store the required number representation

	if (NUMBER_MODE == HEX) {
		representation = change_number_base(number, BASE_16);
	} else {
		representation = change_number_base(number, BASE_10);
	}

	int i = 0;

	for (i; i < DIGITS; i++) {			//Copy over number representation into the digits array using the required
										//format: digits[3] -> digits[0] <=> Least significant value -> Most significant valye
		digits[3 - i] = representation.digits[i];
	}

	numOfDigits = representation.numOfDigits;

	int isFloat = is_floating_point(number); //Will hold boolean indicating if float or not

	//Need to check if float is within range of display before deciding to include the decimal point
	if ((number < 1000) && (number >= -100) && isFloat && (NUMBER_MODE == DECIMAL)) {

		digits[0] = digits[1];
		digits[1] = digits[2];
		digits[2] = digits[3];
		digits[3] = (u16) (absolute * BASE_10) % BASE_10; //Get first decimal point

		numOfDigits = numOfDigits + 1;					  //Decimal point implies an extra digit to display
	}

	else if ((number < 0xFFF) && (number >= -(0xFF)) && isFloat && (NUMBER_MODE == HEX)) {

		digits[0] = digits[1];
		digits[1] = digits[2];
		digits[2] = digits[3];
		digits[3] = (u16) (absolute * BASE_16) % BASE_16; //Get first hexadecimal point

		numOfDigits = numOfDigits + 1;					  //Hexadecimal point implies an extra digit to display
	}

	return;
}

/*
 * This function displays the global digitToDisplay variable in its corresponding
 * 7-segment display cell based on the digitNumber global variable
 *
 * Note: The function is based on the macro constants defined in seg7_display.h
 */
void display_digit()
{
	/*
	 * This timer ISR is used to display the digits
	 */
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
		case 10 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, A);
			break;
		case 11 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, B);
			break;
		case 12 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, C);
			break;
		case 13 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, D);
			break;
		case 14 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, E);
			break;
		case 15 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, F);
			break;
		case NUMBER_DASH :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_DASH);
			break;
		case DECIMAL_POINT:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DP);
		default:
			break;
	}

	// Select the appropriate digit
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


/*
 * This function determines whether the provided number should
 * be considered a floating point number in regard to the
 * calculator features.
 * 		-> If the number has a 0 in the first decimal point then consider it integer
 * 		-> If the number has a non-zero value in the first decimal point then consider it a float
 *
 * @Param
 * 	-> number - the specified number that should be examined
 *
 * @Return
 * 	-> returns integer that is 1 if number is float else 0
 */
int is_floating_point(float number) {

	if (number < 0) {
		number = number * (-1); //Deal only with magnitude
	}

	int toInt = (int) (number * 10);
	int firstDigit = toInt % 10;

	if (firstDigit) {
		return 1;
	}

	return 0;
}
