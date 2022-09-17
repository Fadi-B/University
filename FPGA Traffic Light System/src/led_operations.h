/*
 * led_operations.h
 *
 *  Created on: 15 Nov 2021
 *      Author: pirat
 */

#ifndef LED_OPERATIONS_H_
#define LED_OPERATIONS_H_

#include "xil_types.h"		// Added for integer type definitions

extern u16 LED_VALUE;

void writeLED(u8 LED);
void removeLED(u8 LED);

#endif /* LED_OPERATIONS_H_ */
