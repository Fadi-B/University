#include <stdio.h>
#include "base.h"

/*
 * This function changes the base of the specified number based
 * on the conversion base provided.
 *
 * Note: Function takes a float number, but the base conversion
 * 		 will only consider the integer value. Dealing with decimal
 * 		 points is outsourced to the calculateDigits function in seg7_display.c
 * 		 as the allowable display range for float values will depend on
 * 		 the number mode.
 *
 * 		 Float value is still used as a parameter since need to determine
 * 		 if value is between 0 and 1 for numOfDigits as if no whole integers
 * 		 then there will still be one digit output on the 7-segment display
 *
 * @Param
 * 	-> number - the floating point number whose base should be changed
 * 	-> conversion_base - the new base that should be used
 *
 * @Return
 * 	-> a base struct that contains the new representation
 */
struct base change_number_base(float number, u16 conversionBase)
{

	char *digits = base.digits;

	if (digits == NULL) {	//Check if we have already allocated memory
		digits = malloc(sizeof(DIGITS * sizeof(char)));
	}

	int i = 0;

	for (i; i < DIGITS; i++) { //Initialize the values to non-null
		digits[i] = 0;
	}

	if (number < 0) {
		number = number * (-1); //Turn into positive to operate on magnitude
	}

	int counter = 0;
	int processedNumber = (int) number;	//Cast to int to only deal with integer value

	/* while loop stores new representation in base struct */
	while (processedNumber != 0) {

		int modulo = 0;

		modulo = processedNumber % conversionBase;

		digits[counter] = modulo;
		counter++;

		processedNumber = processedNumber / conversionBase;

	}

	if (number < 1) {	//Less than 1 then number of digits is 1 and not 0
		base.numOfDigits = 1;
	} else {
		base.numOfDigits = counter;
	}

	return base;
}
