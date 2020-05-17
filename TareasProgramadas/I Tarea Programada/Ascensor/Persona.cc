#include "Persona.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SUBIR 1
#define BAJAR -1
#define MAX_ESPERA 5
#define PISOS 10
#define MAX_PISOS 10

// MENSAJES

#define PULSAR_BOTON 0
#define ACCION_SUBIRSE 1
#define ACCION_BAJARSE 2
#define NO_PUEDE_SUBIR 3

// GLOBALES

extern std::vector<Persona *> solicitudes;
extern int personas_esperando[PISOS];
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern sem_t sem_ascensor;
extern sem_t sem_ascensor2;
extern sem_t sem_ascensor3;

int asignarDireccion(int piso_actual, int piso_destino) {
    int direccion = BAJAR;

    if(piso_actual < piso_destino) {
        direccion = SUBIR;
    }

    return direccion;   
}

Persona::Persona(int id) {
    this->id = id;
    this->piso_actual = (rand() % MAX_PISOS) + 1;
    this->piso_destino = (rand() % MAX_PISOS) + 1;
    
    while(piso_destino == piso_actual) {
        this->piso_destino = (rand() % MAX_PISOS) + 1;
    }

    this->direccion = asignarDireccion(this->piso_actual, this->piso_destino);

    int segundos_espera = rand() % MAX_ESPERA;
    sleep(segundos_espera);
}

Persona::~Persona() {
}

void Persona::pulsarBoton() {

    pthread_mutex_lock(&mutex2);
    solicitudes.push_back(this);

    if(solicitudes.size() == 1) {
        sem_post(&sem_ascensor);
    }
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex1);
    personas_esperando[this->piso_actual - 1]+= 1;
    this->mensaje(PULSAR_BOTON);

    pthread_mutex_unlock(&mutex1);
}

void Persona::subirse() {
    pthread_mutex_lock(&mutex1);

    this->en_ascensor = true;
    personas_esperando[this->piso_actual - 1]-= 1;

    this->mensaje(ACCION_SUBIRSE);
    sem_post(&sem_ascensor2);

    pthread_mutex_unlock(&mutex1);
}

void Persona::bajarse(int piso) {
    this->piso_actual = piso;
    this->en_ascensor = false;
    this->fue_atendido = true;
    this->mensaje(ACCION_BAJARSE);

    sem_post(&sem_ascensor3);
}

void Persona::mensaje(int tipo) {
    switch(tipo) {
        case PULSAR_BOTON:
            printf("Persona %d: he pulsado el boton en el piso %d para ir al piso %d\n", this->id, this->piso_actual, this->piso_destino);
            break;

        case ACCION_SUBIRSE:
            printf("Persona %d: me he subido al ascensor en el piso %d\n", this->id, this->piso_actual);
            break;

        case ACCION_BAJARSE:
            printf("Persona %d: me he bajado en el piso %d\n", this->id, this->piso_actual);
            break;

        case NO_PUEDE_SUBIR:
            printf("Persona %d: no puedo subir al ascensor en este momento\n", this->id);
            break;
    }
}

bool Persona::puedeSubir(int direccion_ascensor, int piso_ascensor) {
    bool A = direccion_ascensor == this->direccion;
    bool B = piso_ascensor == this->piso_actual;
    bool C = !this->en_ascensor;

    if(B & !(A || C)) {
        this->mensaje(NO_PUEDE_SUBIR);
    }

    return(A & B & C);
}