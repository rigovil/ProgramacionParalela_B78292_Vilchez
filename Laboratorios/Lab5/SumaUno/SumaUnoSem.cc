#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <pthread.h>
#include "Semaphore.h"

int resultado;
Semaphore *s;

void * sumarUno(void * arg) {
    s->Wait();
    for (int i = 0; i < 1000; i++) {
      resultado++;
    }
    s->Signal();

    return NULL;
}


int main() {
    s = new Semaphore(1);
    pthread_t *hilos;
    int numero_hilos = 100;

    hilos = (pthread_t*) malloc(numero_hilos*sizeof(pthread_t));

    for(int i = 0; i < numero_hilos; ++i) {
        pthread_create(&hilos[i], NULL, sumarUno, NULL);
    }

    for(int i = 0; i < numero_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    free(hilos);
    delete s;

    printf("Resultado: %d", resultado);
}