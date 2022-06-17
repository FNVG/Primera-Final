#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include<sys/wait.h>
#include <termios.h>
#include <math.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condprint = PTHREAD_COND_INITIALIZER;

struct Dato {
  int id;
  int number;
};



int next_thread = 0;
int mythread=0;

void* thread_function(void *argcthread) {
  pthread_mutex_lock (&mutex);        
struct Dato argumento = *(struct Dato*) argcthread;  
  
  while (next_thread != argumento.id) 
      pthread_cond_wait(&condprint, &mutex);

  int factorial = 1;
  for(int x=1;x<=argumento.number;x++)
       factorial=factorial*x;
  
  printf("Factorial de %d es %d\n",argumento.number, factorial);
next_thread++;
pthread_cond_broadcast(&condprint);
pthread_mutex_unlock (&mutex);
  return NULL;
  }


  



int main(int argc, char *argv[]) {
   
    pthread_t thread[argc-1];
    struct Dato arr_datos[argc-1];
    int k=0;
  
    for(int i=1;i<argc;i++) {
          arr_datos[k].id = k;
          arr_datos[k].number = atoi(argv[i]);  
           
   
      pthread_create(&thread[k], NULL, thread_function,&arr_datos[k]);

       k++;
      
      }
  
  
   for(int i=0;i<argc-1;i++) {
        pthread_join(thread[i], NULL);

    
    }

  
  return 0;
}
