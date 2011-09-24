#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define PThreadCallEx(call, retcode) \
    { \
        int result; \
        result = call; \
        if (result != 0) \
        { \
            fprintf(stderr, "pthread error on %s:%d", __FILE__, __LINE__); \
            return retcode; \
        } \
    }

#define PThreadCall(call) PThreadCallEx(call, EXIT_FAILURE)
