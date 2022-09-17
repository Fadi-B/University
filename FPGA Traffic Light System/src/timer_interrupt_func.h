#ifndef TIMER_INTERRUPT_FUNC_H_
#define TIMER_INTERRUPT_FUNC_H_

/* ################################################### File Description follows below ################################################# */

/*
 * File Name: timer_interrupt_func.h
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
 * 				in the timer_interrupt_func.h file.
 *
 *				The main purpose of this module is to provide the Interrupt Service Routine framework
 *				that will be used by the program in order to create the required timing mechanism for the
 *				correct operation of the traffic lights.
 *
 *				Most important function in this  module:
 *
 *					-> The hwTimerISR() as this will be called each time an interrupt is received and hence it will
 *					   determine the code that should be executed when the interrupt is received.
 *
 *					   Note: The hardware files are currently set such that we receive an interrupt every 4ms
 *
 *				Utility functions to perform the required operations when hwTimerISR() is called constitute:
 *
 *					1. operatePedestrianLightInterrupt()
 *					2. operateTrafficLightInterrupt()
 *					3. operateTimeElapsedInterrupt()
 *
 *				These are described in the corresponding source file
 *
 *				Note: The setUpInterruptSystem() function is declared in this file, but its definition can be found
 *					  in the xinterruptES3.c file.
 *
 *					  Its main purpose is to setup the interrupt system when the application is launched and so it should
 *					  be called in the main function before the main while loop is entered.
 *
 */

/* ################################################### Module imports follow below ################################################# */


/* ################################################### Macro constants follow below ################################################# */

#define INTERRUPT_TIME 0.004
#define PED_INTERRUPT_TIME 0.2
#define LIGHT_DURATION_TIME 1
#define ELAPSED_TIME_INTERRUPT_TIME 0.1

#define PED_INTERRUPT_COUNTER_LIM (PED_INTERRUPT_TIME / INTERRUPT_TIME)
#define LIGHT_INTERRUPT_COUNTER_LIM (LIGHT_DURATION_TIME / INTERRUPT_TIME)
#define ELAPSED_TIME_INTERRUPT_COUNTER_LIM (ELAPSED_TIME_INTERRUPT_TIME / INTERRUPT_TIME)

/* ################################################### Function declarations follow below ############################################ */


/* Declarations for function responsible
 * for setting up the interrupt system¨
 *
 * Definition can be found in the xinterruptES3.c file */
int setUpInterruptSystem();

/* Declarations for the timer_interrupt_func.c functions */
void hwTimerISR(void *CallbackRef);
void operatePedestrianLightInterrupt();
void operateTrafficLightInterrupt();
void operateTimeElapsedInterrupt();

#endif /* TIMER_INTERRUPT_FUNC_H_ */
