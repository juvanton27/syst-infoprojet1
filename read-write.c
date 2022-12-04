#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#if DOPTIM == 0
  #define NREAD 2560
  #define NWRITE 640
#else
  #define NREAD 6400
  #define NWRITE 6400
#endif

sem_t wsem;
sem_t rsem;
int rcount = 0; // nombre de read effectués
int wcount = 0; // nombre de write effectués

void write_database(void)
{
  wcount++;
  for (int i = 0; i < 10000; i++);
}

void read_database(void)
{
  rcount++;
  for (int i = 0; i < 10000; i++);
}

/**
 * @brief Partie 1 + Partie 2.4
 */
#if OPTIM == 0 || OPTIM == 3

  pthread_mutex_t mutex_writecount;
  pthread_mutex_t mutex_readcount;
  int readcount = 0;  // nombre de readers
  int writecount = 0; // nombre de writers

  int readsize = 1;
  int writesize = 1;

  void my_wait(int *sem)
  {
    while(*sem <= 0);
    asm("movl %1, %%eax; decl %%eax; xchgl %%eax, %0;":"=r"(*sem):"r"(*sem):"%eax");
  }

  void my_post(int *sem)
  {
    while(*sem >= 1);
    asm("movl %1, %%ebx; incl %%ebx; xchgl %%ebx, %0;":"=r"(*sem):"r"(*sem):"%ebx");
  }

  void *writer()
  {
    while (wcount < NWRITE)
    {
      pthread_mutex_lock(&mutex_writecount);
      // section critique - writecount
      writecount++;
      if (writecount == 1)
      {
        // premier writer arrive
        #if OPTIM == 3
          my_wait(&readsize);
        #else
          sem_wait(&rsem);
        #endif
      }
      pthread_mutex_unlock(&mutex_writecount);

      #if OPTIM == 3
        my_wait(&writesize);
      #else
        sem_wait(&wsem);
      #endif
      // section critique, un seul writer à la fois
      write_database();
      #if OPTIM == 3
        my_post(&writesize);
      #else
        sem_post(&wsem);
      #endif

      pthread_mutex_lock(&mutex_writecount);
      // section critique - writecount
      writecount--;
      if (writecount == 0)
      {
        // départ du premier writer
        #if OPTIM == 3
          my_post(&readsize);
        #else
          sem_post(&rsem);
        #endif
      }
      pthread_mutex_unlock(&mutex_writecount);
    }

    return EXIT_SUCCESS;
  }

  void *reader()
  {
    while (rcount < NREAD)
    {
      #if OPTIM == 3
        my_wait(&readsize);
      #else
        sem_wait(&rsem);
      #endif
      pthread_mutex_lock(&mutex_readcount);
      // section critique
      readcount++;
      if (readcount == 1)
      { // arrivée du premier reader
        #if OPTIM == 3
          my_wait(&writesize);
        #else
          sem_wait(&wsem);
        #endif
      }
      pthread_mutex_unlock(&mutex_readcount);

      #if OPTIM == 3
        my_post(&readsize);
      #else
        sem_post(&rsem);
      #endif
      read_database();

      pthread_mutex_lock(&mutex_readcount);
      // section critique
      readcount--;
      if (readcount == 0)
      { // départ du dernier reader
        #if OPTIM == 3
          my_post(&writesize);
        #else
          sem_post(&wsem);
        #endif
      }
      pthread_mutex_unlock(&mutex_readcount);
    }

    return EXIT_SUCCESS;
  }

/**
 * @brief Partie 2
 */
#else

  int verrou = 0;

  // Test and set
  #if DOPTIM == 1
    void lock()
    {
      while (verrou == 1);
      asm("movl $1, %%eax;"
          "xchgl %%eax, %0;"
          :"=r"(verrou)
          :
          :"%eax"
      );
    }

  // Test and test and set
  #else
    void lock()
    {
      while (verrou == 1)
      {
        while(verrou);
      }
      asm("movl $1, %%eax;"
          "xchgl %%eax, %0;"
          :"=r"(verrou)
          :
          :"%eax"
      );
    }
  #endif

  void unlock()
  {
    asm("movl $0, %%eax;"
        "xchgl %%eax, %0;"
        :"=r"(verrou)
        :
        :"%eax"
    );
  }

  void *writer()
  {
    while (wcount < NWRITE)
    {
      lock();

      // critical section
      write_database();

      unlock();
    }

    return EXIT_SUCCESS;
  }

  void *reader()
  {
    while (rcount < NREAD)
    {
      lock();

      // critical section
      read_database();

      unlock();
    }

    return EXIT_SUCCESS;
  }
#endif

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
