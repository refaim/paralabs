#ifndef _PARALABS_H_
#define _PARALABS_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

void check_pthread(int retcode, char* file, int line)
{
    if (retcode != 0)
    {
        fprintf(stderr, "pthread error on %s:%d", file, line);
        exit(retcode);
    }
}

#define PCheck(call) check_pthread(call, __FILE__, __LINE__)

#endif
