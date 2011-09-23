#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define PTCALLEX(call, retcode) \
    { \
        int result; \
        result = call; \
        if (result != 0) \
        { \
            fprintf(stderr, "pthread error on %s:%d", __FILE__, __LINE__); \
            return retcode; \
        } \
    }

#define PTCALL(call) PTCALLEX(call, EXIT_FAILURE)

