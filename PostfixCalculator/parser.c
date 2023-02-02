#include "parser.h"
#include "ctype.h"
#include <string.h>

#define CREATE_TOKEN(token)						 \
		do {									 \
			token = malloc(sizeof (token));		 \
			CHECK_AND_RETURN_IF_NOT_EXIST(token);\
		} while(0)

static inline ret_t push_token(cqueue_t* token_queue, tokenType type, char character) {
	token_t* token = NULL;
	CREATE_TOKEN(token);

	token->type = type;
	token->operand = (type == OPERAND) ? (character - '0') : character;
	if (queue_push_end(token_queue, token) != CQUEUE_SUCCESS)
		return FAILURE;
	return SUCCESS;
}

static inline ret_t l_parenthese(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	if (queue_peek_end(token_queue, &token) == CQUEUE_UNDERFLOW) {
		returns = push_token(token_queue, L_PARENTHESE, '(');
	}
	else if (token->type == OPERAND || token->type == R_PARENTHESE) {
		if (push_token(token_queue, B_MULTIPLY, '*') == FAILURE)
			return FAILURE;
		returns = push_token(token_queue, L_PARENTHESE, '(');
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static inline ret_t r_parenthese(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND) {
		returns = push_token(token_queue, R_PARENTHESE, ')');
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static inline ret_t plus(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = push_token(token_queue, B_PLUS, '+');
	}
	else {
		returns = push_token(token_queue, U_PLUS, '+');
	}

	return returns;
}

static inline ret_t minus(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = push_token(token_queue, B_MINUS, '-');
	}
	else {
		returns = push_token(token_queue, U_MINUS, '-');
	}

	return returns;
}

static inline ret_t multiply(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = push_token(token_queue, B_MULTIPLY, '*');
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static inline ret_t divide(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = push_token(token_queue, B_DIVIDE, '/');
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static inline ret_t digit(cqueue_t* token_queue, char digit, bool prev_is_space) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND) {
		if (prev_is_space) {
			returns = INVALID_EXPRESSION;
		}
		else {
			token->operand *= 10;
			token->operand += digit - '0';
		}
	}
	else if (token->type == R_PARENTHESE) {
		if (push_token(token_queue, B_MULTIPLY, '*') == FAILURE)
			return FAILURE;
		returns = push_token(token_queue, OPERAND, digit);
	}
	else {
		returns = push_token(token_queue, OPERAND, digit);
	}

	return returns;
}

ret_t parser(const char expression[], cqueue_t* token_queue) {
	size_t exp_length = strlen(expression);
	bool prev_is_space = false;
	token_t* token = NULL;
	size_t idx = 0;
	ret_t returns = SUCCESS;

	returns = l_parenthese(token_queue); // add initial '('
	if (returns != SUCCESS) {
		return returns;
	}

	while (idx++ < exp_length) {
		if (isdigit(expression[idx])) {
			returns = digit(token_queue, expression[idx], prev_is_space);
			prev_is_space = false;
		}
		else if (isspace(expression[idx])) {
			prev_is_space = true;
			returns = SUCCESS;
		}
		else {
			switch (expression[idx]) {
			case '(': returns = l_parenthese(token_queue);
				break;
			case ')': returns = r_parenthese(token_queue);
				break;
			case '+': returns = plus(token_queue);
				break;
			case '-': returns = minus(token_queue);
				break;
			case '*': returns = multiply(token_queue);
				break;
			case '/': returns = divide(token_queue);
				break;
			default:
				returns = INVALID_EXPRESSION;
			}
		}
		if (returns != SUCCESS)
			return returns;
	}

	returns = r_parenthese(token_queue); // add final ')'

	return returns;
}