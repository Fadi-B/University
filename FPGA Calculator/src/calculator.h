#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <stdio.h>
#include "platform.h"
#include "xil_types.h"

/* Header file includes calculator functions and
 * integer encodings for the different mathematical operations
 */

/* The constants below define integer codes for the mathematical operation
 * values specified by the top 4 slide switches (slide switches 16-13) */
#define NONE 0
#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define POW 5
#define EXP 6
#define SQRT 7
#define SINE 8
#define COS 9
#define TAN 10
#define ARC_SINE 11
#define ARC_COS 12
#define ARC_TAN 13
#define LOG 14	   //Normal logarithm in which a base can be specified
#define NAT_LOG 15 //Natural logarithm


float calculate(u16 operation, float current_number, s16 input_number);

#endif /* CALCULATOR_H_ */
