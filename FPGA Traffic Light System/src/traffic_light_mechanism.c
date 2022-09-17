/* ################################################### File Description follows below ################################################# */

/*
 * File Name: traffic_light_mechanism.c
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
 *				The functions contained in this source file will be used to control the next traffic light
 *				to be operated at all times. This file contains 3 functions.
 *
 *				1. The initialiseTrafficLights() will initialise all traffic lights to hold their initial values
 *				   prior to the traffic light operations starting.
 *
 *				   	-> The initial output of all traffic lights will simply be red
 *
 *				2. The operateTrafficLights() function is the main function responsible for determining
 *				   which traffic light should be traced next. This is done by utilising the updateTrafficLight()
 *				   function which updates the current value of the TRAFFIC_LIGHT variable continuously.
 *
 *				3. The updateTrafficLight() function will continuously update the TRAFFIC_LIGHT variable
 *				   depending on the traffic light states that have been traversed, which will be used by
 *				   the operateTrafficLights() function to determine which traffic light to trace next.
 *
 *				   Note: To continuously update the TRAFFIC_LIGHT variable this function will be called by the
 *				   		 hwTimerISR in synchronisation with the traceRegion() function as this updates the state of
 *				   		 the traffic light.
 *
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "traffic_light_output.h"
#include "trace_light.h"
#include "gpio_init.h"
#include "traffic_light_mechanism.h"
#include "led_operations.h"
#include "priority_light.h"

/* ################################################### Global variables follow below ############################################### */

/* Defining the extern global variables */
u8 TRAFFIC_LIGHT = 0;
u8 STATE_COUNTER = 1; //Interesting - If this is set to 0 then we repeat first light twice, causes delay
					   //State counter should always show the next state that's the reason
					   //If we start at 0 then next state will not be 5 when we are at 4th transition, which
					   //is the issue

/* ################################################### Function definitions follow below ############################################ */


/**
 * This function initialises the traffic lights to hold their correct values before
 * the lights are being operated. Hence, it should only be called once at the start of the
 * program before entering the main while loop that controls the program operation.
 *
 * Note: This function utilises the LED operations in the led_operations.c class to
 *       initialise the LED values that will also simulate the traffic light sequence.
 *
 * @Param : None
 * @Return : None
 */
void initialiseTrafficLights() {

	/* Initialisation for traffic light A */
	trafficLightA.lightR = REGION_0_COLOUR;
	trafficLightA.yellowVGA = REGION_1_COLOUR;
	trafficLightA.lightG = REGION_2_COLOUR;
	trafficLightA.LED_R = LED_RED_A;
	trafficLightA.LED_Y = LED_YELLOW_A;
	trafficLightA.LED_G = LED_GREEN_A;
	trafficLightA.ID = TRAFFIC_LIGHT_A;

	writeR(trafficLightA);

	/* Initialisation for traffic light B */
	trafficLightB.lightR = REGION_3_COLOUR;
	trafficLightB.yellowVGA = REGION_4_COLOUR;
	trafficLightB.lightG = REGION_5_COLOUR;
	trafficLightB.LED_R = LED_RED_B;
	trafficLightB.LED_Y = LED_YELLOW_B;
	trafficLightB.LED_G = LED_GREEN_B;
	trafficLightB.ID = TRAFFIC_LIGHT_B;

	writeR(trafficLightB);

	/* Initialisation for traffic light C */
	trafficLightC.lightR = REGION_6_COLOUR;
	trafficLightC.yellowVGA = REGION_7_COLOUR;
	trafficLightC.lightG = REGION_8_COLOUR;
	trafficLightC.LED_R = LED_RED_C;
	trafficLightC.LED_Y = LED_YELLOW_C;
	trafficLightC.LED_G = LED_GREEN_C;
	trafficLightC.ID = TRAFFIC_LIGHT_C;

	writeR(trafficLightC);

}

/*
 * This function operates the traffic lights by determining which
 * traffic light should be traced next. This is done by continuously
 * checking the current value of the global variable TRAFFIC_LIGHT, which
 * is updated in the updateTrafficLight function.
 *
 * Note: Since this function operates by continuously checking the current value of the
 * 		 traffic light then it will be used in the main
 * 	     while loop to define the program flow.
 *
 * @Param : None
 * @Return : None
 */
