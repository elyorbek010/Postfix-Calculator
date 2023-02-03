#ifndef PARSER_H_
#define PARSER_H_

#include "calculatorReturnType.h"
#include "cqueue.h"

typedef enum TokenType {
	OPERAND,
	L_PARENTHESE,
	R_PARENTHESE,
	B_PLUS,
	B_MINUS,
	B_MULTIPLY,
	B_DIVIDE,
	U_PLUS,
	U_MINUS
} tokenType;

typedef struct Token {
	int value;
	tokenType type;
}token_t;

ret_t parser(const char expression[], cqueue_t* token_queue);

#endif // PARSER_H_