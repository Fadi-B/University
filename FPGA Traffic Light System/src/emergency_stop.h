/*
 * emergency_stop.h
 *
 *  Created on: 22 Nov 2021
 *      Author: pirat
 */

#ifndef EMERGENCY_STOP_H_
#define EMERGENCY_STOP_H_

#include "xil_types.h"		// Added for integer type definitions


#define EMERGENCY_SWITCH 1

extern u8 EMERGENCY_MODE;

void operateEmergencyStop();
u8 probeEmergencySwitch();


#endif /* EMERGENCY_STOP_H_ */
