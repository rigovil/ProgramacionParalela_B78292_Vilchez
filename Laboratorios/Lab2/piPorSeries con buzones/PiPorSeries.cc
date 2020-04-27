/*
  Calcula el numero PI utilizando una serie "infinita"
  Debe recibir la cantidad de iteraciones como parametro

  pi = (-1)^i x 4/(2xi + 1)

*/

#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define KEY 0xB78292

struct my_msgbuf {
   long tipo;
   double n;
};


/*
   Realiza la acumulacion de terminos desde una posicion inicial hasta un termino final
*/
double calcularSumaParcialPi(my_msgbuf pi, int id, int proceso, long inicial, long terminos ) {
   double casiPi = 0;
   double alterna = 4;
   long divisor = 0;
   long termino;

   for ( termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		// 2 x termino + 1
      casiPi += alterna/divisor;		// 4 / (2xi + 1)
      alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
   }
   
   pi.n = casiPi;
   msgsnd(id, (const void *) &pi, sizeof(pi), IPC_NOWAIT);

   exit( 0 );
}


int main( int argc, char ** argv ) {
   long terminos, inicio, fin;
   int proceso;
   int pid;
   int id;
   
   struct my_msgbuf msj;
   msj.tipo = getpid();
   msj.n = 0;

   id = msgget(KEY, IPC_CREAT | 0600);

   terminos = 1000000;
   if ( argc > 1 ) {
      terminos = atol( argv[ 1 ] );
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
      if ( ! pid ) {
         calcularSumaParcialPi(msj, id, proceso, inicio, fin );
      } else {
         printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
      }
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }

   double resultado;
   for ( proceso = 1; proceso < 10; proceso++ ) {
      msgrcv(id, (void * ) &msj , sizeof(msj), 0 , 0);
      resultado += msj.n;
   }

   printf( "Valor calculado de Pi es \033[91m %g \033[0m con %ld terminos\n", resultado, terminos );


}

