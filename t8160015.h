#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Header for stree

// Constant variables
#define N_TEL 3 // Thlefonhtes
#define N_CASH 2 // Cashiers
#define N_SEAT 10 
#define N_ZONE_SEATS 10
#define N_ZONE_A 10
#define N_ZONE_B 20
#define P_ZONE_A 0.3 // Probability to choose zone a
#define C_ZONE_A 30 // Cost of zone A
#define C_ZONE_B 20 // Cost of zone A
#define N_SEAT_LOW 1 
#define N_SEAT_HIGH 5

// All Ts need srand
#define T_RES_LOW 1
#define T_RES_HIGH 5
#define T_SEAT_LOW 5
#define T_SEAT_HIGH 13
#define T_CASH_LOW 4
#define T_CASH_HIGH 8
#define P_CARD_SUCCESS 0.9

pthread_mutex_t lock; // Creating mutex
pthread_cond_t tel_cond = PTHREAD_COND_INITIALIZER;

static int main_cash = 0;
static int total_purchases = 0;
static int purchases200 = 0;
static int purchases404 = 0;
static int purchases400 = 0;

int busy_tel = 3;

void bank_account()
{
    printf("Hello from bank account\n");
}

void *call_center(void *threadid)
{
    long t_cust_id = *((long *) (threadid)); // Customer's thread id -> t_thread_id
    int rc;

    rc = pthread_mutex_lock(&lock);
    while(busy_tel == 0)
    {
        rc = pthread_cond_wait(&tel_cond, &lock);
    }

    busy_tel--;

    rc = pthread_mutex_unlock(&lock);
    sleep(5);
    rc = pthread_mutex_lock(&lock);

    busy_tel++;

    
    rc = pthread_cond_signal(&tel_cond);
    rc = pthread_mutex_unlock(&lock); // Should exist before any return statements
    pthread_exit(NULL);
}

void cashier()
{
    printf("Hello from the cashiers\n");
}