void operateTrafficLights() {

	switch(TRAFFIC_LIGHT) {

		case TRAFFIC_LIGHT_A :

			traceLightA();

			break;
		case TRAFFIC_LIGHT_B :

			traceLightB();

			break;
		case TRAFFIC_LIGHT_C :

			traceLightC();

			break;
	}

}

/*
 * This function updates the TRAFFIC_LIGHT variable by examining
 * the current state of the traffic lights.
 *
 * Each traffic light can traverse 5 states as follows:
 *
 * 		R -> RY -> G -> Y -> R
 *
 * Hence, in total we will have 15 traversed states. This observation
 * can be used as follows:
 *
 * 	 -> If the STATE_COUNTER is in between the first 5 states then TRAFFIC_LIGHT_A should be operated
 * 	 -> If the STATE_COUNTER is in between the first 5 states and the first 10 states then TRAFFIC_LIGHT_B should be operated
 * 	 -> If the STATE_COUNTER is in between the first 10 states and the first 15 states then TRAFFIC_LIGHT_C should be operated
 *
 * 	 Otherwise -> RESET
 *
 * @Param : None
 * @Return : None
 */
void updateTrafficLight() {

	if (STATE_COUNTER < STATES) {
		TRAFFIC_LIGHT = TRAFFIC_LIGHT_A;
		//STATE_COUNTER++;
	}

	else if (STATE_COUNTER < 2*STATES) {
		TRAFFIC_LIGHT = TRAFFIC_LIGHT_B;
		//STATE_COUNTER++;
	}

	else if (STATE_COUNTER < 3*STATES) {
		TRAFFIC_LIGHT = TRAFFIC_LIGHT_C;
		//STATE_COUNTER++;
	}

	/*if (STATE_COUNTER == 3*STATES) {
		STATE_COUNTER = 0;	//Reset
	}*/

	updateStateCounter();

}

void updateStateCounter() {

	u8 sensorsIn = probeTrafficLightSensors();

	switch(sensorsIn) {

		case PRIORITY_NONE :
			STATE_COUNTER++;

			if (STATE_COUNTER == 3*STATES) {
				STATE_COUNTER = 0;
			}
			break;
		case PRIORITY_A :

			if (STATE_COUNTER >= STATES) {
				STATE_COUNTER = 0;
			}
			else if (STATE_COUNTER == 3) { //State counter keeps track of next state so we are in 3rd state
				//Do nothing
			} else {
				STATE_COUNTER++;
			}

			SINGLE_PRIORITY = SINGLE_PRIORITY_A;

			break;
		case PRIORITY_B :

			if (STATE_COUNTER >= 2*STATES) {
				STATE_COUNTER = 5;
			}
			else if (STATE_COUNTER == 8) { //State counter keeps track of next state so we are in 3rd state
				//Do nothing
			} else {
				STATE_COUNTER++;
			}

			SINGLE_PRIORITY = SINGLE_PRIORITY_B;

			break;
		case PRIORITY_C :

			if (STATE_COUNTER >= 3*STATES || STATE_COUNTER >= STATES - 1) {
				STATE_COUNTER = 13;
			}
			else if (STATE_COUNTER == 13) { //State counter keeps track of next state so we are in 3rd state
				//Do nothing
			} else {
				STATE_COUNTER++;
			}

			SINGLE_PRIORITY = SINGLE_PRIORITY_C;

			break;
		case PRIORITY_AB :
			STATE_COUNTER++;

			if (STATE_COUNTER >= 2*STATES) {
				STATE_COUNTER = 0;
			}

			break;
		case PRIORITY_AC :
			STATE_COUNTER++;

			if (STATE_COUNTER == STATES) {
				STATE_COUNTER = 2*STATES;
			}

			else if (STATE_COUNTER == 3*STATES)

			{
				STATE_COUNTER = 0;
			}

			break;
		case PRIORITY_CB :
			STATE_COUNTER++;

			if (STATE_COUNTER == 3*STATES) {
				STATE_COUNTER = 5;
			}

			break;
		case PRIORITY_ABC :
			STATE_COUNTER++;

			if (STATE_COUNTER == 3*STATES) {
				STATE_COUNTER = 0;
			}
			break;
		default:
			break;
	}

}
