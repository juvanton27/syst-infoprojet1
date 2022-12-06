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

#if OPTIM != 0
  int * verrou;
#endif

/**
 * @brief Do nothing
 * 
 * @param id the philospoh's id
 */
void mange(int id) {}

/**
 * @brief Partie 1
 */
#if OPTIM == 0
  /**
   * @brief Philosoph's problem
   * Act as 1.1 if launched with ./philosophe
   * @param arg 
   * @return void* 
   */
  void *philosophe(void *arg)
  {
    // Get the philosopher ID from the argument
    int id = *((int *)arg);
    int left = id;
    int right = (left + 1) % philosophes;
    int i = 0;
    while (i < CYCLES)
    {
      // Check if the mutex is already locked before trying to lock it
      if (pthread_mutex_trylock(&baguette[left]) == 0)
      {
        if (pthread_mutex_trylock(&baguette[right]) == 0)
        {
          mange(id);
          pthread_mutex_unlock(&baguette[left]);
          pthread_mutex_unlock(&baguette[right]);
        }
      }
      i++;
    }
    return (NULL);
  }
/**
 * @brief Partie 2
 */
#else

  // Test and set
  #if OPTIM == 1

    /**
     * @brief Test and set lock
     * Task 2.1
     * @param side 
     */
    void lock(int side)
    {
      // while (verrou[side] == 1);
      // asm("movl $1, %%eax;"
      //     "xchgl %%eax, %0;"
      //     : "=r"(verrou[side])
      //     :
      //     : "%eax");
    }

    /**
     * @brief Test and set unlock
     * Task 2.1
     * @param side 
     */
    void unlock(int side)
    {
      // asm("movl $0, %%eax;"
      //     "xchgl %%eax, %0;"
      //     : "=r"(verrou[side])
      //     :
      //     : "%eax");
    }

  // Test and test and set
  #else
    /**
     * @brief Test and test and set lock
     * Task 2.3
     * @param side left or right
     */
    void lock(int side)
    {
      // while (verrou[side] == 1)
      // {
      //   while (verrou[side]);
      // }
      // asm("movl $1, %%eax;"
      //     "xchgl %%eax, %0;"
      //     : "=r"(verrou[side])
      //     :
      //     : "%eax");
    }

    /**
     * @brief Test and test and set unlock
     * Task 2.3
     * @param side left or right
     */
    void unlock(int side)
    {
      // asm("movl $0, %%eax;"
      //     "xchgl %%eax, %0;"
      //     : "=r"(verrou[side])
      //     :
      //     : "%eax");
    }
  #endif

  /**
   * @brief Philosoph's problem
   * Act as 2.1 if launched with ./philosophe-test-and-set
   * Act as 2.3 if launched with ./philosophe-test-and-test-and-set
   * @param arg 
   * @return void* 
   */
  void *philosophe(void *arg)
  {
    // Get the philosopher ID from the argument
    int id = *((int *)arg);
    int left = id;
    int right = (left + 1) % philosophes;
    int i = 0;
    while (i < CYCLES)
    {
      if (left < right)
      {
        lock(left);
        lock(right);
      }
      else
      {
        lock(right);
        lock(left);
      }
      mange(id);
      unlock(left);
      unlock(right);
      i++;
    }
    return (NULL);
  }
#endif

int main(int argc, char **argv)
{
  // Args handling
  int opt;
  int nthreads;
  while ((opt = getopt(argc, argv, "j:n:")) != -1)
  {
    switch (opt)
    {
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

  #if OPTIM != 0
    verrou = (int *) malloc(nthreads * sizeof(int));
  #endif

  // Initialize the mutexes and philosopher IDs
  for (int i = 0; i < nthreads; i++)
  {
    pthread_mutex_init(&baguette[i], NULL);
    philosopher_ids[i] = i;
    #if OPTIM != 0
      verrou[i] = 0;
    #endif
  }

  // Creating threads
  for (int i = 0; i < nthreads; i++)
    pthread_create(&phil[i], NULL, philosophe, (void *)&philosopher_ids[i]);
  // Joining threads
  for (int i = 0; i < nthreads; i++)
    pthread_join(phil[i], NULL);

  #if OPTIM != 0
    free(verrou);
  #endif

  return EXIT_SUCCESS;
}