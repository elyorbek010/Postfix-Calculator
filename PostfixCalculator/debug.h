#ifndef DEBUG_H_
#define DEBUG_H_

#if _DEBUG
#define debug_print(fmt, ...) \
    fprintf(stderr, fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...)
#endif

#define CHECK_AND_RETURN_IF_NOT_EXIST(ptr)				\
		do{												\
			if ((ptr) == NULL) {                        \
				debug_print("object does not exist\n"); \
				return FAILURE;						    \
			}											\
		} while (0)

#endif
