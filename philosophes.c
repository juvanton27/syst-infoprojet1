#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <unistd.h>

#define CYCLES 100000

int philosophes;

// Declare the threads, mutexes and philosopher IDs as global variables
pthread_t phil[];
pthread_mutex_t baguette[];
int philosopher_ids[];

void mange(int id) {
  printf("Philosophe %d mange\n", id);
}

void * philosophe(void * arg) {
  // Get the philosopher ID from the argument
  int id = * ((int * ) arg);
  int left = id;
  int right = (left + 1) % philosophes;
  int i = 0;
  while (i < CYCLES) {
    // Check if the mutex is already locked before trying to lock it
    if (pthread_mutex_trylock( & baguette[left]) == 0) {
      if (pthread_mutex_trylock( & baguette[right]) == 0) {
        mange(id);
        pthread_mutex_unlock( & baguette[left]);
        pthread_mutex_unlock( & baguette[right]);
      }
    }
    i++;
  }
  return (NULL);
}

int main(int argc, char ** argv) {
  // Args handling
  int opt;
  int nthreads;
  while ((opt = getopt(argc, argv, "j:n:")) != -1) {
    switch (opt) {
    case 'j':
      nthreads = atoi(optarg);
      break;
    case 'n':
      philosophes = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-j threads] [-n philosophes]\n", argv[0]);
    }
  }

  // Initialize the arrays to be the same size as nthreads
  pthread_t phil[nthreads];
  pthread_mutex_t baguette[nthreads];
  int philosopher_ids[nthreads];

  // Initialize the mutexes and philosopher IDs
  for (int i = 0; i < nthreads; i++) {
    pthread_mutex_init( & baguette[i], NULL);
    philosopher_ids[i] = i;
  }

  // Creating threads
  for (int i = 0; i < nthreads; i++)
    pthread_create( & phil[i], NULL, philosophe, (void * ) & philosopher_ids[i]);
  // Joining threads
  for (int i = 0; i < nthreads; i++)
    pthread_join(phil[i], NULL);

  return EXIT_SUCCESS;
}