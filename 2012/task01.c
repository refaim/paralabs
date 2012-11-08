#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define USER_COUNT 2

typedef enum
{
    opDeposit,
    opWithdraw,
    opAccountOpTypeCount,
} account_operation_t;

typedef struct
{
    pthread_mutex_t lock;
} account_t;

void* client(void* arg)
{
    account_t* account = arg;

    pthread_mutex_lock(&account->lock);
    // do stuff
    pthread_mutex_unlock(&account->lock);

    return NULL;
}

void* user(void* arg)
{
    int operation, amount;
    pthread_t client_thread;

    pthread_create(&client_thread, NULL, client, arg);
    while (1)
    {
        operation = rand() % opAccountOpTypeCount;
        amount = 1 + rand();
        switch (operation)
        {
            case opDeposit:
            {
                break;
            }
            case opWithdraw:
            {
                break;
            }
        }
        // Sleep for 1-2 seconds.
        sleep(1 + rand() % 2);
    }
    
    return NULL;
}

void init_rand()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);
}

int main()
{
    pthread_t users[USER_COUNT];
    account_t account;
    int money;

    init_rand();
    money = RAND_MAX / 2 + rand();

    pthread_mutex_init(&account.lock, NULL);

    for (int i = 0; i < USER_COUNT; ++i)
    {
        pthread_create(&users[i], NULL, user, &account_t);
    }

    while (1)
    {

    }

    return 0;
}