#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // Header for clock_gettime
#include "t8160015.h"

static short seat_array[N_ZONE_A + N_ZONE_B][N_SEAT]; // Representing seat layout
struct timespec start, stop;

int main(int argc, char const *argv[])
{
    int customers = atoi(argv[1]); // Converts a str type to an int.
                                   // atoi resides to stdlib
    unsigned int seed =  atoi(argv[2]);
    rand_r(&seed);

    int rc;
    pthread_t threads[customers];
    pthread_mutex_init(&lock, NULL); // Initializing mutex
    pthread_mutex_init(&seat_array_lock, NULL);
    pthread_cond_init(&tel_cond, NULL);

    if(clock_gettime(CLOCK_REALTIME, &start) == -1)
    {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

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
    
    if(clock_gettime(CLOCK_REALTIME, &stop) == -1)
    {
        perror("clock gettime");
        exit(EXIT_FAILURE);
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
            printf("\b\b> with cost %d euros.\n", arrptr[i].ticket.value);
            purchases200++;
            break;
        default:
            break;
        }
    }

    printf("%.2f%% of transactions was successful.\n", (float) purchases200/customers);
    printf("%.2f%% of transactions failed, because proper seats weren't found.\n", (float) purchases404/customers);
    printf("%.2f%% of transactions failed, as the card was declined.\n", (float) purchases402/customers);
    
    double waiting_avg = (stop.tv_sec - start.tv_sec)/customers;
    printf("On average a customer needed %.2f to be served.\n", waiting_avg);

    printf("On average a customer needed %.2f to reach call center.\n", (float) waiting/(customers));
    printf("Total revenue %d\n", main_cash);
    

    return 0;
}
