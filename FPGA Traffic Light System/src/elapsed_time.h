/*
 * elapsed_time.h
 *
 *  Created on: 19 Nov 2021
 *      Author: pirat
 */

#ifndef ELAPSED_TIME_H_
#define ELAPSED_TIME_H_

#include "xil_types.h"		// Added for integer type definitions

#define NONE 0

extern float elapsedPedestrianTime;
extern float elapsedTrafficLightTime;

void showElapsedPedestrianTime();
void showElapsedTrafficLightTime();
void updateElapsedTime();

#endif /* ELAPSED_TIME_H_ */
