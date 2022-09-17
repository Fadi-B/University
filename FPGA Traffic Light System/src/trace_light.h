#ifndef TRACE_H_
#define TRACE_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: trace_light.h
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
 * 				in the trace_light.c file. Moreover, it also contains macro
 * 				definitions that are used to enhance code readability and global variables
 * 				that will be accessed throughout the program to keep track of the mode of the
 * 				traffic light tracing.
 *
 * 				Important macros follow below:
 *
 * 				1. NUMBER_OF_LIGTS : Defined as three as the brittish traffic lights have 3 lights, namely red, yellow and green
 * 				2. LIGHT_DURATION : Defined as the time allocated to each light during tracing of the traffic light
 *
 * 				Important variables follow below:
 *
 * 				1. traceFinished : extern global variable that will be used to determine if a tracing procedure
 * 								   for a traffic light has finished
 *
 *
 *
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "xil_types.h"		// Added for integer type definitions
#include "xgpio.h"
#include "gpio_init.h"
#include "traffic_light_mechanism.h"

/* ################################################### Macro constants follow below ################################################# */

#define NUMBER_OF_LIGHTS 3

/* ################################################### Extern variable declarations below ############################################ */

u8 extern traceFinished;

/* ################################################### Function declarations follow below ############################################ */

void traceLightA();
void traceLightB();
void traceLightC();
void traceLight(struct trafficLight trafficLight);
void updateLight(u16 state, struct trafficLight trafficLight);
void traceRegion();

#endif /* TRACE_H_ */
