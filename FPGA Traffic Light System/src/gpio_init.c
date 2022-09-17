/* ################################################### File Description follows below ################################################# */

/*
 * File Name: gpio_init.c
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

#include "gpio_init.h"

/* ################################################### Global variables follow below ############################################### */


/* ################################################### Function definitions follow below ############################################ */

/*
 * This function initialises all the GPIOs that
 * have been declared in the corresponding header file.
 *
 *
 * Note: The initialisation makes use of the XGpio_Initialize()
 * 		 function defined in the xgpio.c file. The Device IDs
 * 		 required are obtained from the xparameters.h file
 */
XStatus initGpio(void)
{
	XStatus status;

	/* Initialise the 7-Segment Display port */
	status = XGpio_Initialize(&SEG7_HEX_OUT, 0);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the 7-Segment digit selection port */
	status = XGpio_Initialize(&SEG7_SEL_OUT, 1);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_0 for the VGA */
	status = XGpio_Initialize(&REGION_0_COLOUR, 7);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_1 for the VGA */
	status = XGpio_Initialize(&REGION_1_COLOUR, 10);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_2 for the VGA */
	status = XGpio_Initialize(&REGION_2_COLOUR, 11);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_3 for the VGA */
	status = XGpio_Initialize(&REGION_3_COLOUR, 12);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_4 for the VGA */
	status = XGpio_Initialize(&REGION_4_COLOUR, 13);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_5 for the VGA */
	status = XGpio_Initialize(&REGION_5_COLOUR, 14);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_6 for the VGA */
	status = XGpio_Initialize(&REGION_6_COLOUR, 15);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_7 for the VGA */
	status = XGpio_Initialize(&REGION_7_COLOUR, 16);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_8 for the VGA */
	status = XGpio_Initialize(&REGION_8_COLOUR, 17);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_9 for the VGA */
	status = XGpio_Initialize(&REGION_9_COLOUR, 18);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_10 for the VGA */
	status = XGpio_Initialize(&REGION_10_COLOUR, 8);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_11 for the VGA */
	status = XGpio_Initialize(&REGION_11_COLOUR, 9);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_12 for the VGA */
	status = XGpio_Initialize(&LED_OUT, 2);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_12 for the VGA */
	status = XGpio_Initialize(&P_BTN_RIGHT, 5);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialise the REGION_12 for the VGA */
	status = XGpio_Initialize(&P_BTN_LEFT, 4);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/* Initialize the Slide Switches */
	status = XGpio_Initialize(&SLIDE_SWITCHES, 19);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
