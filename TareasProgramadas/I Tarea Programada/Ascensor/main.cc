#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include "Ascensor.h"
#include "Persona.h"

#define ASCENSOR 1
#define PERSONA 0
#define INICIO 0
#define PISOS 10

int total_personas;
int id_persona;
long num_hilos;

// DATOS COMPARTIDOS POR LOS HILOS

std::vector<Persona *> solicitudes;
int personas_esperando[PISOS] = {0,0,0,0,0,0,0,0,0,0};
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
sem_t sem_ascensor;
sem_t sem_ascensor2;
sem_t sem_ascensor3;

void * iniciarAscensor(void * arg);
void * iniciarPersona(void * arg);


int main(int argc, char* argv[]) {

    srand(time(nullptr));
    pthread_t *hilos;

    id_persona = PERSONA;
    num_hilos = strtol(argv[1], NULL, 10) + ASCENSOR;
    hilos = (pthread_t *) malloc(num_hilos*sizeof(pthread_t));
    sem_init(&sem_ascensor, 0, 0);
    sem_init(&sem_ascensor2, 0, 0);
    sem_init(&sem_ascensor3, 0, 0);

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&hilos[INICIO], NULL, iniciarAscensor, NULL);     // crea el hilo del ascensor
    sleep(ASCENSOR);    // garantiza que el ascensor comience a operar antes de que alguna persona solicite el ascensor

    for(int i = ASCENSOR; i < num_hilos; ++i) {
        pthread_create(&hilos[i], NULL, iniciarPersona, NULL);       // crea un hilo por persona
    }

    for(int i = 1; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    pthread_join(hilos[0], NULL);

    sem_destroy(&sem_ascensor);
    sem_destroy(&sem_ascensor2);
    sem_destroy(&sem_ascensor3);

    free(hilos);

    return 0;
}

void * iniciarPersona(void * arg) {
    Persona *persona = new Persona(++id_persona);

    persona->pulsarBoton();
    
    while(!persona->fue_atendido) {}

    delete persona;
    return NULL;
}

void * iniciarAscensor(void * arg) {
    int cantidad_personas = num_hilos - ASCENSOR;
    Ascensor *ascensor = new Ascensor(cantidad_personas);

    ascensor->operar();
    printf("Personas dentro: %d, personas atendidads: %d, personas en espera: %ld\n", ascensor->capacidad, ascensor->getPersonasAtendidas(), solicitudes.size());
        
    return NULL;
}