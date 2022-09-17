/* ################################################### File Description follows below ################################################# */

/*
 * File Name: trace_light.c
 *
 * Project Name: BrittishTrafficLight
 *
 * Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
 * Tool Version: Xilinx SDK 2015.2
 *
 * Name: Fadi Barazi
 * Company: University of Edinburgh
 * Creation Date and Time: 14/11/2021; 15:39
 *
 * Description: This source file contains the function definitions for the function
 * 				declarations in the corresponding header file.
 *
 *				The functions contained in this source file will be used to drive the three
 *				traffic lights A, B and C on the VGA display and the LED outputs. This file contains 6 functions.
 *
 *				1. The traceLight() function is the most important function in this class and it will be responsible for tracing the
 *				   traffic light pattern and outputting the correct display on the VGA and the LEDs. The tracing function will
 *				   trace the traffic light pattern such that each light (R -> RY -> G -> Y -> R) lasts for 1 second. Hence, it
 *				   will make use of the ISR during the tracing.
 *
 *				2. The updateLight() function will be used by the traceLight() function to output the correct display on the VGA and
 *				   the LEDs as the pattern is traced.
 *				  .
 *				3. The traceLightA() will call the traceLight() function with the required parameters to trace traffic light A
 *
 *				4. The traceLightB() will call the traceLight() function with the required parameters to trace traffic light B
 *
 *				5. The traceLightC() will call the traceLight() function with the required parameters to trace traffic light C
 *
 *				6. The traceRegion() will update the regionMoved boolean that indicates that the traceLight() function should
 *				   continue the traffic light tracing. This function will be called by the hwTimerISR in order to update the boolean
 *				   every 1 second, which is currently the time that is allocated for each light.
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "trace_light.h"
#include "gpio_init.h"
#include "led_operations.h"
#include "traffic_light_mechanism.h"
#include "pedestrian_mechanism.h"
#include "seg7_display.h"
#include "elapsed_time.h"
#include "traffic_light_output.h"
#include "emergency_stop.h"
#include "priority_light.h"

/* ################################################### Global variables follow below ############################################### */

/* Defining the extern global variables */
u8 traceFinished = FALSE;

/* Defining source file global variables */
u8 regionMoved = FALSE;  //Will be used to stop code progression until next light traced

/* ################################################### Function definitions follow below ############################################ */

/*
 * This function will trace traffic light A by calling traceLight()
 * with the required parameters.
 *
 * @Param : None
 * @Return : None
 */
void traceLightA() {

	traceLight(trafficLightA);
}

/*
 * This function will trace traffic light B by calling traceLight()
 * with the required parameters.
 *
 * @Param : None
 * @Return : None
 */
void traceLightB() {

	traceLight(trafficLightB);
}

/*
 * This function will trace traffic light C by calling traceLight()
 * with the required parameters.
 *
 * @Param : None
 * @Return : None
 */
void traceLightC() {

	traceLight(trafficLightC);
}

/*
 * This function will trace the pattern of the specified traffic light
 * and it will output the pattern being traced on the VGA display, as well as
 * on the LEDs.
 *
 * Note: The traffic light pattern is as follows,
 *
 * 		 		R -> RY -> G -> Y -> R
 *
 *		 Each light will be allocated a share of 1 second in this pattern.
 *		 This function ensures that 1 second is allocated to each light by
 *		 waiting on the regionMoved boolean to turn TRUE, which will be updated
 *		 by the traceRegion() function that is called in the hwTimerISR function.
 *
 *		 During each transition the required pattern output is achieved using updateLight()
 *
 */
void traceLight(struct trafficLight trafficLight) {

	traceFinished = FALSE;

	u16 counter = 0;
	u16 previous = 0;

	while (counter < STATES) {

		if (EMERGENCY_MODE) {
			break;
		}

		if (!PEDESTRIAN_MODE) {
			u8 freezeLight = examineSinglePriorityFreezeState(trafficLight);

			if (freezeLight && counter == 3) {
				counter = previous;
			}
		}

		/* Writing to relevant GPIO objects */
		updateLight(counter, trafficLight);

		/* Wait for ISR to complete updating the regionMoved boolean
		 * which indicates that 1 second has elapsed
		 *
		 * While we wait we want to display the total elapsedTrafficLightTime
		 * using the elapsed_time.h module. We also want to probe the pedestrian
		 * button to ensure we do not miss when it is pressed.
		 */
		while (regionMoved == FALSE) {

			u8 pedestrianBtnPressed = probePedestrianButton();
			u8 emergencySwitchActivated = probeEmergencySwitch();
			u8 showLight = probeShowLightSwitch();

			if (pedestrianBtnPressed) {
				PEDESTRIAN_MODE = TRUE;
				writeLED(LED_INDICATOR_PEDESTRIAN);
			}

			if (emergencySwitchActivated) {
				EMERGENCY_MODE = TRUE;
				break;
			}

			if (showLight) {
				SHOW_LIGHT = TRUE;
			} else {
				SHOW_LIGHT = FALSE;
			}

			showElapsedTrafficLightTime();
		}

		regionMoved = FALSE;

		previous = counter;
		counter++;
	}

	traceFinished = TRUE;

}

/*
 * This function updates the traffic light such that the correct
 * output is rendered on the VGA and the LEDs as the traffic light
 * light pattern is traced.
 *
 * @Param
 * 	-> position : u16 indicator of the traffic light state that we are in (R, RY, G, Y, R)
 * 	-> regions : XGpio array containing the three regions that define the traffic light in the order RED, YELLOW, GREEN
 * 	-> trafficLight : u8 indicator of which traffic light that should be updated
 *
 * 	@Return : None
 */
void updateLight(u16 state, struct trafficLight trafficLight) {

	switch (state) {
		case 0 :

			writeR(trafficLight);

			break;
		case 1:

			writeRY(trafficLight);

			break;
		case 2:

			writeG(trafficLight);

			break;
		case 3:

			writeY(trafficLight);

			break;

		case 4:

			writeR(trafficLight);

		default :
			//Should add an error here I think
			break;
	}

}

/*
 * This function updates the regionMoved boolean which is used by the
 * traceLight() function to determine when it should continue to trace
 * the traffic light pattern.
 *
 * @Param : None
 * @Return : None
 */
void traceRegion() {

	regionMoved = TRUE;

}

