#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "cqueue.h"

#define STRING_LEN 100	// Maximum string length

typedef enum CalculatorReturnCode {
	SUCCESS,
	FAILURE,
	INVALID_EXPRESSION
} ret_t;

/**
* Gets string arithmetic expression and calculates it
*/
void postfixCalculator(char expression[], double* result);

#endif