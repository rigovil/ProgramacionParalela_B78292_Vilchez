/*
   C++ class to encapsulate Unix semaphore intrinsic structures and
   system calls
*/

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0xB78292	// Valor de la llave del recurso

class Semaphore {
   public:
      Semaphore(int ValorInicial = 1);
      ~Semaphore();
      int Signal();	// Puede llamarse V
      int Wait();	// Puede llamarse P
   private:
      int id;		// Identificador del semaforo
};


