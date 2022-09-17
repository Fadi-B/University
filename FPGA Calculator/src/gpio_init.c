#include "gpio_init.h"

/*
 * This function initialises all the relevant XGpio objects
 *
 * @Return
 * 	-> XStatus indicating if the initialisation was a success or failure
 */
XStatus init_gpio(void)
{
	XStatus status;

	/* Note that all the required Xgpio objects
	 * have been declared in the gpio_init.h header file
	 * but confirm that the IDs used to initialize
	 * the GPIOs here are correct by inspecting the xparameters.h file
	 * to avoid any bugs!
	 */

	//XGpio object used for the 7 segments display
	status = XGpio_Initialize(&SEG7_HEX_OUT, 0);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object used for deciding which 7 segment display to use
	status = XGpio_Initialize(&SEG7_SEL_OUT, 1);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object for the LEDs
	status = XGpio_Initialize(&LED_OUT, 2);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object used for the down button
	status = XGpio_Initialize(&P_BTN_DOWN, 3);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object used for the left button
	status = XGpio_Initialize(&P_BTN_LEFT, 4);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object used for the right button
	status = XGpio_Initialize(&P_BTN_RIGHT, 5);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object used for the up button
	status = XGpio_Initialize(&P_BTN_UP, 6);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//XGpio object used for the slide switches
	status = XGpio_Initialize(&SLIDE_SWITCHES, 7);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
