#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "t8160015.h"

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


int main(int argc, char const *argv[])
{
    // I need 2 mutexes
    // Available thlefonhths
    // Available Chasiers

    // Seat Array
    short seat_array[N_ZONE_SEATS][N_ZONE_A + N_ZONE_B];
    // At Least Array Mutex

    // Mutex for Bank account
    // Statistics for bank account
    int customers = atoi(argv[1]); // Converts a str type to an int.
                                   // atoi resides to stdlib
    unsigned int seed =  atoi(argv[2]);
    rand_r(&seed);
   
    return 0;
}
