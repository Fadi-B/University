/*
 * emergency_stop.c
 *
 *  Created on: 22 Nov 2021
 *      Author: pirat
 */

#include "emergency_stop.h"
#include "input_extract.h"
#include "traffic_light_output.h"
#include "gpio_init.h"
#include "pedestrian_mechanism.h"
#include "seg7_display.h"
#include "elapsed_time.h"
#include "led_operations.h"

u8 EMERGENCY_MODE = FALSE;

void operateEmergencyStop() {

	LED_VALUE = 0;
	elapsedPedestrianTime = 5;
	elapsedTrafficLightTime = 1;
	STATE_COUNTER = 1;
	TRAFFIC_LIGHT = TRAFFIC_LIGHT_A;
	PEDESTRIAN_MODE = FALSE;

	u8 emergencySwitchActivated = probeEmergencySwitch();

	while (emergencySwitchActivated) {

		emergencySwitchActivated = probeEmergencySwitch();

		writeR(trafficLightA);
		writeR(trafficLightB);
		writeR(trafficLightC);

		turnPedestrianLightOnRed();

		displayNumber(NONE);
	}

	EMERGENCY_MODE = FALSE;

}

u8 probeEmergencySwitch() {

	u16 slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	u16 emergencySwitch = extractSwitches(slideSwitchIn, EMERGENCY_SWITCH, EMERGENCY_SWITCH);

	if (emergencySwitch) {
		return 1;
	} else {
		return 0;
	}

}
