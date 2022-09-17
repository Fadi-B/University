#ifndef __ARITHMETIC_H_
#define __ARITHMETIC_H_

#include <stdio.h>
#include "platform.h"
#include "xil_types.h" //Used for u16

/**
 * Header file includes the function declarations for
 * the different arithmetic operations that the calculator
 * should be able to perform
 *
 * Additional mathematical functionalities should build upon this file
 */

/* Mathematical operations that calculator should perform*/
float adder(float augend, float addend);
float subtractor(float minuend, float subtrahend);
float multiplicator(float multiplicand, float multiplier);
float divider(float divident, float divisor);
float power(float base, float exponent);
float exponential(float number);
float square_root(float number);
float sine(float number, u16 angleMode);
float cosine(float number, u16 angleMode);
float tangent(float number, u16 angleMode);
float arcsine(float number, u16 angleMode);
float arccosine(float number, u16 angleMode);
float arctangent(float number, u16 angleMode);
float logarithm_natural(float number);
float logarithm(float number, u16 base);

/* Utility functions for implementing some mathematical operations*/
float convert_to_degrees(float number);
float convert_to_radians(float number);

#endif
