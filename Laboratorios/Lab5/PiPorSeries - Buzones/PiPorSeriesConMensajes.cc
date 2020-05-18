#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "Buzon.h"

double calcularSumaParcialPi(Buzon * miBuzon, int proceso, long inicial, long terminos ) {
   double casiPi = 0;
   double alterna = 4;
   long divisor = 0;
   struct my_msgbuf mensaje;

   for ( long termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		// 2 x termino + 1
      casiPi += alterna/divisor;		// 4 / (2xi + 1)
      alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
   }

   mensaje.mtype = 2020;
   mensaje.parcial = casiPi;
   miBuzon->Enviar(mensaje);
   exit( 0 );

}


int main( int argc, char ** argv ) {
   Buzon *miBuzon = new Buzon();

   long terminos, inicio, fin;
   int proceso;
   int pid, msgid;
   double resultado;
   struct my_msgbuf recibe;


   terminos = 1000000;

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
      if ( ! pid ) {
         calcularSumaParcialPi(miBuzon, proceso, inicio, fin );
      } else {
         printf("Creando el proceso %d: termino inicial %ld, termino final %ld\n", pid, inicio, fin );
      }
   }

   resultado = 0.0;
   for ( proceso = 0; proceso < 10; proceso++ ) {
      struct my_msgbuf mensaje_recibido;

      mensaje_recibido = miBuzon->Recibir();
      printf( "Resultado parcial recibido %15.10g \n", mensaje_recibido.parcial);
      resultado += mensaje_recibido.parcial;
   }

   printf( "Valor calculado de Pi es \033[91m %15.10g \033[0m con %ld terminos\n", resultado, terminos );

   msgctl( msgid, IPC_RMID, NULL );

}


