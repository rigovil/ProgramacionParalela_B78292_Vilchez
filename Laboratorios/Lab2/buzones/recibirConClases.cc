
#include <stdio.h>
#include <string.h>

#include "Buzon.h"

int main( int argc, char ** argv ) {
   int id, st, veces;
   char label[32];
   Buzon m;

   while ( (st = m.Recibir(label, veces, 2020 ) ) > 0 ) {
      printf( "Label: %s, status %d \n", label, st );
   }

   id = msgget(KEY, 0);
   msgctl(id, IPC_RMID, 0);
}

   

