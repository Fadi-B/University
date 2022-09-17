/* ################################################### File Description follows below ################################################# */

/*
 * File Name: pedestrian_mechanism.c
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
 *				The functions contained in this source file will be used will be used to drive the
 *				pedestrian light mechanism of the traffic light. This file contains 6 main functions.
 *
 *				1. The initialisePedestrianLights() will initialise the pedestrian lights to hold their initial values
 *				   prior to the traffic light operations starting.
 *
 *				   	-> The initial output of the pedestrian light will simply be red
 *
 *				2. The operatePedestrianLight() function will operate the pedestrian light such that when it is called
 *				   the pedestrian light on the VGA and the LEDs are turned on for 5 seconds and they blink 5 times in the
 *				   remaining 2 seconds. It will simultaneously also display the elapsed pedestrian time on the 7-Segment display.
 *
 *				3. The probePedestrianButton() function will check if the pedestrian button BTNR has been pressed and so it
 *				   will be used in the traceLight() function continuously (defined in trace_light.c) to examine if the button has been
 *				   pressed when the traffic lights are in operation.
 *
 *				4. The turnPedestrianLightOff() is a utility function used in the operatePedestrianLight() function to turn off
 *				   the pedestrian light on the VGA and the LED.
 *
 *				5. The turnPedestrianLightOn() is a utility function used in the operatePedestrianLight() function to turn on
 *				   the pedestrian light on the VGA and the LED.
 *
 *				6. The updatePedestrianTimeInterval() function is used by the hwTimerISR() function defined in the timer_interrupt_func.c
 *				   file. Its purpose is to update a boolean pedestrianTimeElapsed every 0.2s by the ISR, which will indicate to the
 *				   the operatePedestrianLight() function that the next time slice of the pedestrian light has been used up until we
 *				   reach 5 seconds after which operatePedestrianLight terminates.
 *
 * 				Note: The functions defined in this class will utilise modules pedestrian_mechanis.h, led_operations.h, trace_light.h,
 * 					  gpio_init.h, elapsed_time.h
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "pedestrian_mechanism.h"
#include "gpio_init.h"
#include "traffic_light_output.h"
#include "led_operations.h"
#include "elapsed_time.h"
#include "emergency_stop.h"
#include "seg7_display.h"

/* ################################################### Global variables follow below ############################################### */

/* Defining the extern global variables */
u8 PEDESTRIAN_MODE = FALSE;

/* Defining source file global variables */
u8 pedestrianTimeSliceElapsed = FALSE;

/* ################################################### Function definitions follow below ############################################ */

/**
 * This function initialises the pedestrian lights to hold their correct value before
 * the traffic light operation begins. Hence, it should only be called once at the start of the
 * program before entering the main while loop that controls the program operation.
 *
 * Note: This function utilises the LED operations in the led_operations.c class to
 *       initialise the LED values that will also simulate the overal traffic light sequence.
 *
 * @Param : None
 * @Return : None
 */
void initialisePedestrianLights() {

	/* Initialisation for pedestrian lights */
	XGpio_DiscreteWrite(&REGION_9_COLOUR, 1, WHITE);
	XGpio_DiscreteWrite(&REGION_10_COLOUR, 1, RED);
	XGpio_DiscreteWrite(&REGION_11_COLOUR, 1, WHITE);

}


/* This function operates the pedestrian light by turning it on for 5 seconds
 * when it is called. Moreover, it also ensures that the pedestrian light
 * blinks 5 times for the remaining 2 seconds out of 5 that it should be on.
 *
 * Note: Turning pedestrian light on means both the VGA pedestrian region and the
 * 	     pedestrian LED, which are currently mapped to region 10 and LED 15 respectively.
 *
 * @Param : None
 * @Return : None
 *
 */
