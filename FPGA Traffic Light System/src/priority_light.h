/*
 * priority_light.h
 *
 *  Created on: 23 Nov 2021
 *      Author: pirat
 */

#ifndef PRIORITY_LIGHT_H_
#define PRIORITY_LIGHT_H_

#include "xil_types.h"		// Added for integer type definitions
#include "traffic_light_mechanism.h"


#define PRIORITY_NONE 0

#define PRIORITY_A 1
#define PRIORITY_B 2
#define PRIORITY_C 4

#define PRIORITY_AB 3
#define PRIORITY_AC 5
#define PRIORITY_CB 6

#define PRIORITY_ABC 7

#define SENSORS_UPPER_SWITCH 15
#define SENSORS_LOWER_SWITCH 13

#define SINGLE_PRIORITY_A 1
#define SINGLE_PRIORITY_B 2
#define SINGLE_PRIORITY_C 3


extern u8 SINGLE_PRIORITY;

u8 probeTrafficLightSensors();
u8 examineSinglePriorityFreezeState(struct trafficLight trafficLight);

#endif /* PRIORITY_LIGHT_H_ */
