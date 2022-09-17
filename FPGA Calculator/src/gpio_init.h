#ifndef __GPIO_INIT_H_
#define __GPIO_INIT_H_

#include "xgpio.h"		// Added for XGpio object definitions

/*
 * Header file includes GPIO initialisation functionalities and
 * declarations of the relevant GPIO objects that will be used
 */

XStatus init_gpio(void);

XGpio SEG7_HEX_OUT;		//7-segment output CA - CG
XGpio SEG7_SEL_OUT;		//7-segment display selection output
XGpio P_BTN_LEFT;		//Left button 	(BTNL)
XGpio P_BTN_RIGHT;		//Right button  (BTNR)
XGpio P_BTN_UP;			//Up button 	(BTNU)
XGpio P_BTN_DOWN;		//Down button   (BTND)
XGpio LED_OUT;			//LEDs corresponding to slide switches
XGpio SLIDE_SWITCHES;   //16-bit slide switches on the board

#endif
