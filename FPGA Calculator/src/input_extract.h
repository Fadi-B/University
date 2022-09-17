#ifndef INPUT_EXTRACT_H_
#define INPUT_EXTRACT_H_

#include <stdio.h>
#include "platform.h"
#include "xil_types.h"

/*
 * Header file describes the operations and constants used
 * for extracting the input numbers and mathematical operation type
 * from the 16-bit slide switches
 *
 * Note: We are counting the slide switches starting from 1
 */


#define SIZE 16 				//Size of input received from switches
#define INPUT_NUMBER_BITS 12 	//Number of bits for the switches that constitute the two input numbers
#define INPUT_OP_BITS 4			//Number of bits for the switches that constitute the input operation

/* First input number is specified by an inclusive upper and lower bit*/
#define FIRST_INPUT_UPPER_BIT 6
#define FIRST_INPUT_LOWER_BIT 1

/* Second input number is specified by an inclusive upper and lower bit*/
#define SECOND_INPUT_UPPER_BIT 12
#define SECOND_INPUT_LOWER_BIT 7

/* Function prototypes*/
s16 extract_input_number(u16 input, u16 upperBit, u16 lowerBit);
u16 extract_input_operation(u16 input);
void show_input_numbers_on_LEDs(u16 input);
u16 create_mask(u16 upper, u16 lower);

#endif /* INPUT_EXTRACT_H_ */
