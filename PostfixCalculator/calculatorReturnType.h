#ifndef RETURN_TYPE_H_
#define RETURN_TYPE_H_

typedef enum CalculatorReturnCode {
	SUCCESS,
	FAILURE,
	INVALID_EXPRESSION,
	INVALID_ARGUMENT,
	DIVISION_BY_ZERO
} ret_t;

#endif