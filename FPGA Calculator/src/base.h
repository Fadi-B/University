#ifndef BASE_H_
#define BASE_H_

#include "xil_types.h" //Used for u16 and u8

/*
 * Header file describes the functions and constants used
 * for converting between different bases, allowing us to
 * use different representations for the 7 segment display
 */

/*
 * The base structure is used to store the number representation.
 *
 * @Elements
 * 	-> digits - a character pointer that will store the values of the digits to be displayed
 * 				For instance, for decimal it will store values between 0 - 9,
 * 				whereas in hex it will store values between 0 - 15
 *
 * 	-> numOfDigits - this integer will store the number of digits currently stored in the representation
 * 					 For instance, 7FA would have 3 digits.
 *
 * 	Note: The character data type was chosen to avoid unnecessary memory
 */
struct base {
	char* digits;
	u8 numOfDigits;
} base;	//Allocate memory for an instance that will be used throughout the program

#define DIGITS 4	//Number of digits that the board display can currently accommodate
#define BASE_10 10	//Number representing base 10
#define BASE_16 16	//Number representing base 16

/* Function prototypes */
struct base change_number_base(float number, u16 conversionBase);

#endif /* BASE_H_ */
