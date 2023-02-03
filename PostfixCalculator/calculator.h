#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "cqueue.h"
#include "CalculatorReturnType.h"

#define MAX_STRING_LEN 100

/**
* Gets string arithmetic expression and calculates it
* returns SUCCESS 
* returns FAILURE if no memory available for calculation
* returns INVALID_EXPRESSION
*/
ret_t postfixCalculator(char expression[], double* result);

#endif