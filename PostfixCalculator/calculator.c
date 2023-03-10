#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "calculator.h"
#include "parser.h"
#include "debug.h"

#define QUEUE_CAPACITY (2 * MAX_STRING_LEN)

static unsigned int type_priority(tokenType type) {
	switch (type) {
	case B_PLUS: 
	case B_MINUS: return 5;

	case B_MULTIPLY: 
	case B_DIVIDE: return 10;

	case U_PLUS: 
	case U_MINUS: return 15;

	default: return 0;
	}
}

static inline void swap_ptrs(void** ptr1, void** ptr2) {
	void* temp;

	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}

static void free_n_destroy_queue(cqueue_t * queue) {
	if (queue == NULL) return;

	void* ptr = NULL;

	while (queue_status(queue) != CQUEUE_EMPTY) {
		queue_pop_begin(queue, &ptr);
		if (ptr != NULL) free(ptr);
	}
	queue_destroy(queue);
}

static ret_t inf2post_impl(cqueue_t* infix_token_queue,
	                       cqueue_t* operator_queue,
	                       cqueue_t* postfix_token_queue) {
	token_t* token = NULL;

	// precondition that the whole expression is wrapped in parentheses
	queue_peek_begin(infix_token_queue, &token);
	assert(token->type == L_PARENTHESE);

	queue_peek_end(infix_token_queue, &token);
	assert(token->type == R_PARENTHESE);

	while (queue_status(infix_token_queue) != CQUEUE_EMPTY) {
		queue_pop_begin(infix_token_queue, &token);

		switch (token->type) {

		case OPERAND:
			queue_push_end(postfix_token_queue, token);
			break;

		case U_PLUS:
		case U_MINUS:
		case L_PARENTHESE:
			queue_push_end(operator_queue, token);
			break;

		case R_PARENTHESE:
		{
			token_t* temp = NULL;
			cqueue_ret_t ret;

			free(token); // free R_PARENTHESE token

			ret = queue_pop_end(operator_queue, &temp);
			assert(ret == CQUEUE_SUCCESS);

			while (temp->type != L_PARENTHESE) {
				queue_push_end(postfix_token_queue, temp);
				queue_pop_end(operator_queue, &temp);
			}

			free(temp);

			if (queue_status(operator_queue) == CQUEUE_EMPTY && queue_status(infix_token_queue) != CQUEUE_EMPTY)
				return INVALID_EXPRESSION; // if initial '(' is withdrawed, but final ')' is not, then parentheses mismatch occured
			break;
		}

		case B_MULTIPLY:
		case B_DIVIDE:
		case B_PLUS:
		case B_MINUS: {
			token_t* prev_token = NULL;
			queue_peek_end(operator_queue, &prev_token);

			while (type_priority(prev_token->type) >= type_priority(token->type)) {
				queue_pop_end(operator_queue, &prev_token);
				queue_push_end(postfix_token_queue, prev_token);
				queue_peek_end(operator_queue, &prev_token);
			}
			queue_push_end(operator_queue, token);
			break;
		}
		}
	}

	if (queue_status(operator_queue) != CQUEUE_EMPTY) {
		return INVALID_EXPRESSION;
	}

	return SUCCESS;
}

static ret_t inf2post(cqueue_t** infix_token_queue) {
	cqueue_t* operator_queue = NULL;
	cqueue_t* postfix_token_queue = NULL;
	ret_t returns = FAILURE;

	if ((operator_queue = queue_create(QUEUE_CAPACITY)) == NULL)
		goto label1;
	
	if ((postfix_token_queue = queue_create(QUEUE_CAPACITY)) == NULL) 
		goto label2;

	returns = inf2post_impl(*infix_token_queue, operator_queue, postfix_token_queue);

	swap_ptrs(infix_token_queue, &postfix_token_queue);

label2:
	free_n_destroy_queue(operator_queue);

label1:
	free_n_destroy_queue(postfix_token_queue);
	return returns;
}

static ret_t binary_calculate(double* value1, double* value2, tokenType operator) {
	if (value1 == NULL || value2 == NULL)
		return FAILURE;

	switch (operator) {
	case B_PLUS:
		*value1 = (*value1) + (*value2);
		break;
	case B_MINUS:
		*value1 = (*value1) - (*value2);
		break;
	case B_MULTIPLY:
		*value1 = (*value1) * (*value2);
		break;
	case B_DIVIDE:
		if (*value2 == 0)
			return DIVISION_BY_ZERO;
		*value1 = (*value1) / (*value2);
		break;
	}
	return SUCCESS;
}

static ret_t post_calc_impl(cqueue_t* postfix_queue, cqueue_t* intermediate_queue) {
	double* value1 = NULL;
	double* value2 = NULL;

	while (queue_status(postfix_queue) != CQUEUE_EMPTY) {
		token_t* token = NULL;
		queue_pop_begin(postfix_queue, &token);

		switch (token->type) {
		case OPERAND:
			if ((value1 = malloc(sizeof(*value1))) == NULL) {
				return FAILURE;
			}
			*value1 = token->value;
			queue_push_end(intermediate_queue, value1);
			free(token);
			break;

		case B_PLUS:
		case B_MINUS:
		case B_MULTIPLY:
		case B_DIVIDE:
			if (queue_pop_end(intermediate_queue, &value2) != CQUEUE_SUCCESS) {
				return INVALID_EXPRESSION;
			}
			if (queue_peek_end(intermediate_queue, &value1) != CQUEUE_SUCCESS) {
				free(value2);
				return INVALID_EXPRESSION;
			}
			if (binary_calculate(value1, value2, token->type) == DIVISION_BY_ZERO)
			{
				free(value2);
				return DIVISION_BY_ZERO;
			}
			free(value2);
			free(token);
			break;

		case U_PLUS:
		case U_MINUS:
			if (queue_peek_end(intermediate_queue, &value2) != CQUEUE_SUCCESS) {
				return INVALID_EXPRESSION;
			}

			if (token->type == U_MINUS)
				*value2 *= -1;
			free(token);
			break;
		}
	}
	return SUCCESS;
}

static ret_t post_calc(cqueue_t* postfix_queue, double* result) {
	cqueue_t* intermediate_queue = NULL;
	ret_t returns = SUCCESS;
	double* double_ptr = NULL;

	if ((intermediate_queue = queue_create(QUEUE_CAPACITY)) == NULL) {
		queue_destroy(intermediate_queue);
		return FAILURE;
	}

	returns = post_calc_impl(postfix_queue, intermediate_queue);
	
	if (returns == SUCCESS) {
		queue_pop_end(intermediate_queue, &double_ptr);
		*result = *double_ptr;
		free(double_ptr);
	}

	free_n_destroy_queue(intermediate_queue);
	return returns;
}

ret_t postfixCalculator(const char expression[], double* result) {
	if (expression == NULL || result == NULL)
		return INVALID_ARGUMENT;

	ret_t returns = SUCCESS;
	
	if (strlen(expression) >= MAX_STRING_LEN - 1)
		return INVALID_ARGUMENT;

	cqueue_t* queue = NULL;

	if ((queue = queue_create(QUEUE_CAPACITY)) == NULL) {
		return FAILURE;
	}

	returns = parser(expression, queue); // pushes tokens into queue

	if (returns != SUCCESS) {
		goto on_exit;
	}
	
	returns = inf2post(&queue); // arranges tokens into postfix sequence

	if (returns != SUCCESS) {
		goto on_exit;
	}

	returns = post_calc(queue, result); // calculates postfix and puts answer into result
	
	on_exit:
	free_n_destroy_queue(queue);
	return returns;
}