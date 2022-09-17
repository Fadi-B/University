#ifndef __GPIO_INIT_H_
#define __GPIO_INIT_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: gpio_init.h
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
 * 				in the gpio_init.c file. Most importantly, it contains declarations of
 * 				the global XGpio variables that will be used throughout the program.
 *
 *				XGpio declarations:
 *
 *				1.  REGION_0_COLOUR : VGA region corresponding to the first square in the first column
 *				2.  REGION_1_COLOUR : VGA region corresponding to the second square in the first column
 *				3.  REGION_2_COLOUR : VGA region corresponding to the third square in the first column
 *
 *				4.  REGION_3_COLOUR : VGA region corresponding to the first square in the second column
 *				5.  REGION_4_COLOUR : VGA region corresponding to the second square in the second column
 *				6.  REGION_5_COLOUR : VGA region corresponding to the third square in the second column
 *
 *				7.  REGION_6_COLOUR : VGA region corresponding to the first square in the third column
 *				8.  REGION_7_COLOUR : VGA region corresponding to the second square in the third column
 *				9.  REGION_8_COLOUR : VGA region corresponding to the third square in the third column
 *
 *				10. REGION_9_COLOUR : VGA region corresponding to the first square in the fourth column
 *				11. REGION_10_COLOUR : VGA region corresponding to the first square in the fourth column
 *				12. REGION_11_COLOUR : VGA region corresponding to the first square in the fourth column
 *
 *				13. SEG7_SEL_OUT : Digit selection output for the 7-Segment Display
 *				14. SEG7_HEX_OUT : 7-Segment selection output for the 7-Segment Display
 *
 *				15. LED_OUT : LED port output
 *
 *				16. P_BTN_RIGHT : BTNR which will correspond to the pedestrian button
 *
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "xgpio.h"		// Added for xgpio object definitions

/* ################################################### Macro constants follow below ################################################# */


/* ################################################### Variable declarations below ############################################ */

/* XGpio objects for the available VGA regions
 *
 * Note: See above file description for comprehensive overview of each object
 *  */
XGpio REGION_0_COLOUR;
XGpio REGION_1_COLOUR;
XGpio REGION_2_COLOUR;
XGpio REGION_3_COLOUR;
XGpio REGION_4_COLOUR;
XGpio REGION_5_COLOUR;
XGpio REGION_6_COLOUR;
XGpio REGION_7_COLOUR;
XGpio REGION_8_COLOUR;
XGpio REGION_9_COLOUR;
XGpio REGION_10_COLOUR;
XGpio REGION_11_COLOUR;

/* XGpio objects for the 7-Segment Display*/
XGpio SEG7_SEL_OUT;
XGpio SEG7_HEX_OUT;

/* XGpio object for the LED port */
XGpio LED_OUT;

/* XGpio object for the BTNR port
 *
 * Note: Will be used as the pedestrian button
 * */
XGpio P_BTN_RIGHT;
XGpio P_BTN_LEFT;

XGpio SLIDE_SWITCHES;

/* ################################################### Function declarations follow below ############################################ */
XStatus initGpio(void);


#endif /* GPIO_INIT_H_ */
