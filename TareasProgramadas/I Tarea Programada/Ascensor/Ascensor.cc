#include "Ascensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

#define CAPACIDAD_MAXIMA 8
#define VACIO 0
#define PISO_INICIAL 1
#define PISOS 10
#define SOLICITUD 0

// MENSAJES

#define ASCENSOR_LLENO 1
#define PARAR 2
#define SUBIR_PISO 3
#define BAJAR_PISO 4
#define SIN_SOLICITUD 5

// DIRECCIONES

#define SUBIR 1
#define QUIETO 0
#define BAJAR -1


extern std::vector<Persona *> solicitudes;
extern int personas_esperando[PISOS];
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern sem_t sem_ascensor;
extern sem_t sem_ascensor2;
extern sem_t sem_ascensor3;

Ascensor::Ascensor(int cantidad_personas) {
    this->capacidad = VACIO;
    this->piso_actual = PISO_INICIAL;
    this->max_solicitudes = cantidad_personas;
    this->personas_dentro.fill(NULL);

}

Ascensor::~Ascensor() {

}

void Ascensor::operar() {
    
    while(!this->terminar()) {
        if(this->haySolicitudes()) {
            //sleep(1);

            this->bajarPersonas();

            if(this->piso_actual == PISOS) {
                this->direccion = BAJAR;
            } 
            else if(this->piso_actual == PISO_INICIAL) {
                this->direccion = SUBIR;
            }

            this->subirPersonas();

            if(this->direccion == SUBIR) {
                this->subirPiso();
            }
            else if(this->direccion == BAJAR) {
                this->bajarPiso();
            }

        } else {
            this->mensaje(SIN_SOLICITUD);
            sem_wait(&sem_ascensor);
            this->decidirDireccion();
        }
    }
}

bool Ascensor::lleno() {
    return(this->capacidad == CAPACIDAD_MAXIMA);
}

bool Ascensor::vacio() {
    return(this->capacidad == VACIO);
}

bool Ascensor::terminar() {
    return(this->personas_atendidas == this->max_solicitudes);
}

bool Ascensor::puedenSubirPersonas() {
    bool A = !this->lleno();

    pthread_mutex_lock(&mutex1);
    bool B = personas_esperando[this->piso_actual-1] != VACIO;
    pthread_mutex_unlock(&mutex1);

    return(A & B);
}

bool Ascensor::haySolicitudes() {
    bool hay = true;
    
    if(solicitudes.size() == 0) {
        hay = false;
    }

    return hay;
}

void Ascensor::subirPiso() {
    this->piso_actual++;
    this->mensaje(SUBIR_PISO);
}

void Ascensor::bajarPiso() {
    this->piso_actual--;
    this->mensaje(BAJAR_PISO);
}

void Ascensor::subirPersonas() {
    std::vector<Persona *>::iterator it;

    if(personas_esperando[this->piso_actual-1] != VACIO) {

        pthread_mutex_lock(&mutex2);
        for(it = solicitudes.begin(); this->puedenSubirPersonas() && it != solicitudes.end(); ++it) {

            Persona *p = *it;

            if(p->puedeSubir(this->direccion, this->piso_actual)) {
                p->subirse();
                sem_wait(&sem_ascensor2);

                this->subirPersonaAlAscensor(p);
            }
        }
        pthread_mutex_unlock(&mutex2);
    }
}

void Ascensor::bajarPersonas() {

    for(int i = 0; i < MAX_CAPACIDAD; ++i) {
        Persona *p = this->personas_dentro.at(i);

        if(p != NULL) {
            if(p->piso_destino == this->piso_actual) {
                this->borrarSolicitud(p);
                p->bajarse(this->piso_actual);

                sem_wait(&sem_ascensor3);

                this->personas_dentro.at(i) = NULL;
                this->capacidad-= 1;
                this->personas_atendidas+= 1;
            }
        }
    }
}

void Ascensor::mensaje(int tipo) {
    switch(tipo) {
        case ASCENSOR_LLENO:
            printf("Ascensor: el ascensor va lleno\n");
            break;

        case PARAR:
            printf("Ascensor: he parado en el piso %d\n", this->piso_actual);
            break;

        case SUBIR_PISO:
            printf("Ascensor: voy subiendo por el piso %d\n", this->piso_actual);
            break;

        case BAJAR_PISO:
            printf("Ascensor: voy bajando por el piso %d\n", this->piso_actual);
            break;

        case SIN_SOLICITUD:
            printf("Ascensor: estoy en el piso %d, no hay solicitudes\n", this->piso_actual);
            break;
    }

}

void Ascensor::borrarSolicitud(Persona * persona) {
    std::vector<Persona *>::iterator it;
    bool end = false;

    for(it = solicitudes.begin(); !end && it != solicitudes.end(); ++it) {
        Persona *p = *it;

        if(p->id == persona->id) {
            pthread_mutex_lock(&mutex2);
            solicitudes.erase(it);
            pthread_mutex_unlock(&mutex2);
            end = true;
        }
    }
    
}

void Ascensor::subirPersonaAlAscensor(Persona * persona) {
    bool insertado = false;

    for(int i = 0; !insertado && i < MAX_CAPACIDAD; ++i) {
        if(this->personas_dentro.at(i) == NULL) {
            this->personas_dentro.at(i) = persona;
            insertado = true;
        }
    }

    ++this->capacidad;
}

void Ascensor::decidirDireccion() {
    if(this->vacio()) {
        if(solicitudes[SOLICITUD]->piso_actual > this->piso_actual) {
            this->direccion = SUBIR;
        } 
        else if(solicitudes[SOLICITUD]->piso_actual < this->piso_actual) {
            this->direccion = BAJAR;
        } 
    }
}

int Ascensor::getPersonasAtendidas() {
    return(this->personas_atendidas);
}