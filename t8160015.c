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

    int rc;
    pthread_t threads[customers];
    pthread_mutex_init(&lock, NULL); // Initializing mutex
    pthread_cond_init(&tel_cond, NULL);


    for(long t = 0; t < customers; t++)
    {
        rc = pthread_create(&threads[t], NULL, call_center, (void *) &t); // Creating customer
        if (rc)
        {
            printf("ERROR code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        int sl = (rand() % T_RES_HIGH) + T_RES_LOW;
        sleep(sl); // After 1st customer every customer calls after
                   // [T_reslow, T_reshigh] seconds
    }
    
    for (int i = 0; i < customers; i++)
    {
        pthread_join(threads[i], NULL);
    }
    

    pthread_mutex_destroy(&lock); // Destroying mutex
    pthread_cond_destroy(&tel_cond);

    // Will start with custID -> pthreadID
    printf("Reservation was succesful. Your seats are in zone <a>, row <b>, number <c, d, ...> with cost <X> euros.\n");
    printf("Reservation failed because we didn't have the appropriate seats.\n");
    printf("Reservation failed because card payment was declined.\n");

    // At the end of the execution
    // Seat overview
    printf("Total revenue %d\n", main_cash);
    

    return 0;
}
