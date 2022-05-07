#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "t8160015.h"


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
