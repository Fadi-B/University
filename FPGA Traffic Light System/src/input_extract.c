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
s16 extractSwitches(u16 input, u16 upperBit, u16 lowerBit) {
	u16 extract_mask = createMask(upperBit, lowerBit);	//Mask used to extract the input number

	s16 extract = input & extract_mask;

	extract = extract >> (lowerBit - 1); 		//Map it so that is starts from 0
										  	  	//The -1 is since we count bits from 1
	return extract;
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
u16 createMask(u16 upperSwitch, u16 lowerSwitch) {

	u16 result = 0;

	u16 limit = lowerSwitch;
	u16 start = upperSwitch;

	for (start; start >= limit; start--) {
		result = result | (1 << (start - 1));
	}

	return result;
}
