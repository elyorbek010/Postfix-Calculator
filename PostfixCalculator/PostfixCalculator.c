// PostfixCalculator.cpp : Defines the functions for the static library.
//

#include "calculator.h"
#include "cqueue.h"

cqueue_t* parser(char* expression) {
	cqueue_t* queue = queue_create(EXPR_LEN);
}

double PostfixCalculator(char * expression)
{
	cqueue_t* cqueue = parser(expression);
}
