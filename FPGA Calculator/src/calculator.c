#include "calculator.h"
#include "arithmetic.h"
#include "mode.h"

/*
 * This function computes the specified operation given two input numbers.
 *
 * Note that if a certain mathematical function only takes one argument
 * then the first_input_number parameter is the one used.
 *
 * @Param
 * 	-> operation - the integer code corresponding to a particular operation,
 * 				   the available integer codes can be found in "calculator.h"
 *
 * 	-> firstInput - the first number that should be used in the computation
 * 	-> secondInput - the second number that should be used in the computation
 *
 * @Return
 * 	-> floating point value corresponding to the computed result
 */
float calculate(u16 operation, float firstInput, s16 secondInput) {

	float computation = 0;

	switch (operation)
	{
		case NONE : //No specified operation implies replace previous display value with first input number
			computation = firstInput;
			break;
		case ADD :
			computation = adder(firstInput, secondInput);
			break;
		case SUB :
			computation = subtractor(firstInput, secondInput);
			break;
		case MULT :
			computation = multiplicator(firstInput, secondInput);
			break;
		case DIV :
			computation = divider(firstInput, secondInput);
			break;
		case POW :
			computation = power(firstInput, secondInput);
			break;
		case EXP :
			computation = exponential(firstInput);
			break;
		case SQRT :
			computation = square_root(firstInput);
			break;
		case SINE :
			computation = sine(firstInput, ANGLE_MODE);
			break;
		case COS :
			computation = cosine(firstInput, ANGLE_MODE);
			break;
		case TAN :
			computation = tangent(firstInput, ANGLE_MODE);
			break;
		case ARC_SINE :
			computation = arcsine(firstInput, ANGLE_MODE);
			break;
		case ARC_COS :
			computation = arccosine(firstInput, ANGLE_MODE);
			break;
		case ARC_TAN :
			computation = arctangent(firstInput, ANGLE_MODE);
			break;
		case LOG :
			computation = logarithm(firstInput, secondInput);
			break;
		case NAT_LOG :
			computation = logarithm_natural(firstInput);
			break;
		default:
			computation = firstInput;
	}

	return computation;
}
