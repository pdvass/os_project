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

pthread_mutex_t lock; // Creating call center mutex
pthread_mutex_t seat_array_lock; // Creating seat array mutex
pthread_cond_t tel_cond = PTHREAD_COND_INITIALIZER; // telehone operator condition
pthread_cond_t seat_array_cond = PTHREAD_COND_INITIALIZER; // Seat array condition

short seat_array[N_ZONE_A + N_ZONE_B][N_SEAT]; // Representing seat layout
// 0-9 ZONE_A (premium)
// 10-29 ZONE_B (basic)

static int main_cash = 0;
static int total_purchases = 0;
static int purchases200 = 0;
static int purchases404 = 0;
static int purchases400 = 0;

int busy_tel = 3;
int check_seat_array = 1;

// Function declarations
void bank_account();
void *call_center(void *threadid);
void cashier();
void check_for_seat();


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
    printf("Serving customer %ld\n", t_cust_id);
    float p_seat = (float) rand() / RAND_MAX; // Generating nums between 0 and 1
    // If p_seat <= 0.3 -> ZONE_A else ZONE_B
    printf("%f\n", p_seat);

    busy_tel--;

    rc = pthread_mutex_unlock(&lock);
    check_for_seat();
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

void check_for_seat()
{
    int rc;
    rc = pthread_mutex_lock(&seat_array_lock);
    while(check_seat_array == 0)
    {
        rc = pthread_cond_wait(&seat_array_cond, &seat_array_lock);
    }

    printf("Hi from check for seat \n");
    seat_array[0][0]++;

    rc = pthread_cond_signal(&seat_array_cond);
    rc = pthread_mutex_unlock(&seat_array_lock);
}