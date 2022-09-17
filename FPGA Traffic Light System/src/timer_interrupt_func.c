/* ################################################### File Description follows below ################################################# */

/*
 * File Name: timer_interrupt_func.c
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
 *				The functions contained in this source file will be used to execute the required operations
 *				when the interrupt service routine is received. This file contains 4 functions.
 *
 *				1. The hwTimerISR() is the most important function as this is the function that is called
 *				   each time an interrupt received. Hence, all code that should be executed when an interrupt
 *				   is received should be contained within this function.
 *
 *				2. The operatePedestrianLightInterrupt() defines the code that should be executed for the
 *				   pedestrian mechanism when the interrupt is received.
 *
 *				3. The operateTrafficLightInterrupt() defines the code that should be executed for the
 *				   traffic lights mechanism when the interrupt is received.
 *
 *				4. The operateTimeElapsedInterrupt() defines the code that should be executed for the
 *				   elapsed time module (keeps track of the time elapsed for both the pedestrian and traffic lights)
 *				   when the interrupt is received.
 *
 *
 *				Note: The different modules that are dependent on the interrupt require different interrupt time intervals
 *					  and so counters will be used to slow down the interrupt service routing as required for each of these
 *					  modules.
 *
 *
 */

/* ################################################### Module imports follow below ################################################# */


#include "trace_light.h"
#include "traffic_light_mechanism.h"

#include "led_operations.h"
#include "pedestrian_mechanism.h"
#include "gpio_init.h"
#include "seg7_display.h"

#include "elapsed_time.h"
#include "timer_interrupt_func.h"
#include "emergency_stop.h"

void xil_printf(const char *ctrl1, ...);

/* ################################################### Global variables follow below ############################################### */

/* Coutner variables that will be used to slow down the interrupt service routine as required */
int trafficLightCounter = 0;
int pedestrianCounter = 0;
int timeElapsedCounter = 0;

/* ################################################### Function definitions follow below ############################################ */

/* This function determines the code that should be
 * executed each time an interrupt is received
 *
 * There are 3 main operations that are dependent on the interrupt
 * service routine
 *
 * 	1. The pedestrian mechanism
 * 	2. The traffic light mechanism
 * 	3. The elapsed time counter
 *
 * Note: The hardware files have set the interrupt to occur every 4ms
 *
 * @Param
 * 	-> CallbackRef : void pointer indicating where to resume program execution
 *
 * @Return : None
 */
void hwTimerISR(void *CallbackRef)
{

	if (!EMERGENCY_MODE) {

		/* If BTNR (pedestrian button) has been pressed and
		 * the traffic light being operated has returned to
		 * the red (stop) state then operate the pedestrian lights
		 * otherwise just continue normal operation
		 * */
		if (PEDESTRIAN_MODE && traceFinished) {

			operatePedestrianLightInterrupt();

		} else {

			operateTrafficLightInterrupt();

		}

		/* The elapsed time should be updated continuously */
		operateTimeElapsedInterrupt();
	}

	/* The display digit should be called every 4ms to ensure
	* that no flickering is observed on the 7-Segment-Display */
	displayDigit();

}

/*
 * This function is responsible for executing the required
 * operations for the pedestrian mechanism when the interrupt
 * is received.
 *
 * The pedestrian mechanism operates using time slices
 * of 0.2 seconds, but an interrupt is received every 4ms
 * and so a counter is used to ensure we only operate this
 * function when a time slice of 0.2 seconds has been reached
 *
 * @Param : None
 * @Return : None
 */
void operatePedestrianLightInterrupt() {

	if (pedestrianCounter == PED_INTERRUPT_COUNTER_LIM) {
		updatePedestrianTimeInterval();


		pedestrianCounter = 0;			//Reset counter if we have reached the required time slice
	}

	pedestrianCounter++;


}

/*
 * This function is responsible for executing the required
 * operations for the traffic lights mechanism when the interrupt
 * is received.
 *
 * The traffic light mechanism  operates using time slices of
 * 1 second, but an interrupt is received every 4ms
 * and so a counter is used to ensure we only operate this
 * function when a time slice of 1 seconds has been reached
 *
 * @Param : None
 * @Return : None
 */
void operateTrafficLightInterrupt() {

	if (trafficLightCounter == LIGHT_INTERRUPT_COUNTER_LIM) {

		traceRegion();
		updateTrafficLight();

		//xil_printf("TrafficLight : %d State Counter : %d\n",
			//		TRAFFIC_LIGHT, STATE_COUNTER);

		trafficLightCounter = 0;			//Reset counter if we have reached the required time slice
	}

	trafficLightCounter++;

}

/*
 * This function is responsible for executing the required
 * operations for the elapsed time module when the interrupt
 * is received.
 *
 * The elapsed time module oerates using time slices of
 * 0.1 seconds, but an interrupt is received every 4ms
 * and so a counter is used to ensure we only operate this
 * function when a time slice of 0.1 seconds has been reached
 *
 * @Param : None
 * @Return : None
 */
void operateTimeElapsedInterrupt() {

	if (timeElapsedCounter == ELAPSED_TIME_INTERRUPT_COUNTER_LIM) {
		updateElapsedTime();


		timeElapsedCounter = 0;			//Reset counter if we have reached the required time slice
	}

	timeElapsedCounter++;

}
