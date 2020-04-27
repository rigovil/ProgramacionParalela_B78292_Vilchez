/*
 *  Esta clase encapsula los llamados al sistema en UNIX, para intercambio de mensajes por medio de buzones
 *  Se provee facilidades para el envio y recepcion de mensajes de cualquier tipo definido por el usuario
 *
 *  Autor: CI0117 Programacion Paralela y Concurrente
 *  Fecha: 2020-i
 *
 *  Ejemplo de un mensaje que contiene etiquetas HTML y su cantidad de apariciones:
 *
 *  struct msgbuf {
 *     long mtype;	// Tipo del mensaje a enviar, tiene que estar de primero en la estructura
 *			// Esta variable, para enviar, debe ser > 0
 *     int veces;	// Cantidad de veces que aparece la etiqueta HTML
 *     char etiqueta[ 32];	// Etiqueta HTML
 *  };
 */
#include "string.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 0xB78292	// Valor de la llave del recurso

class Buzon {
   public:
      Buzon();
      int Enviar( const char * mensaje, int veces, long tipo );	// Envia la tira de caracteres como un mensaje tipo
      int Recibir( char * mensaje, int veces, long tipo );   // len es el tamaño máximo que soporte la variable mensaje

   private:
      int id;		// Identificador del buzon
};

