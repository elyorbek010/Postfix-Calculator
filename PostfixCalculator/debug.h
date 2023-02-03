#ifndef DEBUG_H_
#define DEBUG_H_

#if _DEBUG
#define debug_print(fmt, ...) \
    fprintf(stderr, fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...)
#endif

#endif
