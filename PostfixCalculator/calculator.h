#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "cqueue.h"

#define STRING_LEN 100	// Maximum string length

#define DEBUG 1

typedef enum CalculatorReturnCode {
	SUCCESS,
	FAILURE,
	INVALID_EXPRESSION
} ret_t;

/**
* Gets string arithmetic expression and calculates it
*/
ret_t postfixCalculator(char expression[], double* result);

#endif