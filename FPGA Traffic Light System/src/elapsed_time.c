/*
 * elapsed_time.c
 *
 *  Created on: 19 Nov 2021
 *      Author: pirat
 */

#include "elapsed_time.h"
#include "seg7_display.h"
#include "pedestrian_mechanism.h"
#include "trace_light.h"

float elapsedPedestrianTime = 5;
float elapsedTrafficLightTime = 1;

void showElapsedPedestrianTime() {

	displayNumber(elapsedPedestrianTime);

}

void showElapsedTrafficLightTime() {

	displayNumber(elapsedTrafficLightTime);

}

void updateElapsedTime() {

	if (PEDESTRIAN_MODE && traceFinished) {

		if (elapsedPedestrianTime < 0.1) {

			elapsedPedestrianTime = 5; //Reset

		} else {

			elapsedPedestrianTime = elapsedPedestrianTime - 0.1; //Interrupt interval

		}

	} else {

		if (elapsedTrafficLightTime < 0.1) {

			elapsedTrafficLightTime = 1;

		} else {

			elapsedTrafficLightTime = elapsedTrafficLightTime - 0.1;

		}

		elapsedPedestrianTime = 5;

	}

}
