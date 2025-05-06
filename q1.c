#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <semaphore.h>
#define MAX_SIZE 1000



sem_t table_sem;
sem_t waiter_sem;
sem_t cooks_sem;
pthread_mutex_t lock, lock2, lock3; 

struct Queue {
    int arr[MAX_SIZE];
    int rear;
    int front;
};

struct Queue kitchen_queue;
struct Queue ready_meals_queue;
struct Queue customer_queue;
struct Queue table_queue;




struct Params{
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
    
    sem_wait(&table_sem);
    pthread_mutex_lock(&lock);
    int current_table = table_queue.arr[table_queue.front];   
    table_queue.front++;
    
    
    customer_queue.arr[customer_queue.rear] = current_customer;
    customer_queue.rear++;
    pthread_mutex_unlock(&lock);
    printf("Customer %d is seated at Table %d\n", current_customer, current_table);
    while(ready_meals_queue.arr[ready_meals_queue.front-1]!=current_customer);
    printf("Customer %d finishes eating and leaves Table %d\n", current_customer, current_table);
    table_queue.rear++;
    table_queue.arr[table_queue.rear] = current_table;
    
    sem_post(&table_sem);


}

void *waiters_thread(void *params){
    while(customer_queue.arr[customer_queue.front]!=0){
        struct Params *my_params = (struct Params*) params;
        pthread_mutex_lock(&lock2);
        int current_customer = customer_queue.arr[customer_queue.front];
        
        if(current_customer!=0){
            kitchen_queue.arr[kitchen_queue.rear] = current_customer;
            kitchen_queue.rear++;
            customer_queue.front++;
            pthread_mutex_unlock(&lock2);
            int current_waiter = my_params->current_waiter;
            printf("Waiter %d takes order from Customer %d\n", current_waiter, current_customer);
            while(ready_meals_queue.arr[ready_meals_queue.front]!=current_customer);
            printf("Waiter %d serves meal to Customer %d\n", current_waiter, current_customer);
            ready_meals_queue.front++;
            
        }
        else pthread_mutex_unlock(&lock2);
    }
}

void *cooks_thread(void *params){

    while(customer_queue.arr[customer_queue.front]!=0){
        struct Params *my_params = (struct Params*) params;
        int current_cooks = my_params->current_cooks;
        pthread_mutex_lock(&lock3);
        int current_customer = kitchen_queue.arr[kitchen_queue.front];
        if(current_customer!=0){
            kitchen_queue.front++;
            printf("Cook %d is preparing order for Customer %d\n", current_cooks, current_customer);
            sleep(1);
            ready_meals_queue.arr[ready_meals_queue.rear] = current_customer;
            ready_meals_queue.rear++;
        }
        pthread_mutex_unlock(&lock3);
    }
}


int main(int argc, char **argv){

    
    if(argc!=5){
        printf("There must be exactly 4 arguments to be prompted as an argument");
    }
    else{

        if (pthread_mutex_init(&lock, NULL) != 0) { 
            printf("\n mutex init has failed\n"); 
            return 1; 
        } 
        if (pthread_mutex_init(&lock2, NULL) != 0) { 
            printf("\n mutex 2 init has failed\n"); 
            return 1; 
        } 
        if (pthread_mutex_init(&lock3, NULL) != 0) { 
            printf("\n mutex 2 init has failed\n"); 
            return 1; 
        } 


        int table_count = atoi(argv[1]);
        int waiter_count = atoi(argv[2]);
        int cooks_count = atoi(argv[3]);
        int customer_count = atoi(argv[4]);

        pthread_t customer_threads[customer_count];
        pthread_t waiter_threads[waiter_count];
        pthread_t cooks_threads[cooks_count];
        struct Params *params = (struct Params *) malloc(sizeof(struct Params));
        
        sem_init(&table_sem, 0, table_count);
        sem_init(&waiter_sem, 0, waiter_count);
        sem_init(&cooks_sem, 0, cooks_count);

        params->current_customer = 0;
        params->current_cooks = 0;
        params->current_waiter = 0;
        params->table_count = table_count;
        int i;
        for(i = 0;i<table_count;i++){
            table_queue.arr[i] = i+1;
        }
        table_queue.rear = table_count-1;
        table_queue.front = 0;
        customer_queue.front = 0;
        customer_queue.rear = 0;
        kitchen_queue.front = 0;
        kitchen_queue.rear = 0;
        ready_meals_queue.front = 0;
        ready_meals_queue.rear = 0;

        for(i = 0;i<customer_count;i++){
            struct Params *params = (struct Params *) malloc(sizeof(struct Params));
            params->current_customer = i+1;
            pthread_create(&customer_threads[i], NULL, customer_thread, (void *)params);
           
        }


        for(i = 0;i<waiter_count;i++){
            struct Params *params = (struct Params *) malloc(sizeof(struct Params));
            params->current_waiter = i+1;
            pthread_create(&waiter_threads[i], NULL, waiters_thread, (void *)params);
           
        }

        for(i = 0;i<cooks_count;i++){
            struct Params *params = (struct Params *) malloc(sizeof(struct Params));
            params->current_cooks = i+1;
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