#include <string.h>
#include "parser.h"
#include "ctype.h"
#include "debug.h"

#define ASCII_TO_DECIMAL(character) (character - '0')

static ret_t create_push_token(cqueue_t* token_queue, token_t token) {
	token_t* token_ptr = NULL;

	token_ptr = malloc(sizeof(*token_ptr));

	if (token_ptr == NULL) {
		return FAILURE;
	}
	*token_ptr = token; // copy contents of auto storage token into dynamic storage token

	if (queue_push_end(token_queue, token_ptr) != CQUEUE_SUCCESS) {
		free(token_ptr);
		return FAILURE;
	}
	return SUCCESS;
}

static ret_t l_parenthese(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	if (queue_peek_end(token_queue, &token) == CQUEUE_UNDERFLOW) {
		returns = create_push_token(token_queue, (token_t) {.type = L_PARENTHESE});
	}
	else if (token->type == OPERAND || token->type == R_PARENTHESE) {
		if (create_push_token(token_queue, (token_t) { .type = B_MULTIPLY }) == FAILURE) {
			return FAILURE;
		}
		returns = create_push_token(token_queue, (token_t) { .type = L_PARENTHESE });
	}
	else {
		returns = create_push_token(token_queue, (token_t) { .type = L_PARENTHESE });
	}

	return returns;
}

static ret_t r_parenthese(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND) {
		returns = create_push_token(token_queue, (token_t) { .type = R_PARENTHESE });
	}
	else if(token->type == R_PARENTHESE){
		returns = create_push_token(token_queue, (token_t) { .type = R_PARENTHESE });
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static ret_t plus(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = create_push_token(token_queue, (token_t) { .type = B_PLUS });
	}
	else {
		returns = create_push_token(token_queue, (token_t) { .type = U_PLUS });
	}

	return returns;
}

static ret_t minus(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);
	
	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = create_push_token(token_queue, (token_t) { .type = B_MINUS });
	}
	else {
		returns = create_push_token(token_queue, (token_t) { .type = U_MINUS });
	}

	return returns;
}

static ret_t multiply(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = create_push_token(token_queue, (token_t) { .type = B_MULTIPLY });
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static ret_t divide(cqueue_t* token_queue) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND || token->type == R_PARENTHESE) {
		returns = create_push_token(token_queue, (token_t) { .type = B_DIVIDE });
	}
	else {
		returns = INVALID_EXPRESSION;
	}

	return returns;
}

static ret_t digit(cqueue_t* token_queue, char digit, bool prev_is_space) {
	token_t* token = NULL;
	ret_t returns = SUCCESS;

	queue_peek_end(token_queue, &token);

	if (token->type == OPERAND) {
		if (prev_is_space) {
			returns = INVALID_EXPRESSION;
		}
		else {
			token->value *= 10;
			token->value += ASCII_TO_DECIMAL(digit);
		}
	}
	else if (token->type == R_PARENTHESE) {
		if (create_push_token(token_queue, (token_t) { .type = B_MULTIPLY }) == FAILURE)
			return FAILURE;
		returns = create_push_token(token_queue, (token_t) { .type = OPERAND, .value = ASCII_TO_DECIMAL(digit) });
	}
	else {
		returns = create_push_token(token_queue, (token_t) { .type = OPERAND, .value = ASCII_TO_DECIMAL(digit) });
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

	while (idx < exp_length) {
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
		idx++;
	}

	returns = r_parenthese(token_queue); // add final ')'

	return returns;
}