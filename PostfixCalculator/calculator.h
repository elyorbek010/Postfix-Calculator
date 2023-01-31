#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "cqueue.h"

#define EXPR_LEN 100

/**
* Gets string expression, returns answer of the expression
*/
double PostfixCalculator(char * expression);

/**
* Gets string expression, returns queue of tokens
*/
cqueue_t* parser(char* expression);

#endif