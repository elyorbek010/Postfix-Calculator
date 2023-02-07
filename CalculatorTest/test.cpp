extern "C" {
#include "calculator.h"
#include "cqueue.h"
}

#include "gtest/gtest.h"

TEST(Valid, justNumber) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("2", &a), SUCCESS);
	EXPECT_EQ(a, 2);
}

TEST(Valid, justNumberInParentheses) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("(2)", &a), SUCCESS);
	EXPECT_EQ(a, 2);
}

TEST(Valid, binaryOperators) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("2+2", &a), SUCCESS);
	EXPECT_EQ(a, 4);

	EXPECT_EQ(postfixCalculator("2-2", &a), SUCCESS);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("2*2", &a), SUCCESS);
	EXPECT_EQ(a, 4);

	EXPECT_EQ(postfixCalculator("2/2", &a), SUCCESS);
	EXPECT_EQ(a, 1);
}

TEST(Valid, parentheses) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("(1+2)", &a), SUCCESS);
	EXPECT_EQ(a, 3);

	EXPECT_EQ(postfixCalculator("1+2*(3)", &a), SUCCESS);
	EXPECT_EQ(a, 7);

	EXPECT_EQ(postfixCalculator("1+2/(4-3)", &a), SUCCESS);
	EXPECT_EQ(a, 3);

	EXPECT_EQ(postfixCalculator("1+2+(1+2)", &a), SUCCESS);
	EXPECT_EQ(a, 6);

	EXPECT_EQ(postfixCalculator("1+2-(1+2)", &a), SUCCESS);
	EXPECT_EQ(a, 0);
}
TEST(Valid, leadingZero) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("01 + 02", &a), SUCCESS);
	EXPECT_EQ(a, 3);

	EXPECT_EQ(postfixCalculator("03 / 002", &a), SUCCESS);
	EXPECT_EQ(a, 1.5);
}

TEST(Valid, divideByZero) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("2/0", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("2/(3-4+1)", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);
}

TEST(Valid, orderOfOperations) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("1+2*(2/4)-5*(6-5)", &a), SUCCESS);
	EXPECT_EQ(a, -3);
}

TEST(Valid, implicitMultiplication) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("2(2)", &a), SUCCESS);
	EXPECT_EQ(a, 4);

	EXPECT_EQ(postfixCalculator("(2)(2)", &a), SUCCESS);
	EXPECT_EQ(a, 4);

	EXPECT_EQ(postfixCalculator("(2)2", &a), SUCCESS);
	EXPECT_EQ(a, 4);

	EXPECT_EQ(postfixCalculator("2(2)(2)(2)2", &a), SUCCESS);
	EXPECT_EQ(a, 32);
}

TEST(Valid, signTest) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("+1", &a), SUCCESS);
	EXPECT_EQ(a, 1);

	EXPECT_EQ(postfixCalculator("-1", &a), SUCCESS);
	EXPECT_EQ(a, -1);

	EXPECT_EQ(postfixCalculator("1+(-2)", &a), SUCCESS);
	EXPECT_EQ(a, -1);

	EXPECT_EQ(postfixCalculator("1+-2", &a), SUCCESS);
	EXPECT_EQ(a, -1);

	EXPECT_EQ(postfixCalculator("1+-(-2)", &a), SUCCESS);
	EXPECT_EQ(a, 3);

	EXPECT_EQ(postfixCalculator("1+(+(-(1)))", &a), SUCCESS);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("-1*-1", &a), SUCCESS);
	EXPECT_EQ(a, 1);

	EXPECT_EQ(postfixCalculator("-1/+1", &a), SUCCESS);
	EXPECT_EQ(a, -1);

	EXPECT_EQ(postfixCalculator("+1/-1", &a), SUCCESS);
	EXPECT_EQ(a, -1);

	EXPECT_EQ(postfixCalculator("1----2", &a), SUCCESS);
	EXPECT_EQ(a, 3);
}

TEST(Valid, space) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("1+ -1", &a), SUCCESS);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("( 1)", &a), SUCCESS);
	EXPECT_EQ(a, 1);

	EXPECT_EQ(postfixCalculator("1 + 2", &a), SUCCESS);
	EXPECT_EQ(a, 3);

	EXPECT_EQ(postfixCalculator("( 1)/ (1 )", &a), SUCCESS);
	EXPECT_EQ(a, 1);
}

TEST(Invalid, emptyParentheses) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("()", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);
}

TEST(Invalid, space) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("( )", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("1 2 + 3", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("(1 2) * 3", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);
}

TEST(Invalid, operatorMismatch) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("*3", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("(*(3))", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("1 ** 2", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("1 */ 2", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("(1 + (2)", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("1+", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("1*", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator(" ", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("+", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);
}

TEST(Invalid, invalidOperators) {
	double a = 0;

	EXPECT_EQ(postfixCalculator("1%2", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("1&3", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);

	EXPECT_EQ(postfixCalculator("$2", &a), INVALID_EXPRESSION);
	EXPECT_EQ(a, 0);
}