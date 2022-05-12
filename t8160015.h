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
pthread_mutex_t cashier_lock; // Creating cashier lock
pthread_cond_t tel_cond = PTHREAD_COND_INITIALIZER; // telehone operator condition
pthread_cond_t seat_array_cond = PTHREAD_COND_INITIALIZER; // Seat array condition
pthread_cond_t cashier_cond = PTHREAD_COND_INITIALIZER; // Cashier condition

static short seat_array[N_ZONE_A + N_ZONE_B][N_SEAT]; // Representing seat layout
// 0-9 ZONE_A (premium)
// 10-29 ZONE_B (basic)

static int main_cash = 0;
static int total_purchases = 0;
static int purchases200 = 0;
static int purchases404 = 0;
static int purchases400 = 0;

int busy_tel = 3;
int check_seat_array = 1;
int cash = 1;
int only_once = 1;

struct Seat {
    int row;
    int pos;
};

// Struct holding reservation details
struct Ticket {
        char zone;
        struct Seat seats[N_SEAT_HIGH];
        int value;
};

// LinkedList holding Messages
struct Message {
    int code;
    struct Ticket ticket;
};

// Get parameters
typedef struct {
    int *custs;
    long *thread_id;

} getParameters;

// Function declarations
void bank_account();
void *call_center(void  *params);
void cashier();
int check_for_seat(char zone, int num);
int *init_array(int n_custs);

int  *init_array(int n_custs)
{
    int *arr = malloc(n_custs * sizeof(int));
    return arr;
}

void bank_account()
{
    printf("Hello from bank account\n");
}

void *call_center(void *params)
{
    getParameters *actual_params = params;
    int custs = *actual_params->custs;
    long t_cust_id = *((long *) (actual_params->thread_id)); // Customer's thread id -> t_thread_id
    int rc;

    if(only_once == 1){
        only_once--;
        int *arrptr;
        arrptr = init_array(custs);
    }
    
    struct Message msg;

    rc = pthread_mutex_lock(&lock);
    while(busy_tel == 0)
    {
        rc = pthread_cond_wait(&tel_cond, &lock);
    }
    printf("Serving customer %ld\n", t_cust_id);

    busy_tel--; // Start Processs
    rc = pthread_mutex_unlock(&lock);

    float p_seat = (float) rand() / RAND_MAX; // Generating nums between 0 and 1
    // If p_seat <= 0.3 -> ZONE_A else ZONE_B
    printf("%f\n", p_seat);

    int n_seats = (rand() % N_SEAT_HIGH ) + N_SEAT_LOW;

    int return_code;
    char send_zone;
    
    if(p_seat < P_ZONE_A)
    {
        send_zone = 'a';
        return_code = check_for_seat(send_zone, n_seats);
    } else {
        send_zone = 'b';
        return_code = check_for_seat(send_zone, n_seats);
    }
    
    if(return_code == 404)
    {
        msg.code = return_code;
    }

    busy_tel++; // End Process
    rc = pthread_cond_signal(&tel_cond);
    rc = pthread_mutex_unlock(&lock); // Should exist before any return statements
    pthread_exit(NULL);
}

void cashier()
{
    printf("Hello from the cashiers\n");
}

int check_for_seat(char zone, int num)
{
    int rc;
    rc = pthread_mutex_lock(&seat_array_lock);

    while(check_seat_array == 0)
    {
        rc = pthread_cond_wait(&seat_array_cond, &seat_array_lock);
    }
    check_seat_array--; // Start Process
    rc = pthread_mutex_unlock(&seat_array_lock);
    
    int start_row, end_row;
    switch (zone)
    {
    case 'a':
        start_row = 0;
        end_row = N_ZONE_A;
        break;
    case 'b':
        start_row = N_ZONE_A;
        end_row = N_ZONE_A + N_ZONE_B;
    default:
        break;
    }
    struct Ticket tck;
    
    short flag = 0;
    int counter = 0;
    for(start_row; start_row < end_row; start_row++ )
    {
        /*
        * 3 conditions:
        *   i <= N_SEAT -> Checked all seats in a row
        *   i + num - counter <= N_SEAT -> If seats are enough
        *   counter < num -> Reserve no more than the seats that has been requested
        */
        for (int i = 0; (i <= N_SEAT) && (i + num - counter <= N_SEAT) && (counter < num); i++)
        {
            if(seat_array[start_row][i] == 0)
            {
                seat_array[start_row][i] = 1;
                tck.seats->row = start_row;
                tck.seats->pos = i;
                tck.zone = zone;
                counter++;
                flag = 1;
            }
        }
        
        if(flag == 1)
        {
            break;
        }
    }

    check_seat_array++; // End Process
    rc = pthread_cond_signal(&seat_array_cond);
    rc = pthread_mutex_unlock(&seat_array_lock);

    if(flag == 1)
    {
        return 200;
    }
    return 404;
}