#include "Semaphore.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define N       10


void H( int );
void O( int );

Semaphore sO(1);	// Inicializado en 1
Semaphore sH(2);	// Inicializado en 

struct Compartidos {
        int nO;
        int nH;
};

typedef struct Compartidos Compartir;
Compartir * pComp;

int id = 0;



int main(){
        int i = 0;

// Creacion de las variables compartidas
        id = shmget( 0xABCDEF, sizeof( Compartir ), 0600 | IPC_CREAT );
        if ( -1 == id ) {
           perror( "main" );
           exit( 0 );
        }

        pComp = ( Compartir * ) shmat( id, NULL, 0 );


        srand( time( NULL ) );  // Genera una nueva semilla para los aleatorios

        while( i++ < N ) {
         int r = rand();
         //printf( "rand = %d\n", r );
        if ( ! fork() ) {       // Es el hijo
             if ( r % 2 ) {
                printf( "Se creo un atomo de oxigeno [%d]\n", i );
                O( i );
             } else {
                printf( "Se creo un atomo de hidrogeno [%d]\n", i );
                H( i );
             }
          }

        }

        i = 0;
int k, n;
        while ( i < N ) {
//printf("Esperando por proceso %d \n", i );
           n = wait( &k );
           i++;
        };

        printf( "Destruyendo los recursos de memoria compartida\n");
        shmdt( pComp );
        shmctl( id, IPC_RMID, NULL );

}       // main



void O( int i ){

	if ( pComp->nH > 1 ) {

	   pComp->nH -= 2;
      printf( "Molecula de agua creada por un O [%d] \n", i );
      sH.Signal();
      sH.Signal();
	} else {
      sO.Wait();
	   pComp->nO++;	   
	}

}

void H( int i ){

	if ( ( pComp->nH > 0) && (pComp->nO > 0) ) {
      

	   pComp->nH --;
	   pComp->nO --;

      printf( "Molecula de agua creada por un H [%d] \n", i );
      sH.Signal();
      sO.Signal();
	} else {
      sH.Wait();
	   pComp->nH++;
	}

}

