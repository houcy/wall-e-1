#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#ifdef DEBUG
#define debug(message, ...) \
    do { \
        fprintf(stderr, "Debug: " message " (" __FILE__ ":" \
            STRINGIZE(__LINE__) ", ", ##__VA_ARGS__); \
        fprintf(stderr, "%s)\n", __func__); \
    } while(0)
#else
#define debug(message, ...)
#endif

#define warning(message, ...) \
    do { \
        fprintf(stderr, "Warning: " message " (" __FILE__ ":" \
            STRINGIZE(__LINE__) ", ", ##__VA_ARGS__); \
        fprintf(stderr, "%s)\n", __func__); \
    } while(0)

#define critical(message, ...) \
    do { \
        fprintf(stderr, "Critical: " message " (" __FILE__ ":" \
            STRINGIZE(__LINE__) ", ", ##__VA_ARGS__); \
        fprintf(stderr, "%s)\n", __func__); \
    } while(0)

#define fatal(message, ...) \
    do { \
        fprintf(stderr, "Fatal: " message " (" __FILE__ ":" \
            STRINGIZE(__LINE__) ", ", ##__VA_ARGS__); \
        fprintf(stderr, "%s)\n", __func__); \
        exit(EXIT_FAILURE); \
    } while(0)

#endif // LOG_H
