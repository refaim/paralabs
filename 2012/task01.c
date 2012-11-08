#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define USER_COUNT 2
#define STARTING_BALANCE 1000
#define AMOUNT_COEFFICIENT (STARTING_BALANCE / 2)

typedef enum
{
    opDeposit,
    opWithdraw,
    opAccountOpTypeCount,
} account_operation_t;

typedef struct
{
    int id;
    account_operation_t operation;
    int operation_amount;
    pthread_cond_t operation_requested;
    pthread_cond_t client_ready;
} request_t;

static pthread_mutex_t account_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t account_access_requested = PTHREAD_COND_INITIALIZER;
static int active_client_id;

void init_rand()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);
}

char* op2string(account_operation_t op)
{
    if (op == opDeposit)
    {
        return "Deposit";
    }
    return "Withdraw";
}

void wait_until(pthread_cond_t* condition)
{
    pthread_mutex_t dummy_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&dummy_mutex);
    pthread_cond_wait(condition, &dummy_mutex);
    pthread_mutex_unlock(&dummy_mutex);
}

void* client(void* arg)
{
    request_t* request = arg;

    pthread_cond_signal(&request->client_ready);

    while (1)
    {
        wait_until(&request->operation_requested);
        pthread_mutex_lock(&account_lock);
        active_client_id = request->id;
        pthread_cond_signal(&account_access_requested);
        pthread_mutex_unlock(&account_lock);
    }

    return NULL;
}

void* user(void* arg)
{
    request_t* request = arg;
    pthread_t client_thread;

    pthread_cond_init(&request->operation_requested, NULL);
    pthread_cond_init(&request->client_ready, NULL);

    pthread_create(&client_thread, NULL, client, request);
    wait_until(&request->client_ready);

    while (1)
    {
        request->operation = rand() % opAccountOpTypeCount;
        request->operation_amount = 1 + rand() % AMOUNT_COEFFICIENT;
        pthread_cond_signal(&request->operation_requested);
        sleep(rand() % USER_COUNT);
    }

    return NULL;
}

int main()
{
    int account = STARTING_BALANCE;
    pthread_t users[USER_COUNT];
    request_t requests[USER_COUNT];
    request_t current_request;

    printf("Account balance is %d\n", account);

    init_rand();

    for (int i = 0; i < USER_COUNT; ++i)
    {
        requests[i].id = i;
        pthread_create(&users[i], NULL, user, &requests[i]);
    }

    while (1)
    {
        wait_until(&account_access_requested);
        current_request = requests[active_client_id];

        if (current_request.operation == opDeposit)
        {
            account += current_request.operation_amount;
        }
        else
        {
            account -= current_request.operation_amount;
        }

        printf("User %d: %s %d [Balance: %d]\n",
            current_request.id + 1,
            op2string(current_request.operation),
            current_request.operation_amount,
            account);
    }

    return 0;
}
