#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <unistd.h>

#include <semaphore.h>

#define SIZE 8
#define MAX_INT 
#define MIN_INT 

int opt, threadsprod, threadsconso;

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[SIZE];
int count = 0;


void * producer(void * args) {
  for (int i=0; i<10000; i++){
    // Produce
    int x = 8192;
    sleep(1);

    // Add to the buffer
    sem_wait( & semEmpty);
    pthread_mutex_lock( & mutexBuffer);
    buffer[count] = x;
    count++;
    pthread_mutex_unlock( & mutexBuffer);
    sem_post( & semFull);
  }
}

void * consumer(void * args) {
  for (int i=0; i<10000; i++){
    int y;

    // Remove from the buffer
    sem_wait( & semFull);
    pthread_mutex_lock( & mutexBuffer);
    y = buffer[count - 1];
    count--;
    pthread_mutex_unlock( & mutexBuffer);
    sem_post( & semEmpty);

    // Consume
    printf("Got %d\n", y);
    sleep(1);
  }
}

int main(int argc, char ** argv) {

  while ((opt = getopt(argc, argv, "p:c:")) != -1) {
    switch (opt) {
    case 'p':
      threadsprod = atoi(optarg);
      break;
    case 'c':
      threadsconso = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-p threadsProd] [-c threadsConso]\n", argv[0]);
    }
  }

  pthread_t thProd[threadsprod];

  pthread_t thConso[threadsconso];

  pthread_mutex_init( & mutexBuffer, NULL);
  sem_init( & semEmpty, 0, SIZE);
  sem_init( & semFull, 0, 0);
  int i;

  for (i = 0; i < threadsprod; i++) {
    if (i > 0) {
      if (pthread_create( & thProd[i], NULL, & producer, NULL) != 0) {
        perror("Failed to create thread");
      }
    }
  }

  for (i = 0; i < threadsconso; i++) {
    if (i > 0) {
      if (pthread_create( & thConso[i], NULL, & consumer, NULL) != 0) {
        perror("Failed to create thread");
      }
    }
  }
  for (i = 0; i < threadsprod; i++) {
    if (pthread_join(thProd[i], NULL) != 0) {
      perror("Failed to join thread");
    }
  }

  for (i = 0; i < threadsconso; i++) {
    if (pthread_join(thConso[i], NULL) != 0) {
      perror("Failed to join thread");
    }
  }
  sem_destroy( & semEmpty);
  sem_destroy( & semFull);
  pthread_mutex_destroy( & mutexBuffer);
  return 0;
}

