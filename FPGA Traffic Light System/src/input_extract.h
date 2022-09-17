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

/* Function prototypes*/
s16 extractSwitches(u16 input, u16 upperSwitch, u16 lowerSwitch);
u16 createMask(u16 upperSwitch, u16 lowerSwitch);

#endif /* INPUT_EXTRACT_H_ */
