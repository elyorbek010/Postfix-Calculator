extern "C" {
#include "calculator.h"
#include "cqueue.h"
}

#include "gtest/gtest.h"

TEST(emptyString, parser) {
	char string[] = "";
	cqueue_t* queue = parser(string);
	queue_destroy(queue);
}

