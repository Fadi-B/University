#include "seg7_display.h"

/* This function determines the code that should be
 * executed each time an interrupt is received
 *
 * Note: The hardware files have set the interrupt to occur every 4ms
 *
 */
void hw_timer_ISR(void *CallbackRef)
{
	display_digit();
}
