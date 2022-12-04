#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 8
#define MAX_INT 128
#define MIN_INT 1
#if OPTIM == 0
  #define NPROD 8192
#else
  #define NPROD 6400
#endif

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int *buffer;
int pos = 0; // position to add items in buffer
int countprod = 0;

/**
 * @brief Create a random item
 * 
 * @return the produced item
 */
int produce_item()
{
  countprod++;
  return (rand() % (MAX_INT - MIN_INT + 1)) + MIN_INT;
}

/**
 * @brief Insert the produced item in the buffer
 * Also increase pos
 * @param item the item to add
 */
void insert_item(int item)
{
  buffer[pos++] = item;
  // simulate
  for (int i = 0; i < 10000; i++);
}

/**
 * @brief Remove the first item in the list
 * Also decrease pos
 */
void remove_item()
{
  for (int i = 1; i <= pos; i++)
  {
    buffer[i - 1] = buffer[i];
  }
  pos--;
  // simulate
  for (int i = 0; i < 10000; i++);
}

/**
 * @brief Partie 1 + Partie 2.4
 */
#if OPTIM == 0 || OPTIM == 3

  int fullsize = 0;
  int emptysize = SIZE;

  /**
   * @brief My implementation of the sem_wait
   * 
   * @param sem a pointer to a sem int
   */
  void my_wait(int *sem)
  {
    while(*sem <= 0);
    asm("movl %1, %%eax; decl %%eax; xchgl %%eax, %0;":"=r"(*sem):"r"(*sem):"%eax");
  }

  /**
   * @brief My implementation of the sem_post
   * 
   * @param sem a pointer to a sem int
   */
  void my_post(int *sem)
  {
    while(*sem >= SIZE-1);
    asm("movl %1, %%ebx; incl %%ebx; xchgl %%ebx, %0;":"=r"(*sem):"r"(*sem):"%ebx");
  }

  /**
   * @brief Produce an item and add it to the buffer
   * Act as Task 1.2 if launched with ./prod-cons
   * Act as Task 2.4 if launched with ./prod-cons-sem
   * @param args 
   * @return void* 
   */
  void *producer(void *args)
  {
    int item;
    while (countprod < NPROD)
    {
      // produce item
      item = produce_item();

      // attente d'une place libre
      #if OPTIM == 3
        my_wait(&emptysize);
      #else
        sem_wait(&empty);
      #endif
      pthread_mutex_lock(&mutex);

      // section critique
      insert_item(item);

      pthread_mutex_unlock(&mutex);
      // il y a une place remplie en plus
      #if OPTIM == 3
        my_post(&fullsize);
      #else
        sem_post(&full);
      #endif
    }

    return EXIT_SUCCESS;
  }

  /**
   * @brief Consume the first item of the buffer
   * Act as Task 1.2 if launched with ./prod-cons
   * Act as Task 2.4 if launched with ./prod-cons-sem
   * @param args 
   * @return void* 
   */
  void *consumer(void *args)
  {
    while (countprod < NPROD)
    {
      // attente d'une place remplie
      #if OPTIM == 3
        my_wait(&fullsize);
      #else 
        sem_wait(&full);
      #endif
      pthread_mutex_lock(&mutex);

      // section critique
      remove_item();

      pthread_mutex_unlock(&mutex);
      // il y a une place libre en plus
      #if OPTIM == 3
        my_post(&emptysize);
      #else
        sem_post(&empty);
      #endif
    }

    return EXIT_SUCCESS;
  }

/**
 * @brief Partie 2
 */
#else
  int verrou = 0;

  // Test and set
  #if OPTIM == 1
    /**
     * @brief Test and set lock
     * Task 2.1
     */
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

    /**
     * @brief Test and set unlock
     * Task 2.1
     */
    void unlock()
    {
      asm("movl $0, %%eax;"
          "xchgl %%eax, %0;"
          :"=r"(verrou)
          :
          :"%eax"
      );
    }

  // Test and test and set
  #else
    /**
     * @brief Test and test and set lock
     * Task 2.3
     */
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

    /**
     * @brief Test and test and set unlock
     * Task 2.3
     */
    void unlock()
    {
      asm("movl $0, %%eax;"
          "xchgl %%eax, %0;"
          :"=r"(verrou)
          :
          :"%eax"
      );
    }
  #endif

  /**
   * @brief Produce an item and add it to the buffer
   * Act as Task 2.1 if launched with ./prod-cons-test-and-set
   * Act as Task 2.3 if launched with ./prod-cons-test-and-test-and-set
   * @param args 
   * @return void* 
   */
  void *producer(void *args)
  {
    int item;
    while (countprod < NPROD)
    {
      // produce item
      item = produce_item();

      // attente d'une place libre
      sem_wait(&empty);
      lock();

      // section critique
      insert_item(item);

      unlock();
      // il y a une place remplie en plus
      sem_post(&full);
    }

    return EXIT_SUCCESS;
  }

  /**
   * @brief Consume the first item of the buffer
   * Act as Task 2.1 if launched with ./prod-cons-test-and-set
   * Act as Task 2.3 if launched with ./prod-cons-test-and-test-and-set
   * @param args 
   * @return void* 
   */
  void *consumer(void *args)
  {
    while (countprod < NPROD)
    {
      // attente d'une place remplie
      sem_wait(&full);
      lock();

      // section critique
      remove_item();

      unlock();
      // il y a une place libre en plus
      sem_post(&empty);
    }

    return EXIT_SUCCESS;
  }
#endif

int main(int argc, char **argv)
{
  // Args handling
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

  // Freeing resources
  sem_destroy(&empty);
  sem_destroy(&full);
  pthread_mutex_destroy(&mutex);
  free(buffer);

  return EXIT_SUCCESS;
}
