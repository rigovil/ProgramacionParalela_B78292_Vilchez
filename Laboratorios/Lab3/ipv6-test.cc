#include <stdio.h>
#include <string.h>
#include "Socket.h"

int main( int argc, char * argv[] ) {
   printf("hola");
   Socket s( 's', true );
   char a[512];

   memset( a, 0, 512 );

   int c = s.Connect("fe80::ffb4:ea86:1aa4:19c%eno1", 80);
   printf("conexion: %d", c);
   s.Write(  (char *) "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n", 36 );
   s.Read( a, 512 );
   printf( "%s\n", a);
   s.Close();
}

