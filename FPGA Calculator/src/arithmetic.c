#include "arithmetic.h"
#include "mode.h"
#include "math.h"

/* Mathematical constants used */
const double pi = 4. * atan(1.);

/*
 * This function performs the addition of two input numbers
 *
 * @Param
 * 	-> augend - the term that a number should be added to
 * 	-> addend - the number that should be added
 *
 * @Return
 * 	-> floating point number of the addition result
 */
float adder(float augend, float addend) {
	float sum;
	sum = augend + addend;
	return sum;
}

/*
 * This function performs the subtraction of two input numbers
 *
 * @Param
 * 	-> minuend - the term that a number should be subtracted from
 * 	-> subtrahend - the number that should be subtracted
 *
 * @Return
 * 	-> floating point number of the subtraction result
 */
float subtractor(float minuend, float subtrahend) {
	float difference;
	difference = minuend - subtrahend;
	return difference;
}

/*
 * This function performs the multiplication of two input numbers
 *
 * @Param
 * 	-> multiplicand - the term that should be multiplied
 * 	-> multiplier - the factor by which the multiplicand should be multiplied with
 *
 * @Return
 * 	-> floating point number of the multiplication result
 */
float multiplicator(float multiplicand, float multiplier) {
	float product;
	product = multiplicand * multiplier;
	return product;
}

/*
 * This function performs the division of two input numbers
 *
 * @Param
 * 	-> divident - the term that should be divided
 * 	-> divisor - the factor that the divident should be divided by
 *
 * @Return
 * 	-> floating point number of the division result
 */
float divider(float divident, float divisor) {
	float quotient;
	quotient = divident / divisor;
	return quotient;
}

/*
 * This function performs the power function
 *
 * @Param
 * 	-> base - the number that should be raised to a certain power
 * 	-> exponent - the term that the base should be raised by
 *
 * @Return
 * 	-> floating point number of the base raised by the exponent value
 */
float power(float base, float exponent) {
	float result;
	result = pow(base, exponent);
	return result;
}

/*
 * This function performs the exponential function
 *
 * @Param
 * 	-> number - the number that the euler number should be raised by
 *
 * @Return
 * 	-> floating point number of the exponential result
 */
float exponential(float number) {
	float exponential;
	exponential = exp(number);
	return exponential;
}

/*
 * This function performs the square root function
 *
 * @Param
 * 	-> number - the number whose square root should be taken
 *
 * @Return
 * 	-> floating point number of square root of the number
 */
float square_root(float number) {
	float result;
	result = sqrt(number);
	return result;
}

/*
 * This function performs the sine function and operates
 * in both degrees and radians depending on the specified
 * parameters.
 *
 *
 * @Param
 * 	-> number - float number whose sine value should be found
 * 	-> angleMode - integer encoding of degree or radian mode,
 * 					the angle mode encoding can be found in "mode.h"
 *
 * @Return
 * 	-> floating point number of the sine of the number
 */
float sine(float number, u16 angleMode) {
	float result;

	if (angleMode == DEGREES) { //Convert to radians as math.h sin function only operates on radians
		number = convert_to_radians(number);
	}

	result = sin(number);

	return result;
}

/*
 * This function performs the cosine function and operates
 * in both degrees and radians depending on the specified
 * parameters.
 *
 *
 * @Param
 * 	-> number - float number whose cosine value should be found
 * 	-> angleMode - integer encoding of degree or radian mode,
 * 					the angle mode encoding can be found in "mode.h"
 *
 * @Return
 * 	-> floating point number of the cosine of the number
 */
float cosine(float number, u16 angleMode) {
	float result;

	if (angleMode == DEGREES) { //Convert to radians as math.h sin function only operates on radians
		number = convert_to_radians(number);
	}

	result = cos(number);

	return result;
}

/*
 * This function performs the tangent function and operates
 * in both degrees and radians depending on the specified
 * parameters.
 *
 *
 * @Param
 * 	-> number - float number whose tangent value should be found
 * 	-> angleMode - integer encoding of degree or radian mode,
 * 					the angle mode encoding can be found in "mode.h"
 *
 * @Return
 * 	-> floating point number of the cosine of the number
 */
float tangent(float number, u16 angleMode) {
	float result;

	if (angleMode == DEGREES) {
		number = convert_to_radians(number);
	}

	result = tan(number);

	return result;
}

/*
 * This function performs the arcsine function and operates
 * in both degrees and radians depending on the specified
 * parameters.
 *
 *
 * @Param
 * 	-> number - float number whose arcsine value should be found
 * 	-> angleMode - integer encoding of degree or radian mode,
 * 					the angle mode encoding can be found in "mode.h"
 *
 * @Return
 * 	-> floating point number of the arcsine of the number
 */
float arcsine(float number, u16 angleMode) {
	float result;

	result = asin(number);

	if (angleMode == DEGREES) { //Convert to degrees as math.h asin outputs radian value
		result = convert_to_degrees(result);
	}

	return result;
}

/*
 * This function performs the arccosine function and operates
 * in both degrees and radians depending on the specified
 * parameters.
 *
 *
 * @Param
 * 	-> number - float number whose arccosine value should be found
 * 	-> angleMode - integer encoding of degree or radian mode,
 * 					the angle mode encoding can be found in "mode.h"
 *
 * @Return
 * 	-> floating point number of the arccosine of the number
 */
float arccosine(float number, u16 angleMode) {
	float result;

	result = acos(number);

	if (angleMode == DEGREES) { //Convert to degrees as math.h acos outputs radian value
		result = convert_to_degrees(result);
	}

	return result;
}

/*
 * This function performs the arctangent function and operates
 * in both degrees and radians depending on the specified
 * parameters.
 *
 *
 * @Param
 * 	-> number - float number whose arctangent value should be found
 * 	-> angleMode - integer encoding of degree or radian mode,
 * 					the angle mode encoding can be found in "mode.h"
 *
 * @Return
 * 	-> floating point number of the arctangent of the number
 */
float arctangent(float number, u16 angleMode) {
	float result;

	result = atan(number);

	if (angleMode == DEGREES) { //Convert to degrees as math.h atan outputs radian value
		result = convert_to_degrees(result);
	}

	return result;
}
/*
 * This function performs the natural logarithm function
 *
 * @Param
 * 	-> number - the number whose natural logarithm should be taken
 *
 * @Return
 * 	-> floating point number of the natural logarithm of the specified number
 */
float logarithm_natural(float number) {
	float result;
	result = log(number);
	return result;
}

/*
 * This function performs the logarithm function
 * in which the base can be specified.
 *
 * The function is based on the change of base rule
 * for logarithms.
 *
 * @Param
 * 	-> number - the number whose logarithm should be taken
 *	-> base - the base of the logarithm
 *
 * @Return
 * 	-> floating point value of the logarithm of the specified number with respect
 * 	   to the specified base
 */
float logarithm(float number, u16 base) {
	float result;

	float numerator = log(number);
	float denominator = log(base);

	result = numerator / denominator; //Apply change of base rule
	return result;
}

/*
 * This function converts the specified radian angle
 * into degrees.
 *
 * @Param
 * 	-> number - the radian value that should be converted to degrees
 *
 * @Return
 * 	-> floating point value of the equivalent degree representation
 */
float convert_to_degrees(float number) {
	return number * (180 / pi);
}

/*
 * This function converts the specified degree angle
 * into radians.
 *
 * @Param
 * 	-> number - the degree value that should be converted to radians
 *
 * 	@Return
 * 	-> floating point value of the equivalent radians representation
 */
float convert_to_radians(float number) {
	return number * (pi / 180);
}
