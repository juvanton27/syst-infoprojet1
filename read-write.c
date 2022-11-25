#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NREAD 2560
#define NWRITE 640

pthread_mutex_t mutex_writecount;
pthread_mutex_t mutex_readcount;
sem_t wsem;
sem_t rsem;
int readcount = 0; // nombre de readers
int writecount = 0; // nombre de writers

void write_database(void)
{
  // printf("J'écris en db\n");
}

void read_database(void)
{
  // printf("Je lis en db\n");
}

void *writer()
{
  int count = 0;
  while (count<NWRITE)
  {
    pthread_mutex_lock(&mutex_writecount);
    // section critique - writecount
    writecount++;
    if(writecount==1) {
      // premier writer arrive
      sem_wait(&rsem);
    }
    pthread_mutex_unlock(&mutex_writecount);

    sem_wait(&wsem);
    // section critique, un seul writer à la fois
    write_database();
    sem_post(&wsem);

    pthread_mutex_lock(&mutex_writecount);
    // section critique - writecount
    writecount--;
    if(writecount==0) {
      // départ du premier writer
      sem_post(&rsem);
    }
    pthread_mutex_unlock(&mutex_writecount);
    count++;
  }

  return EXIT_SUCCESS;
}

void *reader()
{
  int count=0;
  while (count<NREAD)
  {
    sem_wait(&rsem);
    pthread_mutex_lock(&mutex_readcount);
    // section critique
    readcount++;
    if (readcount == 1)
    { // arrivée du premier reader
      sem_wait(&wsem);
    }
    pthread_mutex_unlock(&mutex_readcount);
    
    sem_post(&rsem);
    read_database();
    
    pthread_mutex_lock(&mutex_readcount);
    // section critique
    readcount--;
    if (readcount == 0)
    { // départ du dernier reader
      sem_post(&wsem);
    }
    pthread_mutex_unlock(&mutex_readcount);
    count++;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  int opt;
  int nbthreadsReader;
  int nbthreadsWriter;
  while ((opt = getopt(argc, argv, "r:w:")) != -1)
  {
    switch (opt)
    {
    case 'r':
      nbthreadsReader = atoi(optarg);
      break;
    case 'w':
      nbthreadsWriter = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-r threads reader] [-w threads writer]", argv[0]);
    }
  }

  sem_init(&wsem, 0, 1);
  sem_init(&rsem, 0, 1);

  pthread_t threadsReader[nbthreadsReader];
  pthread_t threadsWriter[nbthreadsWriter];
  int err;
  // Creating threads
  for (long i = 0; i < nbthreadsReader; i++)
  {
    err = pthread_create(&(threadsReader[i]), NULL, &reader, NULL);
    if (err != 0)
      perror("pthread_create reader");
  }
  for (long i = 0; i < nbthreadsWriter; i++)
  {
    err = pthread_create(&(threadsWriter[i]), NULL, &writer, NULL);
    if (err != 0)
      perror("pthread_create writer");
  }
  // Joining threads
  for (int i = 0; i < nbthreadsReader; i++)
  {
    err = pthread_join(threadsReader[i], NULL);
    if (err != 0)
      perror("pthread_join");
  }
  for (int i = 0; i < nbthreadsWriter; i++)
  {
    err = pthread_join(threadsWriter[i], NULL);
    if (err != 0)
      perror("pthread_join");
  }
}
