#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "event.h"
#include "queue.h"

#define TRUE 1
#define FALSE 0

#define USER_COUNT 2
#define STARTING_BALANCE 1000
#define AMOUNT_COEFFICIENT (STARTING_BALANCE / 2)

typedef enum
{
    opLockAccount,
    opUnlockAccount,
    opDeposit,
    opWithdraw,
} account_operation_t;

typedef struct
{
    int id;
    account_operation_t operation;
    int transaction_amount;
    event_t operation_requested;
    event_t operation_completed;
    event_t client_ready;
} request_t;

static queue_t transaction_queue;
static pthread_mutex_t transaction_lock = PTHREAD_MUTEX_INITIALIZER;
static event_t transaction_requested, transaction_completed;

void* client(void* arg)
{
    request_t* request = arg;
    event_send(&request->client_ready);
    while (TRUE)
    {
        event_wait(&request->operation_requested);
        switch (request->operation)
        {
            case opLockAccount:
            {
                pthread_mutex_lock(&transaction_lock);
                break;
            }
            case opUnlockAccount:
            {
                pthread_mutex_unlock(&transaction_lock);
                break;
            }
            case opDeposit:
            case opWithdraw:
            {
                push(&transaction_queue, request);
                event_send(&transaction_requested);
                event_wait(&transaction_completed);
                break;
            }
        }
        event_send(&request->operation_completed);
    }
    return NULL;
}

void perform_operation(request_t* request, account_operation_t op)
{
    request->operation = op;
    event_send(&request->operation_requested);
    event_wait(&request->operation_completed);
}

void* user(void* arg)
{
    request_t* request = arg;
    while (TRUE)
    {
        perform_operation(request, opLockAccount);
        request->transaction_amount = 1 + rand() % AMOUNT_COEFFICIENT;
        perform_operation(request, ((rand() % 2) ? opDeposit: opWithdraw));
        perform_operation(request, opUnlockAccount);
        sleep(rand() % USER_COUNT);
    }
    return NULL;
}

char* transaction2string(account_operation_t op)
{
    if (op == opDeposit)
    {
        return "Deposit";
    }
    return "Withdraw";
}

void* server(void *arg)
{
    int account = STARTING_BALANCE;
    int sufficient_funds;
    request_t* request;
    event_t* server_ready = arg;

    printf("Account balance is %d\n", account);

    event_init(&transaction_requested);
    event_init(&transaction_completed);
    pthread_mutex_init(&transaction_queue.lock, NULL);

    event_send(server_ready);
    while (TRUE)
    {
        event_wait(&transaction_requested);
        request = pop(&transaction_queue);

        switch (request->operation)
        {
            case opDeposit:
            {
                sufficient_funds = TRUE;
                account += request->transaction_amount;
                break;
            }
            case opWithdraw:
            {
                sufficient_funds = account >= request->transaction_amount;
                if (sufficient_funds)
                {
                    account -= request->transaction_amount;
                }
                break;
            }
            default:
            {
                perror("Unknown operation!\n");
                exit(EXIT_FAILURE);
            }
        }

        if (sufficient_funds)
        {
            printf("User %d: %s %d [Balance: %d]\n",
                request->id + 1,
                transaction2string(request->operation),
                request->transaction_amount,
                account);
        }

        event_send(&transaction_completed);
    }
}

void init_rand()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);
}

int main()
{
    request_t requests[USER_COUNT];
    pthread_t users[USER_COUNT];
    pthread_t clients[USER_COUNT];
    pthread_t server_thread;
    event_t server_ready;

    init_rand();

    event_init(&server_ready);
    pthread_create(&server_thread, NULL, server, &server_ready);
    event_wait(&server_ready);

    for (int i = 0; i < USER_COUNT; ++i)
    {
        requests[i].id = i;
        event_init(&requests[i].client_ready);
        event_init(&requests[i].operation_requested);
        event_init(&requests[i].operation_completed);
        pthread_create(&clients[i], NULL, client, &requests[i]);
        event_wait(&requests[i].client_ready);
        pthread_create(&users[i], NULL, user, &requests[i]);
    }

    pthread_join(server_thread, NULL);

    return EXIT_SUCCESS;
}
