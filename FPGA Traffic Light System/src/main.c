#include <stdio.h>
#include "platform.h"
#include "xil_types.h"		// Added for integer type definitions
#include "gpio_init.h" // Added for 7-segment definitions

#include "traffic_light_mechanism.h"
#include "gpio_init.h"
#include "pedestrian_mechanism.h"

#include "led_operations.h"
#include "seg7_display.h"
#include "emergency_stop.h"
void xil_printf(const char *ctrl1, ...);


int main()
{
	init_platform();
	int status;

    // Initialize the GPIOs
    status = initGpio();
	if (status != XST_SUCCESS) {
		print("GPIOs initialization failed!\n\r");
		cleanup_platform();
		return 0;
	}

	// Setup the Interrupt System
	status = setUpInterruptSystem();
	if (status != XST_SUCCESS) {
		print("Interrupt system setup failed!\n\r");
		cleanup_platform();
		return 0;
	}

	initialiseTrafficLights();
	initialisePedestrianLights();

	while(1) {

		if (EMERGENCY_MODE) {
			operateEmergencyStop();
		}

		 if (PEDESTRIAN_MODE) {
			operatePedestrianLight();
			PEDESTRIAN_MODE = FALSE; //Removes bugs with pressing in between
		} else {
			operateTrafficLights();
		}


	}


    cleanup_platform();
    return 0;
}
