#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define CANTIDAD_HILOS 2

typedef struct {
    int maxNumero;
    int maxPasos;
    double tiempo;
} calculo;

typedef struct {
    long limiteInferior;
    long limiteSuperior;
    long thread_id;
} datos_hilo;

/* VARIABLES GLOBALES */

int num_hilos;
long pasosTotales;
double time_spent;
calculo *buf;
pthread_mutex_t mutex;

/* MÉTODOS */

void * collatz(void * datos);



int main(int argc, char* argv[]) {
    time_t begin = time(NULL);

    num_hilos = strtol(argv[2], NULL, 10);
    pasosTotales = 0;
    time_spent = 0;
    pthread_mutex_init(&mutex, NULL);

    long inferior, superior;
    long limiteSuperior = strtol(argv[1], NULL, 10);
    
    if(limiteSuperior % num_hilos != 0) {
        num_hilos = CANTIDAD_HILOS;
        printf("La cantidad de hilos debe ser multiplo de %ld, se usaran %d hilos.\n", limiteSuperior, CANTIDAD_HILOS);
    }

    long incremento = limiteSuperior/num_hilos;
    pthread_t *hilos;

    hilos = malloc(num_hilos*sizeof(pthread_t));
    buf = malloc(num_hilos*sizeof(calculo));

    for(int i = 0; i < num_hilos; ++i) {
        datos_hilo *datos = malloc(sizeof(datos_hilo));
        datos->limiteInferior = (i * incremento) + 1;
        datos->limiteSuperior = (i + 1) * incremento;
        datos->thread_id = i;
        pthread_create(&hilos[i], NULL, collatz, (void *) datos);
    }

    for(int i = 0; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    int numero = 0;
    int pasos = 0;

    for(int i = 0; i < num_hilos; ++i) {
        if(buf[i].maxPasos > pasos) {
            numero = buf[i].maxNumero;
            pasos = buf[i].maxPasos;
        }
    }

    time_t end = time(NULL);

    printf("\nNumero con mas pasos: %d \nCantidad de pasos: %d \nTiempo entre todos los procesos: %f \n"
            "Tiempo real de ejecución: %ld \nTotal de pasos: %ld\n", numero, pasos, time_spent, (end-begin), pasosTotales);
    
    free(hilos);
    free(buf);

    return 0;
}



void * collatz(void * datos) {

    clock_t begin = clock();

    long pasosPorHilo = 0;
    datos_hilo *info = (datos_hilo *) datos;
    calculo m;
    m.tiempo = 0.0;
    m.maxNumero = 0;
    m.maxPasos = 0;

    printf("HILO #%ld \t RANGO DE NUMEROS: [%ld, %ld]\n", info->thread_id, info->limiteInferior, info->limiteSuperior);

    for(int i = info->limiteInferior; i <= info->limiteSuperior; ++i) {
        long pasos = 0;
        long n = i;

        while(n != 1) {
            if(n % 2 == 0) {    // es par
                n /= 2;
            } else {    // es impar
                n = 3*n + 1;
            }
            ++pasos;
        }

        if(pasos > m.maxPasos) {
            m.maxNumero = i;
            m.maxPasos = pasos;
        }

        pasosPorHilo+= pasos;
    }

    buf[info->thread_id] = m;
    free(info);

    clock_t end = clock();

    pthread_mutex_lock(&mutex);
    pasosTotales+= pasosPorHilo;
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}