#ifndef MODE_H_
#define MODE_H_

#include "xil_types.h"		// Added for integer type definitions

/*
 * Header file contains the mode
 * switching mechanisms and integer encodings for the
 * different representation modes
 */

/* Define extern integers that will be used globally*/
extern u16 NUMBER_MODE;
extern u16 ANGLE_MODE;

/* Integer encoding for different number representations*/
#define DECIMAL 0
#define HEX 1

/* Integer encodings for different angle representations*/
#define RADIANS 0
#define DEGREES 1

/* Function prototypes*/
void switch_number_mode(u16 *previous);
void switch_angle_mode(u16 *previous);

#endif /* MODE_H_ */