void operatePedestrianLight() {

	u8 counter = 0;				//Will be used to determine when 5 seconds have elapsed
	u8 blinkCounter = 1;		//Will be used in the blinking mechanism. Initialised to 1 as we start by turning light white in the blinking

	u8 counterLimit = PED_NUMBER_TIME_SLICES;
	u8 blinkThreshold = PED_NUMBER_TIME_SLICES - PED_NUMBER_BLINK_TIME_SLICES;

	removeLED(LED_INDICATOR_PEDESTRIAN);
	turnPedestrianLightOnGreen();

	while (counter < counterLimit) {

		if (EMERGENCY_MODE) {
			break;
		}

		/* Perform the blinking mechanism if remaining 2 seconds reached */
		if (counter >= blinkThreshold) {

			if (blinkCounter%2 == 0) { 		//Turn on when counter even
				turnPedestrianLightOnGreen();

			} else {
				turnPedestrianLightOff();   //Turn off when counter odd
			}

			blinkCounter++;
		}

		counter++;

		/* Wait for ISR to complete updating the pedestrianTimeElapsed
		 * which indicates that 0.2 seconds have elapsed
		 *
		 * While we wait we want to display the total elapsedPedestrianTime
		 * using the elapsed_time.h module
		 */
		while(pedestrianTimeSliceElapsed == FALSE) {
			u8 emergencySwitchActivated = probeEmergencySwitch();
			u8 showLight = probeShowLightSwitch();

			if (emergencySwitchActivated) {
				EMERGENCY_MODE = TRUE;
				break;
			}

			if (showLight) {
				SHOW_LIGHT = TRUE;
			} else {
				SHOW_LIGHT = FALSE;
			}

			showElapsedPedestrianTime();
		}

		pedestrianTimeSliceElapsed = FALSE;

	}

	turnPedestrianLightOff();
	turnPedestrianLightOnRed();

}

/*
 * This function probes the pedestrian button, which corresponds
 * to BTNR on the Basys3 board to, in order to see if a pedestrian
 * has indicated that they want to pass.
 *
 * Note: It will be called by the traceLight() function in the trace_light.c source file
 * 		 continuously to ensure that we are probing the pedestrian button during the entire
 * 		 operation of the traffic lights.
 *
 * @Param : None
 * @Return
 * 	-> 8-bit unsigned integer indicating if BTNR has been pressed.
 * 	   Receiving a 1 indicates that it has been pressed, otherwise not.
 */
u8 probePedestrianButton() {

	u8 pushBtnRightIn = 0;

	pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);

	if (pushBtnRightIn) {
		return 1;
	}

	return 0;

}

/*
 * This function turns off the pedestrian VGA region by setting
 * it to the colour red it turns off the pedestrian LED.
 *
 * Note: 1. Pedestrian VGA region corresponds to region 10
 * 		 2. Pedestrian LED corresponds to LED 15
 *
 * @Param : None
 * @Return : None
 */
void turnPedestrianLightOff() {

	XGpio_DiscreteWrite(&REGION_10_COLOUR, 1, WHITE);
	removeLED(LED_PEDESTRIAN);

}

/*
 * This function turns on the pedestrian VGA region by setting
 * it to the colour green and it turns on the pedestrian LED.
 *
 * Note: 1. Pedestrian VGA region corresponds to region 10
 * 		 2. Pedestrian LED corresponds to LED 15
 *
 * @Param : None
 * @Return : None
 *
 */
void turnPedestrianLightOnGreen() {

	XGpio_DiscreteWrite(&REGION_10_COLOUR, 1, GREEN);
	writeLED(LED_PEDESTRIAN);

}

/*
 * This function turns on the pedestrian VGA region by setting
 * it to the colour red and it turning off the pedestrian LED.
 *
 * Note: 1. Pedestrian VGA region corresponds to region 10
 * 		 2. Pedestrian LED corresponds to LED 15
 *
 * @Param : None
 * @Return : None
 *
 */
void turnPedestrianLightOnRed() {

	XGpio_DiscreteWrite(&REGION_10_COLOUR, 1, RED);
	removeLED(LED_PEDESTRIAN);

}

/*
 * This function updates the boolean for the operatePedestrianLight function
 * that indicates that a time slice of 0.2 seconds has passed.
 *
 * Note: It is called by the ISR when 0.2 seconds have
 *
 * @Param : None
 * @Return : None
 */
void updatePedestrianTimeInterval() {

	pedestrianTimeSliceElapsed = TRUE;

}
