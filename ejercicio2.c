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
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>
#define MAX 20
int vector[MAX];
int numeroBuscado = -1;
int posicion = -1;
int numeroHilos = 0;
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;
bool encontrado = false;
sem_t semaphore = {0};



struct Dato {
 int id;
  int comienzo;
  int final;
};


void *terminal_hilos (void *argcthread) {
  printf("\nAntes\n");

   if (sem_wait(&semaphore) != 0) {
        printf("ERROR: En WAIT");
      exit(EXIT_FAILURE);
   } 

  printf("\nDespues\n");
  
  encontrado = true;
 
 
  return NULL;
}
void avanzar() {

  
  if (sem_post(&semaphore) !=0 ) {
   printf("ERROR: en POST");
    exit(EXIT_FAILURE);
 } else {
    printf("\n\n");
 }
  
  
}

void *threat_function(void *argcthread) {
pthread_mutex_lock(&mutexB);
struct Dato argumento = *(struct Dato*) argcthread;  
 int i = argumento.comienzo;
  int ret2=0;
  while ((i <= argumento.final) && (!encontrado)) {  
    if (vector[i] == numeroBuscado) {
     posicion = i;
     printf("\nHilo %d encontro el numero primero\n", 
   argumento.id);    
    encontrado = true;
      }
   i++;
}
  pthread_mutex_unlock(&mutexB);
   return NULL;
  }



int main(int argc, char *argv[]) {
 int lower = 0, upper = 9, count = 1;
pthread_t tid_signal;
 sem_init(&semaphore,0,0);
   int VALUE = 0;

 /* sema =  *sem_open("/semaforo", O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO,0);
  */
//O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO
  
  srand(0);
    for (int i =0 ; i< MAX; i++) {
          vector [i] =  (rand() %
           (upper - lower + 1)) + lower;
        printf("Posicion %d -> %d\n", i,  vector[i]);
    }
printf("\n\n");
  numeroBuscado = atoi(argv[1]);
  numeroHilos = atoi(argv[2]);
     
   int c = MAX / numeroHilos;
  int r = MAX % numeroHilos;
  pthread_t tids[numeroHilos];
int recorrido_datos [numeroHilos];
  for (int i=0; i<numeroHilos; i++) {
    if (r > 0) {
      recorrido_datos[i] = c+1;
      r--;
    } else {
      recorrido_datos[i] = c;
    }
  }
   struct Dato arr_datos[numeroHilos];

 int comienzo =0;
    for (int i=0; i<numeroHilos; i++) {
      arr_datos[i].id = i;
      arr_datos[i].comienzo = comienzo;
      arr_datos[i].final = recorrido_datos[i] +comienzo -1;
      comienzo = recorrido_datos[i] +comienzo;
    } 

  for (int i=0; i<numeroHilos; i++) {
    printf("Hilo %d rango [%d,%d]\n",arr_datos[i].id, arr_datos[i].comienzo, arr_datos[i].final);
  }

printf("\n\n");
  //Hilos que buscan el numero
  for (int i=0; i<numeroHilos; i++) {
 if ( pthread_create(&tids[i], NULL,threat_function, &arr_datos[i])) {
    printf("\nERROR: pthread_create de los hilos\n");
     exit(EXIT_FAILURE);
 }

    
  }

  
  
 //Join de los hilos
  for (int i=0; i<numeroHilos; i++) {
  if (pthread_join(tids[i], NULL) !=0 ){
    printf("ERROR: funcion pthread_join de hilos que buscan el numero");
    exit(EXIT_FAILURE);
  }
    
  }
  
  if(posicion == -1)
  {
  
    printf("\nNo esta el dato en el vector\n");
  } else {
    printf("\n Posicion del elemento en el vector : %d\n", posicion);
  }
    

  
  //Solucionar el hilo que termina los demas hilos despues
  
  return 0;
}
