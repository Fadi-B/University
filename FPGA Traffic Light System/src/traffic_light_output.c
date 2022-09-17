/* ################################################### File Description follows below ################################################# */

/*
 * File Name: traffic_light_output.c
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
 *				The functions contained in this source file will be used to output the correct signals to the
 *				relevant GPIOs such that the LEDs and the VGA will simulate the required traffic light sequence.
 *				This file contains 8 functions.
 *
 *
 *				1. The writeR() function will write the colour RED to the VGA region corresponding to the
 *				   red light of the specified traffic light
 *
 *
 *				2. The writeRY() function will write the colour RED and the colour YELLOW to the VGA regions corresponding to the
 *				   red and yellow lights respectively of the specified traffic light
 *
 *				3. The writeG() function will write the colour GREEN to the VGA region corresponding to the
 *				   green light of the specified traffic light
 *
 *				4. The writeY() function will write the colour YELLOW to the VGA region corresponding to the
 *				   yellow light of the specified traffic light
 *
 *				5. The writeLED_R() function will turn on the LED that corresponds to the red light
 *				   of the specified traffic light.
 *
 *				6. The writeLED_RY() function will turn on the LEDs that correspond to the red and yellow lights
 *				   of the specified traffic light.
 *
 *				7. The writeLED_G function will turn on the LED that corresponds to the green light
 *			  	   of the specified traffic light.
 *
 *			  	8. The writeLED_Y function will turn on the LED that corresponds to the yellow light
 *			  	   of the specified traffic light.
 *
 *
 */

/* ################################################### Module imports follow below ################################################# */

#include "traffic_light_output.h"
#include "traffic_light_mechanism.h"
#include "led_operations.h"

/* ################################################### Global variables follow below ############################################### */


/* ################################################### Function definitions follow below ############################################ */

/*
 * This function will write the colour red to the
 * traffic light that has been specified.
 *
 * Note: The function knows which traffic light to write to
 * 		 based on the XGpio array of regions objects that are provided.
 * 		 This array should contain the 3 VGA regions that constitute the
 * 		 traffic light of interest and these regions should be sorted as follows,
 *
 * 		 -> Element 1 : The region that will indicate the colour red for the traffic light
 * 		 -> Element 2 : The region that will indicate the colour yellow for the traffic light
 * 		 -> Element 3 : The region that will indicate the colour green for the traffic light
 *
 * @Param
 * 	-> regions : XGpio array containing the regions that constitute the traffic light of interest
 *
 * 	@Return : None
 */
void writeR(struct trafficLight trafficLight) {

	XGpio_DiscreteWrite(&trafficLight.lightR, 1, RED);			//Write RED to correct VGA Region
	XGpio_DiscreteWrite(&trafficLight.yellowVGA, 1, WHITE);			//Overwrite previous colour such that only RED is shown
	XGpio_DiscreteWrite(&trafficLight.lightG, 1, WHITE);			//Overwrite previous colour such that only RED is shown

	writeLED(trafficLight.LED_R);
	removeLED(trafficLight.LED_Y);
	removeLED(trafficLight.LED_G);

}

/*
 * This function will write the colour red and the colour yellow to the
 * traffic light that has been specified.
 *
 * Note: The function knows which traffic light to write to
 * 		 based on the XGpio array of regions objects that are provided.
 * 		 This array should contain the 3 VGA regions that constitute the
 * 		 traffic light of interest and these regions should be sorted as follows,
 *
 * 		 -> Element 1 : The region that will indicate the colour red for the traffic light
 * 		 -> Element 2 : The region that will indicate the colour yellow for the traffic light
 * 		 -> Element 3 : The region that will indicate the colour green for the traffic light
 *
 * @Param
 * 	-> regions : XGpio array containing the regions that constitute the traffic light of interest
 *
 * 	@Return : None
 */
void writeRY(struct trafficLight trafficLight) {

	XGpio_DiscreteWrite(&trafficLight.lightR, 1, RED);			//Write RED to correct VGA Region
	XGpio_DiscreteWrite(&trafficLight.yellowVGA, 1, YELLOW);		//Write YELLOW to correct VGA Region
	XGpio_DiscreteWrite(&trafficLight.lightG, 1, WHITE);			//Overwrite previous colour such that only RY shown

	writeLED(trafficLight.LED_R);
	writeLED(trafficLight.LED_Y);
	removeLED(trafficLight.LED_G);

}

/*
 * This function will write the colour green to the
 * traffic light that has been specified.
 *
 * Note: The function knows which traffic light to write to
 * 		 based on the XGpio array of regions objects that are provided.
 * 		 This array should contain the 3 VGA regions that constitute the
 * 		 traffic light of interest and these regions should be sorted as follows,
 *
 * 		 -> Element 1 : The region that will indicate the colour red for the traffic light
 * 		 -> Element 2 : The region that will indicate the colour yellow for the traffic light
 * 		 -> Element 3 : The region that will indicate the colour green for the traffic light
 *
 * @Param
 * 	-> regions : XGpio array containing the regions that constitute the traffic light of interest
 *
 * 	@Return : None
 */
void writeG(struct trafficLight trafficLight) {

	XGpio_DiscreteWrite(&trafficLight.lightR, 1, WHITE);			//Overwrite previous colour such that only G shown
	XGpio_DiscreteWrite(&trafficLight.yellowVGA, 1, WHITE);		//Overwrite previous colour such that only G shown
	XGpio_DiscreteWrite(&trafficLight.lightG, 1, GREEN);			//Write GREEN to correct VGA Region

	removeLED(trafficLight.LED_R);
	removeLED(trafficLight.LED_Y);
	writeLED(trafficLight.LED_G);
}

/*
 * This function will write the colour yellow to the
 * traffic light that has been specified.
 *
 * Note: The function knows which traffic light to write to
 * 		 based on the XGpio array of regions objects that are provided.
 * 		 This array should contain the 3 VGA regions that constitute the
 * 		 traffic light of interest and these regions should be sorted as follows,
 *
 * 		 -> Element 1 : The region that will indicate the colour red for the traffic light
 * 		 -> Element 2 : The region that will indicate the colour yellow for the traffic light
 * 		 -> Element 3 : The region that will indicate the colour green for the traffic light
 *
 * @Param
 * 	-> regions : XGpio array containing the regions that constitute the traffic light of interest
 *
 * 	@Return : None
 */
void writeY(struct trafficLight trafficLight) {

	XGpio_DiscreteWrite(&trafficLight.lightR, 1, WHITE);			//Overwrite previous colour such that only Y shown
	XGpio_DiscreteWrite(&trafficLight.yellowVGA, 1, YELLOW);		//Write YELLOW to correct VGA Region
	XGpio_DiscreteWrite(&trafficLight.lightG, 1, WHITE);			//Overwrite previous colour such that only Y shown

	removeLED(trafficLight.LED_R);
	writeLED(trafficLight.LED_Y);
	removeLED(trafficLight.LED_G);

}

