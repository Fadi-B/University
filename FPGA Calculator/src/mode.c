#include "mode.h"

//Define MODE - initially in decimal mode
u16 NUMBER_MODE = 0;

//Define ANGLE_MODE - initially in degrees
u16 ANGLE_MODE = 0;

/*
 * This function switches between number representations
 * depending on the previous number mode.
 * 
 * Note: The next previous mode is updated by the function using pointers
 * 
 * @Param
 * 	-> unsigned integer pointer that references the memory location of previous number mode value
 */
void switch_number_mode(u16 *previous) {

	if (*previous == DECIMAL) {
		NUMBER_MODE = HEX;
		*previous = HEX;
	} else {
		NUMBER_MODE = DECIMAL;
		*previous = DECIMAL;
	}
}

/*
 * This function switches between angle representations
 * depending on the previous angle mode
 *
 * Note: The next previous mode is updated by the function using pointers
 *
 * @Param
 * 	-> *previous - unsigned integer pointer that references the memory location of previous angle mode value
 */
void switch_angle_mode(u16 *previous) {

	if (*previous == RADIANS) {
		ANGLE_MODE = DEGREES;
		*previous = DEGREES;
	} else {
		ANGLE_MODE = RADIANS;
		*previous = RADIANS;
	}
}
