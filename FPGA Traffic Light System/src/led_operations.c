/*
 * led_operations.c
 *
 *  Created on: 15 Nov 2021
 *      Author: pirat
 */

#include "led_operations.h"
#include "gpio_init.h"

u16 LED_VALUE = 0;

void writeLED(u8 LED) {

	if (LED_VALUE & (1 << LED)) {
		return;
	}

	u16 out = 1 << LED;

	LED_VALUE = LED_VALUE + out;

	XGpio_DiscreteWrite(&LED_OUT, 1, LED_VALUE);

}

void removeLED(u8 LED) {

	if (!(LED_VALUE & (1 << LED))) {
		return;
	}

	u16 remove = 1 << LED;

	LED_VALUE = LED_VALUE - remove;

	XGpio_DiscreteWrite(&LED_OUT, 1, LED_VALUE);

}
