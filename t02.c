#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <pp.h>

#define N 120

int gCounter = 0;
int gOther = 0;
int gbPrinted = 1;

void* first_body(void* arg)
{
    for (int i = 2; i <= N; i += 2)
    {
        while ((gbPrinted == 0) || (gCounter % 2 == 0))
        {
            sleep(1);
        }
        gbPrinted = 0;
        gCounter = i;
    }
}

void* second_body(void *arg)
{
    for (int i = 1; i <= N + 1; i += 2)
    {
        while ((gbPrinted == 0) || (gCounter % 2 != 0))
        {
            sleep(1);
        }
        gbPrinted = 0;
        gCounter = i;
    }
}

void* third_body(void* arg)
{
    while (gCounter <= N+1)
    {
        if ((gCounter != 0) && (gOther != gCounter))
        {
            gOther = gCounter;
            printf("%d\n", gOther);
            gbPrinted = 1;
        }
    }
}

int main()
{
    pthread_t first, second, third;

    PTCALL(pthread_create(&first, NULL, first_body, NULL))
    PTCALL(pthread_create(&second, NULL, second_body, NULL))
    PTCALL(pthread_create(&third, NULL, third_body, NULL))
    PTCALL(pthread_join(first, NULL))
    PTCALL(pthread_join(second, NULL))
    PTCALL(pthread_join(third, NULL))
}