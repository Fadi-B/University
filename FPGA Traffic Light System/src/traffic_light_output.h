#ifndef TRAFFIC_LIGHT_OUTPUT_H_
#define TRAFFIC_LIGHT_OUTPUT_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: traffic_light_output.h
 *
 * Project Name: BrittishTrafficLight
 *
 * Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA
 * Tool Version: Xilinx SDK 2015.2
 *
 * Name: Fadi Barazi
 * Company: University of Edinburgh
 * Creation Date and Time: 14/11/2021; 15:39
 *
 * Description: This header file will outline the structure of the traffic light output operations
 * 				which will allow us to write to the relevant GPIOs during the traffic light operations.
 * 				Hence, it will contain definitions of the colours that can be written to the traffic light
 * 				and which LEDs will simulate the traffic light sequence.
 *
 * 				This file contains the function declarations that will be defined
 * 				in the traffic_light_output.c file. Moreover, it also contains macro
 * 				definitions that are used to enhance code readability.
 *
 *				Macro definitions for the output colours follow below:
 *
 *				Note: The Basys3 board uses a 12-bit RGB colour scheme for the VGA.
 *
 *				1. RED : 12-bit RGB colour code for red.
 *				2. YELLOW : 12-bit RGB colour code for yellow.
 *				3. GREEN : 12-bit RGB colour code for green.
 *				4. WHITE : 12-bit RGB colour code for white/blank.
 *
 *				Macro definitions for the output LEDs follow the following naming convention
 *
 *					-> LED_'colour'_'trafficLight'
 *
 *				and they will hold the value of the LED position that will represent the specified colour
 *				of the provided traffic light.
 *
 *				For instance,
 *
 *					-> LED_RED_A 2 : Indicates that the third LED (we start from 0) on the board will represent
 *									 the red light for traffic light A
 *
 *
 */


/* ################################################### Module imports follow below ################################################# */

#include "xgpio.h"		// Added for xgpio object definitions
#include "traffic_light_mechanism.h"

/* ################################################### Macro constants follow below ################################################# */

/* Macros for the different colours for the traffic lights
 * that can be written to the VGA output */
#define RED 3840
#define YELLOW 4080
#define GREEN 240
#define WHITE 0xFFF

/* Macros for the LED positions corresponding
 * to each light for traffic light A */
#define LED_RED_A 0
#define LED_YELLOW_A 1
#define LED_GREEN_A 2

/* Macros for the LED positions corresponding
 * to each light for traffic light B */
#define LED_RED_B 4
#define LED_YELLOW_B 5
#define LED_GREEN_B 6

/* Macros for the LED position corresponding
 * to each light for traffic light C */
#define LED_RED_C 8
#define LED_YELLOW_C 9
#define LED_GREEN_C 10

/* ################################################### Extern variable declarations below ############################################ */


/* ################################################### Function declarations follow below ############################################ */

void writeR(struct trafficLight trafficLight);
void writeRY(struct trafficLight trafficLight);
void writeG(struct trafficLight trafficLight);
void writeY(struct trafficLight trafficLight);

#endif /* TRAFFIC_LIGHT_OUTPUT_H_ */
