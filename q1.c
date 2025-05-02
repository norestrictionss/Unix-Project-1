#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <semaphore.h>
#define MAX_SIZE 100


sem_t table_sem;
sem_t customer_sem;
sem_t waiter_sem;

struct Queue {
    int *arr;
    int rear;
    int front;
};

struct Params{
    struct Queue kitchen_queue;
    struct Queue ready_meals_queue;
    struct Queue table_queue;
    int current_customer;
    int current_waiter;
    int current_cooks;
};


/*
    0 -> Customer
    1 -> Waiter
    2 -> Cooks
*/
void *customer_thread(void *params){
    struct Params *my_params = (struct Params*) params;
    sem_wait(&table_sem);
    my_params->table_queue.rear++;
    int rear = my_params->table_queue.rear;
    int front = my_params->table_queue.front;
    my_params->table_queue.arr[rear] = my_params->current_customer;
    my_params->current_customer++;
    printf("Customer %d is seated at Table %d\n", my_params->current_customer, my_params->table_queue.arr[front]);
}

void *waiters_thread(void *params){
    struct Params *my_params = (struct Params*) params;
    sem_wait(&customer_sem);
    my_params->kitchen_queue.rear++;

    int rear = my_params->kitchen_queue.rear;
    int front = my_params->table_queue.front;

    my_params->kitchen_queue.arr[rear] = my_params->table_queue.arr[front];
    my_params->table_queue.front++;

    printf("Waiter %d serves meal to Customer %d\n", my_params->current_waiter, my_params->ready_meals_queue.arr[front]);
    my_params->ready_meals_queue.front++;
}

void *cooks_thread(void *params){
    struct Params *my_params = (struct Params*) params;
    sem_wait(&waiter_sem);
    int front = my_params->kitchen_queue.front;
    printf("Cook %d is preparing order for Customer %d\n", my_params->current_cooks, my_params->kitchen_queue.arr[front]);
    my_params->ready_meals_queue.rear++;
    int rear = my_params->ready_meals_queue.rear;
    front = my_params->kitchen_queue.front;
    my_params->ready_meals_queue.arr[rear] = my_params->kitchen_queue.arr[front];
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
        struct Queue table_queue;
        struct Params *params = (struct Params *) malloc(sizeof(struct Params));
        
        sem_init(&table_sem, 0, table_count);
        sem_init(&waiter_sem, 0, waiter_count);
        sem_init(&customer_sem, 0, customer_count);

        kitchen_queue.arr = (int *) malloc(sizeof(int)*customer_count);
        ready_meals_queue.arr = (int *) malloc(sizeof(int)*customer_count);
        table_queue.arr = (int *) malloc(sizeof(int)*table_count);


        params->kitchen_queue = kitchen_queue;
        params->ready_meals_queue = ready_meals_queue;
        params->table_queue = table_queue;

        int i;

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


        
        
    }

    return 0;
}