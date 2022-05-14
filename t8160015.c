#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "t8160015.h"

static short seat_array[N_ZONE_A + N_ZONE_B][N_SEAT]; // Representing seat layout

int main(int argc, char const *argv[])
{
    // I need 2 mutexes
    // Available thlefonhths
    // Available Chasiers

    // Seat Array
    // short seat_array[N_ZONE_SEATS][N_ZONE_A + N_ZONE_B];
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
    pthread_mutex_init(&seat_array_lock, NULL);
    pthread_cond_init(&tel_cond, NULL);

    for(long t = 0; t < customers; t++)
    {
        getParameters *params = malloc(sizeof *params);
        params->custs = &customers;
        params->thread_id = &t;
        rc = pthread_create(&threads[t], NULL, call_center, params); // Creating customer
        if (rc)
        {
            printf("ERROR code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        int sl = (rand() % T_RES_HIGH) + T_RES_LOW;
        sleep(sl); // After 1st customer every customer calls after
                   // [T_reslow, T_reshigh] seconds
        
        free(params);
    }
    
    for (int i = 0; i < customers; i++)
    {
        pthread_join(threads[i], NULL);
    }
    

    pthread_mutex_destroy(&lock); // Destroying mutex
    pthread_mutex_destroy(&seat_array_lock);
    pthread_cond_destroy(&tel_cond);

    int n_seats;

    for(int i = 0; i < customers; i++)
    {
        switch (arrptr[i].code)
        {
        case 404:
            printf("Customer %d: Reservation failed because we didn't have the appropriate seats.\n", i);
            purchases404++;
            break;
        case 402:
            printf("Customer %d: Reservation failed because card payment was declined.\n", i);
            purchases402++;
            break;
        case 200:
            printf("Customer %d: Reservation was succesful. Your seats are in zone %c, row %d, number <", i, arrptr[i].ticket.zone, arrptr[i].ticket.seats[0].row);
            if(arrptr[i].ticket.zone == 'a')
            {
                n_seats =  arrptr[i].ticket.value / 30;
            } 
            else
            {
                n_seats =  arrptr[i].ticket.value / 20;
            }
            for(int j = 0; j < n_seats; j++)
            {
                printf("%d, ", arrptr[i].ticket.seats[j].pos);
            }
            printf("> with cost %d euros.\n", arrptr[i].ticket.value);
            purchases200++;
            break;
        default:
            break;
        }
    }
    // Will start with custID -> pthreadID
    // printf("Reservation was succesful. Your seats are in zone <a>, row <b>, number <c, d, ...> with cost <X> euros.\n");
    printf("%d of transactions was successful.", purchases200/customers);
    printf("%d of transactions failed, because proper seats weren't found.", purchases404/customers);
    printf("%d of transactions failed, as the card was declined.", purchases402/customers);
    

    // At the end of the execution
    // Seat overview
    printf("Total revenue %d\n", main_cash);
    

    return 0;
}
