// postfixCalculator.c : Defines the functions for the static library.
//
#include "calculator.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define QUEUE_CAPACITY (2 * STRING_LEN)

#if _DEBUG
#define debug_print(fmt, ...) \
    fprintf(stderr, fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...)
#endif

#define CHECK_AND_RETURN_IF_NOT_EXIST(ptr)     \
    if (ptr == NULL)                           \
    {                                          \
        debug_print("object does not exist\n");\
        return FAILURE;						   \
    }


#define CREATE_TOKEN(token)			 \
{									 \
token = malloc(sizeof token);		 \
CHECK_AND_RETURN_IF_NOT_EXIST(token);\
}								

typedef enum TokenType {
	OPERAND,
	U_PLUS,
	U_MINUS,
	B_PLUS,
	B_MINUS,
	B_MULTIPLY,
	B_DIVIDE,
	L_PARENTHESE,
	R_PARENTHESE
} tokenType;

typedef struct Token {
	union {
		char operator;
		int operand;
	};
	tokenType type;
}token_t;

ret_t push_token(cqueue_t* token_queue, tokenType type, char character) {
	token_t* token;
	CREATE_TOKEN(token);

	token->type = type;
	token->operand = (type == OPERAND) ? (character - '0') : character;
	if (queue_push_end(token_queue, token) != CQUEUE_SUCCESS)
		return FAILURE;
	return SUCCESS;
}

static inline ret_t l_parenthese(cqueue_t* token_queue) {
	token_t* token = NULL;

	if (queue_peek_end(token_queue, &token) == CQUEUE_UNDERFLOW) {
		if (push_token(token_queue, L_PARENTHESE, '(') == FAILURE)
			return FAILURE;
	}
	else if(token->type == OPERAND || token->type == R_PARENTHESE) {
		if (push_token(token_queue, B_MULTIPLY, '*') == FAILURE)
			return FAILURE;
		if (push_token(token_queue, L_PARENTHESE, '(') == FAILURE)
			return FAILURE;
	}
	else {
		return INVALID_EXPRESSION;
	}

	return SUCCESS;
}

static inline ret_t r_parenthese(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND) {
		returns = push_token(token_queue, R_PARENTHESE, ')');
	}
	else {
		returns =  FAILURE;
	}

	return returns;
}

static inline void plus(cqueue_t* queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = push_token(queue, B_PLUS, '+');
	}
	else {
		returns = push_token(queue, U_PLUS, '+');
	}

	return returns;
}

static inline ret_t parser(const char expression[], cqueue_t* token_queue) {
	//
}

static inline ret_t inf2post(cqueue_t* inf_token_queue) {
	//
}

static inline ret_t post_calc(cqueue_t* postfix_queue, double* result) {
	//
}

ret_t postfixCalculator(char expression[], double* result) {
	ret_t returns = SUCCESS;

	if (strlen(expression) >= STRING_LEN - 1)
		return INVALID_EXPRESSION;

	cqueue_t* queue = NULL;
	
	if (queue = queue_create(2 * STRING_LEN))
		return FAILURE;

	returns = parser(expression, queue); // parser pushes tokens into queue

	if (returns != SUCCESS) {
		return returns;
	}

	returns = inf2post(queue); // inf2post arranges tokens into postfix sequence

	if (returns != SUCCESS) {
		return returns;
	}

	returns = post_calc(queue, result); // calculates postfix and puts answer into result

	return returns;
}
