/*
 * priority_light.c
 *
 *  Created on: 23 Nov 2021
 *      Author: pirat
 */

#include "priority_light.h"
#include "input_extract.h"
#include "gpio_init.h"

u8 SINGLE_PRIORITY = 0;

u8 probeTrafficLightSensors() {

	u16 slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	u16 sensorSwitchesIn = extractSwitches(slideSwitchIn, SENSORS_UPPER_SWITCH, SENSORS_LOWER_SWITCH);

	return sensorSwitchesIn;

}

u8 examineSinglePriorityFreezeState(struct trafficLight trafficLight) {

	u8 ID = trafficLight.ID;
	u8 freeze = FALSE;

	switch (SINGLE_PRIORITY) {

		case SINGLE_PRIORITY_A :

			if (ID == TRAFFIC_LIGHT_A && STATE_COUNTER == 3) {
				freeze = TRUE;
			}

			break;
		case SINGLE_PRIORITY_B :

			if (ID == TRAFFIC_LIGHT_B && STATE_COUNTER == 8) {
				freeze = TRUE;
			}

			break;
		case SINGLE_PRIORITY_C :

			if (ID == TRAFFIC_LIGHT_C && STATE_COUNTER == 13) {
				freeze = TRUE;
			}

			break;
		default :
			break;

	}

	return freeze;

}
