#include "input_extract.h"
#include "gpio_init.h" //Used for the show on LED function

/*
 * This function extracts the input number, which is specified by an upper and lower bit.
 * The bits are counted starting from 1.
 * For instance, to extract the first input of the slide switch the
 * upper and lower bits are 6 and 1 respectively.
 *
 * @Param
 * 	-> input - value of all the slide switches
 * 	-> upperBit - the upper bit position of the number to be extracted
 * 	-> lowerBit - the lower bit position of the number to be extracted
 * @Return
 * 	-> signed integer representing the specified number of the 12 bottom slide switches
 */
s16 extract_input_number(u16 input, u16 upperBit, u16 lowerBit) {
	u16 extract_mask = create_mask(upperBit, lowerBit);	//Mask used to extract the input number

	u16 INPUT_MSB_MASK = 1 << (upperBit - 1); //We are using 2's complement representation.
											   //Mask used for determining if MSB (negative bit) is set
	u16 SIGN_EXTEND_MASK = 0;				   //Mask used for sign extending input if MSB is set

	SIGN_EXTEND_MASK = create_mask(SIZE, upperBit + 1); //The + 1 is to exclude the upper bit of number from mask

	s16 extract = input & extract_mask;

	if (extract & INPUT_MSB_MASK) {				//If MSB set then sign extend with 1's
		extract = extract | SIGN_EXTEND_MASK;
	}

	extract = extract >> (lowerBit - 1); 		//Map it so that is starts from 0
										  	  	//The -1 is since we count bits from 1
	return extract;
}

/*
 * This function extracts the top 4-bit operation from the 16 slide switches
 *
 * @Param -> input - value of all the slide switches
 * @Return -> unsigned integer representing the specified number of the top 4 slide switches
 */
u16 extract_input_operation(u16 input) {
	u16 INPUT_OP_MASK = 0;						//Creates mask 1111 00...00 for the operation bits

	INPUT_OP_MASK = create_mask(SIZE, SIZE - INPUT_OP_BITS + 1); //The + 1 is to exclude last bit so that only upper 4 extracted

	u16 extract = INPUT_OP_MASK & input;		//Extract the specified operation bits

	extract = (extract >> INPUT_NUMBER_BITS); 	//Map it to 0 - 15

	return extract;
}

/*
 * This function displays the specified input numbers on the LEDs.
 * This is achieved by extracting the bottom 12-bits from the 16 slide switches
 * as these are reserved for number inputs.
 *
 * @Param
 * 	-> input - value of all the slide switches
 */
void show_input_numbers_on_LEDs(u16 input) {

	u16 output = 0;
	u16 INPUT_OP_MASK = 0;						//Creates mask 1111 00...00 for the operation bits

	INPUT_OP_MASK = create_mask(SIZE, SIZE - INPUT_OP_BITS + 1); //The + 1 is to exclude last bit so that only upper 4 extracted

	output = input & ~(INPUT_OP_MASK); 			//Mask for bottom 12 bits is the inverse of INPUT_OP_MASK

	XGpio_DiscreteWrite(&LED_OUT, 1, output);
}

/*
 * This function creates a bit mask as specified by the upper
 * and lower bits. The bits from the upper to lower bit will be
 * set to 1 and the rest will be set to 0.
 *
 * @Param
 * 	-> upperBit - the upper bit limit used in creating the mask
 * 	-> lowerBit - the lower bit limit used in creating the mask
 *
 * @Return
 * 	-> unsigned integer containing the created mask
 */
u16 create_mask(u16 upperBit, u16 lowerBit) {

	u16 result = 0;

	u16 limit = lowerBit;
	u16 start = upperBit;

	for (start; start >= limit; start--) {
		result = result | (1 << (start - 1));
	}

	return result;
}
