#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <unistd.h>

#define CYCLES 100000

int opt, philosophes, nthreads;

void mange(int id) {
  printf("Philosophe [%d] mange\n", id);
}

void * philosophe(void * arg) {
  pthread_t phil[philosophes];
  pthread_mutex_t baguette[philosophes];
  int * id = (int * ) arg;
  int left = * id;
  int right = (left + 1) % philosophes;
  int i = 0;
  while (i < CYCLES) {
    // philosophe pense
    if (left < right) {
      pthread_mutex_lock( & baguette[left]);
      pthread_mutex_lock( & baguette[right]);
    } else {
      pthread_mutex_lock( & baguette[right]);
      pthread_mutex_lock( & baguette[left]);
    }
    mange( * id);
    pthread_mutex_unlock( & baguette[left]);
    pthread_mutex_unlock( & baguette[right]);
    i++;
  }
  return (NULL);
}

int main(int argc, char ** argv) {

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
  printf("%i\n",nthreads);
  printf("%i\n",philosophes);

  pthread_t phil[philosophes];
  pthread_mutex_t baguette[philosophes];

  for (int i = 0; i < nthreads; i++)
    pthread_create( &phil[i], NULL, philosophe, (void * ) &i);

  
  for (int i = 0; i < nthreads; i++)
    pthread_join(phil[i], NULL);
  
  return 0;

  
}