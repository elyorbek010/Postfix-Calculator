#ifndef PARSER_H_
#define PARSER_H_

#include "returnType.h"
#include "cqueue.h"
#include "debug.h"

typedef enum TokenType {
	OPERAND,
	B_PLUS,
	B_MINUS,
	B_MULTIPLY,
	B_DIVIDE,
	U_PLUS,
	U_MINUS,
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

ret_t parser(const char expression[], cqueue_t* token_queue);

#endif // PARSER_H_