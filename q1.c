#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <semaphore.h>
#define MAX_SIZE 100



sem_t table_sem;
sem_t waiter_sem;
sem_t cooks_sem;

struct Queue {
    int arr[MAX_SIZE];
    int rear;
    int front;
};


struct Params{
    struct Queue kitchen_queue;
    struct Queue ready_meals_queue;
    struct Queue customer_queue;
    struct Queue table_queue;
    int current_customer;
    int current_waiter;
    int current_cooks;
    int table_count;
};


/*
    0 -> Customer
    1 -> Waiter
    2 -> Cooks
*/
void *customer_thread(void *params){
    struct Params *my_params = (struct Params*) params;
    int current_customer = my_params->current_customer;
    printf("Customer %d arrives and waits for a table.\n", my_params->current_customer);
    my_params->current_customer++;
    sem_wait(&table_sem);
    int current_table = my_params->table_queue.arr[my_params->table_queue.front];    
    my_params->customer_queue.rear++;
    my_params->customer_queue.arr[my_params->customer_queue.rear] = my_params->current_customer;

    my_params->table_queue.front++;
    printf("Customer %d is seated at Table %d\n", current_customer, current_table);
    while(my_params->ready_meals_queue.arr[my_params->ready_meals_queue.front]!=current_customer);
    printf("Customer %d finishes eating and leaves Table %d\n", current_customer, current_table);
    my_params->table_queue.rear++;
    my_params->table_queue.arr[my_params->table_queue.rear] = current_table;
    sem_post(&table_sem);
}

void *waiters_thread(void *params){
    struct Params *my_params = (struct Params*) params;
    sem_wait(&waiter_sem);
    my_params->kitchen_queue.rear++;
    
    int current_customer = my_params->customer_queue.arr[my_params->customer_queue.front];
    int current_waiter = my_params->current_waiter;
    printf("Waiter %d takes order from Customer %d\n", current_waiter, my_params->customer_queue.arr[my_params->customer_queue.front]);
    my_params->kitchen_queue.arr[my_params->kitchen_queue.front] = my_params->customer_queue.arr[my_params->customer_queue.front];
    my_params->customer_queue.front++;


    while(my_params->ready_meals_queue.arr[my_params->ready_meals_queue.front]!=current_customer);
    printf("Waiter %d serves meal to Customer %d\n", current_waiter, current_customer);
    my_params->ready_meals_queue.front++;
    sem_post(&waiter_sem);
    
}

void *cooks_thread(void *params){
    struct Params *my_params = (struct Params*) params;
    sem_wait(&cooks_sem);
    int front = my_params->kitchen_queue.front;
    int current_cooks = my_params->current_cooks;
    int current_customer = my_params->kitchen_queue.arr[my_params->kitchen_queue.front];
    printf("Cook %d is preparing order for Customer %d\n", current_cooks, current_customer);
    sem_post(&cooks_sem);
    my_params->ready_meals_queue.rear++;
    my_params->ready_meals_queue.arr[my_params->ready_meals_queue.rear] = my_params->kitchen_queue.arr[my_params->kitchen_queue.front];
    my_params->kitchen_queue.front++;

}


int main(int argc, char **argv){

    
    if(argc!=5){
        printf("There must be exactly 4 arguments to be prompted as an argument");
    }
    else{
        int table_count = atoi(argv[1]);
        int waiter_count = atoi(argv[2]);
        int cooks_count = atoi(argv[3]);
        int customer_count = atoi(argv[4]);

        pthread_t customer_threads[table_count];
        pthread_t waiter_threads[waiter_count];
        pthread_t cooks_threads[cooks_count];
        struct Queue kitchen_queue;
        struct Queue ready_meals_queue;
        struct Queue customer_queue;
        struct Queue table_queue;
        struct Params *params = (struct Params *) malloc(sizeof(struct Params));
        
        sem_init(&table_sem, 0, table_count);
        sem_init(&waiter_sem, 0, waiter_count);
        sem_init(&cooks_sem, 0, cooks_count);

        params->kitchen_queue = kitchen_queue;
        params->ready_meals_queue = ready_meals_queue;
        params->customer_queue = customer_queue;
        params->current_customer = 0;
        params->current_cooks = 0;
        params->current_waiter = 0;
        params->table_count = table_count;
        int i;
        for(i = 0;i<table_count;i++){
            table_queue.arr[i] = i;
        }
        for(i = 0;i<customer_count;i++){
            params->current_customer = i;
            pthread_create(&customer_threads[i], NULL, customer_thread, (void *)params);
           
        }


        for(i = 0;i<waiter_count;i++){
            params->current_waiter = i;
            pthread_create(&waiter_threads[i], NULL, waiters_thread, (void *)params);
           
        }

        for(i = 0;i<cooks_count;i++){
            params->current_cooks = i;
            pthread_create(&cooks_threads[i], NULL, cooks_thread, (void *)params);
          
        }
        
        for(i = 0;i<customer_count;i++){
            pthread_join(customer_threads[i], NULL);
        }

        for(i = 0;i<waiter_count;i++){
            pthread_join(waiter_threads[i], NULL);
        }
        for(i = 0;i<cooks_count;i++){
            pthread_join(cooks_threads[i], NULL);
        }

        
        
    }

    return 0;
}