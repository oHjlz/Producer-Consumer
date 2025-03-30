#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include "buffer.h"

#define BUFFER_SIZE 5
buffer_item buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

// Global semaphore and mutex variables
sem_t empty, full;      //counts empty/full buffer slots
pthread_mutex_t mutex;  // Ensures mutual exclusion



int insert_item(buffer_item item) {
    sem_wait(&empty);  //waits if there are no empty spots
    // pthread_mutex_lock(&mutex); //locks to ensure exclusive access.
    // /* insert item into buffer
    // return 0 if successful, otherwise
    // return -1 indicating an error condition */
    // if(sem_trywait(&empty) != 0){  //sem_trywait returns 0 for a successful decrement of empty
    //                                // returns -1 if empty = 0, unsuccesful removal
    //     return -1;
    // }

    pthread_mutex_lock(&mutex);

    buffer[in] = item;

    in = (in + 1) % BUFFER_SIZE;   //allows a circular implementation 
                         //wraps around when it gets to the end
    count++;
    pthread_mutex_unlock(&mutex); //unlock the mutex after modifying the buffer
    sem_post(&full); //signal that there is an item in the buffer
    return 0;
    
}
int remove_item(buffer_item *item) {
    sem_wait(&full);
    // if(sem_trywait(&full) != 0){  //returns 0 for a succesful decrement of full
    //                               //retruns -1 for unsuccesful
    //     return -1;
    // }     //waits for a full spot to remove
    pthread_mutex_lock(&mutex);
    /* remove an object from buffer
    placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */
   
    //stores the item that 'gets removed' so we can know what gets removed
    //the insert essentially overwrites the indexesm that we've 'removed'
    *item = buffer[out];

    //ciruclar wrap around
    out = (out + 1) % BUFFER_SIZE;

    count--;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty); //signal that there is an open space
    return 0;
}


// Buffer initialization function
void buffer_init() {
    // Initialize mutex (binary semaphore)
    // Initial value 1 means the buffer is initially unlocked
    pthread_mutex_init(&mutex, NULL);

    // Initialize empty semaphore
    // Initial value is BUFFER_SIZE (all slots are empty)
    sem_init(&empty, 0, BUFFER_SIZE);

    // Initialize full semaphore
    // Initial value is 0 (no slots are filled at start)
    sem_init(&full, 0, 0);
}

void *producer(void *param) {
    buffer_item item;
    while (1) {
        /* sleep for a random period of time */
        sleep(rand() % 3 + 1);
        /* generate a random number */
        item = rand() % 100;
        if (insert_item(item) == -1){
        fprintf(stderr, "Producer error: Buffer full\n");
        }else{
        printf("producer produced %d\n",item);
        }
    }
}
void *consumer(void *param) {
    buffer_item item;
    while (1) {
        /* sleep for a random period of time */
        sleep(rand() % 3 + 1);
        if (remove_item(&item) == -1){
        fprintf(stderr, "Consumer error: Buffer empty\n");
        }else{
        printf("consumer consumed %d\n",item);
        }
    }
}

int main(int argc, char *argv[]){
    //printf("Entering main\n");

    if(argc != 4) { //make sure there is the correct ammount of arguements
                    //example: ./buffer 1 3 4
        fprintf(stderr, "Usage: ./buffer <Sleep_time> <Producer_threads> <Consumer_threads>\n");
        exit(1);
    }

    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    if(sleep_time <= 0 || num_consumers <= 0 || num_producers <= 0){
        fprintf(stderr, "Error: All arguements must be positibe integers\n");
        exit(1);
    }

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    buffer_init();


    //filling arrays with threads
    for(int i = 0; i < num_producers; i ++){
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for(int i = 0; i < num_consumers; i ++){
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    sleep(sleep_time);


    // for(int i = 0; i < num_producers; i ++){
    //     pthread_join(producers[i], NULL);
    // }
    // for(int i = 0; i < num_consumers; i ++){
    //     pthread_join(consumers[i], NULL);
    // }

    //cleanup semaphores and the mutex
   // Sleep for the specified time
    
   printf("\nTime's up! Terminating program after %d seconds\n", sleep_time);

   // Cleanup semaphores and mutex before exiting
   sem_destroy(&empty);
   sem_destroy(&full);
   pthread_mutex_destroy(&mutex);
   
   return 0;
}

