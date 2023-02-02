// PostfixCalculator.c : Defines the functions for the static library.
//
#include "calculator.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define QUEUE_CAPACITY (2 * STRING_LEN)

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
