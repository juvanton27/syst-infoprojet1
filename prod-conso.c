#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 8
#define MAX_INT 128
#define MIN_INT 1
#define NPROD 64

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int *buffer;
int pos = 0; // position to add items in buffer
int countprod = 0;

void print_array()
{
  printf("[");
  for (int i = 0; i < SIZE; i++)
  {
    printf("%i, ", buffer[i]);
  }
  printf("]\n");
}

int produce_item()
{
  countprod++;
  return (rand() % (MAX_INT - MIN_INT + 1)) + MIN_INT;
}

void insert_item(int item)
{
  printf("Trying to insert %i => ", item);
  buffer[pos++] = item;
  print_array();
  // simulate
  for (int i = 0; i < 10000; i++)
    ;
}

void remove_item()
{
  int item = buffer[0];
  printf("Trying to remove %i => ", item);
  for (int i = 1; i <= pos; i++)
  {
    buffer[i - 1] = buffer[i];
  }
  pos--;
  print_array();
  // simulate
  for (int i = 0; i < 10000; i++)
    ;
}

void *producer(void *args)
{
  int item;
  while (countprod < NPROD)
  {
    // produce item
    item = produce_item();

    // attente d'une place libre
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    // section critique
    insert_item(item);

    pthread_mutex_unlock(&mutex);
    // il y a une place remplie en plus
    sem_post(&full);
  }

  return EXIT_SUCCESS;
}

void *consumer(void *args)
{
  int item;
  while (countprod < NPROD)
  {
    // attente d'une place remplie
    int err = sem_wait(&full);
    if(err!=0) perror("sem wqit");
    pthread_mutex_lock(&mutex);

    // section critique
    remove_item();

    pthread_mutex_unlock(&mutex);
    // il y a une place libre en plus
    sem_post(&empty);
  }

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  int opt;
  int nthreadsProd;
  int nthreadsConso;

  while ((opt = getopt(argc, argv, "p:c:")) != -1)
  {
    switch (opt)
    {
    case 'p':
      nthreadsProd = atoi(optarg);
      break;
    case 'c':
      nthreadsConso = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-p threadsProd] [-c threadsConso]\n", argv[0]);
    }
  }

  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, SIZE);
  sem_init(&full, 0, 0);

  buffer = malloc(SIZE * sizeof(int));

  pthread_t threadsProd[nthreadsProd];
  pthread_t threadsConso[nthreadsConso];
  int err;

  // Creating threads
  for (long i = 0; i < nthreadsProd; i++)
  {
    err = pthread_create(&threadsProd[i], NULL, &producer, NULL);
    if (err != 0)
      perror("Failed to create thread");
  }
  for (long i = 0; i < nthreadsConso; i++)
  {
    err = pthread_create(&threadsConso[i], NULL, &consumer, NULL);
    if (err != 0)
      perror("Failed to create thread");
  }
  // Joining threads
  for (long i = 0; i < nthreadsProd; i++)
  {
    err = pthread_join(threadsProd[i], NULL);
    if (err != 0)
      perror("Failed to join thread");
  }
  for (long i = 0; i < nthreadsConso; i++)
  {
    err = pthread_join(threadsConso[i], NULL);
    if (err != 0)
      perror("Failed to join thread");
  }

  // sem_destroy(&empty);
  // sem_destroy(&full);
  // pthread_mutex_destroy(&mutex);
  return EXIT_SUCCESS;
}
