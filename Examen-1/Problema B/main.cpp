#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>

sem_t semaforo;

void cruzarBarranco(int direccion, int mono) {
    sem_wait(&semaforo);
    printf("Mono %d ha cruzado en direccion %d\n", mono, direccion);
    sem_post(&semaforo);
}

int main(int argc, char ** argv ) {
    int cantidad_monos, mono, pid;
    sem_init(&semaforo, 0, 1);
    srand(time(nullptr));

    if ( argc > 1 ) {
      cantidad_monos = atol( argv[ 1 ] );
    }

    for(mono = 0; mono < cantidad_monos; mono++) {
        int direccion = rand() % 2;
        pid = fork();
        if(!pid) {
            cruzarBarranco(direccion, mono);
        }
    }

    for (mono = 0; mono < 10; mono++ ) {
        int status;
        pid_t pid = wait( &status );
    }

    sem_destroy(&semaforo);

    return 0;
}