#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

void* second_body(void* arg)
{
    int number = *(int*)arg;
    printf("%d\n", number);
    return NULL;
}

void* first_body(void* arg)
{
    int number = *(int*)arg;
    pthread_t thread;

    printf("%d\n", number);
    number++;
    pthread_create(&thread, NULL, second_body, &number);
    pthread_join(thread, NULL);

    return NULL;
}

int main()
{
    pthread_t thread;

    for (int i = 1; i < N; i += 2)
    {
        pthread_create(&thread, NULL, first_body, &i);
        pthread_join(thread, NULL);
    }

    return EXIT_SUCCESS;
}
