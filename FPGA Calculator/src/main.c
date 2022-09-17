#include <stdio.h>
#include "platform.h"
#include "xil_types.h"		// Added for integer type definitions
#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"

#include "calculator.h"		// Added for calculate functionality
#include "input_extract.h"	// Added for input extraction functionalities

#include "mode.h"			// Added for different number and angle representations

/* Helper function prototypes*/
void wait_until_release(int displayValue, XGpio *instance);

int main()
{
	init_platform();
	int status;

    // Initialise the GPIOs
    status = init_gpio();
	if (status != XST_SUCCESS) {
		print("GPIOs initialization failed!\n\r");
		cleanup_platform();
		return 0;
	}

	// Setup the Interrupt System
	status = set_up_interrupt_system();
	if (status != XST_SUCCESS) {
		print("Interrupt system setup failed!\n\r");
		cleanup_platform();
		return 0;
	}

	u16 slideSwitchIn = 0;			//Will hold input from slide switches

	u16 ENTER = 0;					//Will hold input from right button
	u16 PREVIOUS_ANSWER = 0;		//Will hold input from up button - will operate as ANS in normal calculator

	u16 SWITCH_ANGLE_MODE = 0;		//Will hold input from down button - trigger for angle representation
	u16 SWITCH_NUMBER_MODE = 0;		//Will hold input from left button - trigger for number representation
	
	u16 PREVIOUS_ANGLE_MODE = 0;	//Will hold encoding for previous angle representation (deg or rad)
	u16 PREVIOUS_NUMBER_MODE = 0;	//Will keep track of the previous number representation (hex or dec)

	s16 firstInputNumber = 0;		//Will hold the value of the first input number
	s16 secondInputNumber = 0;		//Will hold the value of the second input number

	float currentNumber = 0;		//Will hold the value of the calculator computed value

	u16 inputOperation = 0;			//Will hold the value of the input operation

	while (1)
	{

		//Reading input from the relevant GPIO ports
		ENTER = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		PREVIOUS_ANSWER = XGpio_DiscreteRead(&P_BTN_UP, 1);
		SWITCH_NUMBER_MODE = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		SWITCH_ANGLE_MODE = XGpio_DiscreteRead(&P_BTN_DOWN, 1);

		slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);

		//Extract the input numbers and operation type
		firstInputNumber = extract_input_number(slideSwitchIn, FIRST_INPUT_UPPER_BIT, FIRST_INPUT_LOWER_BIT);
		secondInputNumber = extract_input_number(slideSwitchIn, SECOND_INPUT_UPPER_BIT, SECOND_INPUT_LOWER_BIT);

		inputOperation = extract_input_operation(slideSwitchIn);

		show_input_numbers_on_LEDs(slideSwitchIn); //Display input numbers on LED output

		if (ENTER) {
			wait_until_release(currentNumber, &P_BTN_RIGHT);
			currentNumber = calculate(inputOperation, firstInputNumber, secondInputNumber);
		}

		if (PREVIOUS_ANSWER) {		//Calculate result using first input and previous computed value that is on display
			wait_until_release(currentNumber, &P_BTN_UP);
			currentNumber = calculate(inputOperation, currentNumber, firstInputNumber);
		}

		if (SWITCH_NUMBER_MODE) {	//Default is decimal number representation
			wait_until_release(currentNumber, &P_BTN_LEFT);
			switch_number_mode(&PREVIOUS_NUMBER_MODE);
		}

		if (SWITCH_ANGLE_MODE) {	//Default is radian angle representation
			wait_until_release(currentNumber, &P_BTN_DOWN);
			switch_angle_mode(&PREVIOUS_ANGLE_MODE);
		}

		display_number(currentNumber);	//Display the computed value

	}

    cleanup_platform();
    return 0;
}

/*
 * This function waits until the specified XGpio instance is
 * released before allowing code execution to continue.
 *
 * @Param
 * 	-> displayValue - the value that should be displayed on the 7-segment display during wait
 * 	-> instance - the XGpio object that we want to be released
 */
void wait_until_release(int displayValue, XGpio *instance) {
	u16 EXIT = 0;
	u16 value = 0;

	while (!EXIT) {
		display_number(displayValue);
		value = XGpio_DiscreteRead(instance, 1);

		if (value == 0) {
			EXIT = 1;
		}
	}
}
