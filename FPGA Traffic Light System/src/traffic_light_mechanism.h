#ifndef TRAFFIC_LIGHT_MECHANISM_H_
#define TRAFFIC_LIGHT_MECHANISM_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: traffic_light_mechanism.h
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
 * Description: This header file contains the function declarations that will be defined
 * 				in the traffic_light_mechanism.c file. Moreover, it also contains macro
 * 				definitions that are used to enhance code readability and global variables
 * 				that will be accessed throughout the program to keep track of the state
 * 				of the traffic lights.
 *
 *				Important macro definitions follow below:
 *
 *				1. TRAFFIC_LIGHT_A : Defined as 0 and will be the ID-Code for the first traffic light
 *				2. TRAFFIC_LIGHT_B : Defined as 1 and will be the ID-Code for the second traffic light
 *				3. TRAFFIC_LIGHT_C : Defined as 2 and will be the ID-Code for the third traffic light
 *				4. STATES : Defined as 5 and indicates the number of states that a traffic light can traverse
 *							R -> RY -> G -> Y -> R
 *
 *				Important variables follow below:
 *
 *				1. TRAFFIC_LIGHT : Extern global variable that will be used to keep track of the current
 *								   traffic light (A, B and C) that is being operated
 *				2. STATE_COUNTER : Extern global variable that will be used to keep track of the current state
 *								   that the traffic light is at
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "xgpio.h"		// Added for xgpio object definitions

/* Struct */
struct trafficLight {
	XGpio lightR;
	XGpio yellowVGA;
	XGpio lightG;
	u8 LED_R;
	u8 LED_Y;
	u8 LED_G;
	u8 ID;
};

/* ################################################### Macro constants follow below ################################################# */

/* Macro definitions for the three traffic lights */
#define TRAFFIC_LIGHT_A 19
#define TRAFFIC_LIGHT_B 20
#define TRAFFIC_LIGHT_C 21

#define STATES 5

/* ################################################### Extern variable declarations below ############################################ */

/* Global variables that will be accessed throughout the program */
extern u8 TRAFFIC_LIGHT;     //Will hold which traffic light should be operated under any given point
extern u8 STATE_COUNTER;     //Will hold the current state of the traffic lights

struct trafficLight trafficLightA;
struct trafficLight trafficLightB;
struct trafficLight trafficLightC;

/* ################################################### Function declarations follow below ############################################ */

void initialiseTrafficLights();
void operateTrafficLights();
void updateTrafficLight();
void updateStateCounter();

#endif /* TRAFFIC_LIGHT_MECHANISM_H_ */
