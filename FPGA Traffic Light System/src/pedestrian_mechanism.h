#ifndef PEDESTRIAN_MECHANISM_H_
#define PEDESTRIAN_MECHANISM_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: pedestrian_mechanism.h
 *
 * Project Name: TrafficLight
 *
 * Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA
 * Tool Version: Xilinx SDK 2015.2
 *
 * Name: Fadi Barazi
 * Company: University of Edinburgh
 * Creation Date and Time: 14/11/2021; 15:39
 *
 * Description: This header file contains the function declarations that will be defined
 * 				in the pedestrian_mechanism.c file. Moreover, it also contains macro
 * 				definitions that are used to enhance code readability and global variables
 * 				that will be accessed throughout the program to keep track of the pedestrian
 * 				mode.
 *
 */


/* ################################################### Module imports follow below ################################################# */

#include "xil_types.h"	// Added for integer type definitions

/* ################################################### Macro constants follow below ################################################# */

#define PED_TIME_SLICE 0.2
#define PED_LIGHT_TIME 5
#define PED_BLINK_TIME 2

#define PED_NUMBER_TIME_SLICES (PED_LIGHT_TIME / PED_TIME_SLICE)
#define PED_NUMBER_BLINK_TIME_SLICES (PED_BLINK_TIME / PED_TIME_SLICE)

/* Macro for the LED position corresponding
 * to the pedestrian button */
#define LED_PEDESTRIAN 15

/* Macro for the LED position corresponding
 * to the pedestrian indicator light*/
#define LED_INDICATOR_PEDESTRIAN 14


/* ################################################### Extern variable declarations below ############################################ */

extern u8 PEDESTRIAN_MODE;

/* ################################################### Function declarations follow below ############################################ */

void initialisePedestrianLights();
void operatePedestrianLight();
u8 probePedestrianButton();
void turnPedestrianLightOnGreen();
void turnPedestrianLightOff() ;
void updatePedestrianTimeInterval();
void turnPedestrianLightOnRed();


#endif /* PEDESTRIAN_MECHANISM_H_ */
