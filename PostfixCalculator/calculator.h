#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "cqueue.h"
#include "calculatorReturnType.h"

#define MAX_STRING_LEN 100 // max length including '\0'

/**
* Gets string arithmetic expression and calculates it
* returns SUCCESS 
* returns FAILURE if no memory available for calculation
* returns INVALID_EXPRESSION
*/
ret_t postfixCalculator(const char expression[], double* result);

#endif