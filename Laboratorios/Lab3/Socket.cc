#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

/* 
   char tipo: el tipo de socket que quiere definir
      's' para "stream
      'd' para "datagram"
   bool ipv6: si queremos un socket para IPv6
 */
Socket::Socket( char tipo, bool ipv6 ){
   /* se asume que el tipo es 's' = stream socket */
   if(!ipv6) {
      idSocket = socket(AF_INET, SOCK_STREAM, 0);  
      /* (dominio del socket, tipo del socket (TCP vs UDP), protocolo) */
   } else {
      idSocket = socket(AF_INET6, SOCK_STREAM, 0);
   }
   
}


Socket::~Socket(){
    Close();
}


void Socket::Close(){
   close(idSocket);
}

/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   int port: ubicacion del proceso, por ejemplo 80
 */
int Socket::Connect( char * hostip, int port ) {
   int conection;

   if(!ipv6) {
      struct sockaddr_in server_address;
      server_address.sin_family = AF_INET;
      server_address.sin_port = htons(port);  /* htons convierte el entero (puerto) en el formato adecuado */
      inet_aton(hostip, &server_address.sin_addr);    /* inet_aton interpreta la dirección ipv4 y la guarda en el lugar correspondiente (indicado en el segundo parm.) */
      conection = connect(idSocket, (struct sockaddr *) &server_address, sizeof(server_address));

   } else {
      struct sockaddr_in6 server_address6;
      server_address6.sin6_family = AF_INET6;
      server_address6.sin6_port = htons(port);
      inet_pton(AF_INET6, hostip, &server_address6.sin6_addr);
      conection = connect(idSocket, (struct sockaddr *) &server_address6, sizeof(server_address6));
   }
     
   return conection;    /* 0 si se hizo la conexión, -1 si la conexión falló */

}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( char *host, char *service ) {

   return -1;

}


int Socket::Read( char *text, int len ) {
   int lectura = read(idSocket, text, len);
   return lectura;

}


int Socket::Write( char *text, int len) {
   int escritura = write(idSocket, text, len);
   return escritura;

}


int Socket::Listen( int queue ) {

    return -1;

}


int Socket::Bind( int port ) {

    return -1;

}


Socket * Socket::Accept(){

    //return -1;

}


int Socket::Shutdown( int mode ) {

    return -1;

}


void Socket::SetIDSocket(int id){

    idSocket = id;

}

