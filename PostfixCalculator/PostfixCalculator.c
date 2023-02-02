// postfixCalculator.c : Defines the functions for the static library.
//
#include "calculator.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>

#define QUEUE_CAPACITY (2 * MAX_STRING_LEN)

static inline ret_t inf2post(cqueue_t* infix_token_queue, cqueue_t* postfix_token_queue) {
	
	// TRANSLATE INF TO POST

	return SUCCESS;
}

static inline ret_t post_calc(cqueue_t* postfix_queue, double* result) {
	return SUCCESS;
}


//before returning every function should free queue;
ret_t postfixCalculator(char expression[], double* result) {
	ret_t returns = SUCCESS;

	if (strlen(expression) >= MAX_STRING_LEN - 1)
		return INVALID_EXPRESSION;

	cqueue_t* queue_infix = NULL;
	cqueue_t* queue_postfix = NULL;

	//FREE QUEUE/////////////////////////////
	if (!((queue_infix = queue_create(QUEUE_CAPACITY)) && (queue_postfix = queue_create(QUEUE_CAPACITY))))
		return FAILURE; // if either of queues is NULL, return FAILURE

	returns = parser(expression, queue_infix); // parser pushes tokens into queue

	if (returns != SUCCESS) {
		return returns;
	}

	returns = inf2post(queue_infix, queue_postfix); // inf2post arranges tokens into postfix sequence

	if (returns != SUCCESS) {
		return returns;
	}

	returns = post_calc(queue_postfix, result); // calculates postfix and puts answer into result

	return returns;
}
