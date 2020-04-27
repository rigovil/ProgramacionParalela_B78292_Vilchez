#include "Buzon.h"

Buzon :: Buzon() {
    id = msgget(KEY, 0600 | IPC_CREAT);
}

int Buzon :: Enviar(const char *mensaje, int veces, long tipo) {

    struct msgbuf {
        long mtype;	// Tipo del mensaje a enviar, tiene que estar de primero en la estructura// Esta variable, para enviar, debe ser > 0
        int veces;	// Cantidad de veces que aparece la etiqueta HTML
        char etiqueta[32];	// Etiqueta HTML
    };
    
    struct msgbuf msj;
    msj.mtype = tipo;
    msj.veces = veces;
    strncpy(msj.etiqueta, mensaje, veces);

    return msgsnd(id, (const void *) &msj, sizeof(msj), 0);
}


int Buzon :: Recibir(char *mensaje, int veces, long tipo) {

    struct msgbuf {
        long mtype;	// Tipo del mensaje a enviar, tiene que estar de primero en la estructura// Esta variable, para enviar, debe ser > 0
        int veces;	// Cantidad de veces que aparece la etiqueta HTML
        char etiqueta[32];	// Etiqueta HTML
    }msj2;
    
    int r;
    msj2.mtype = tipo;
    r = msgrcv(id, (void *) &msj2, sizeof(msj2), 2020, IPC_NOWAIT);

    int size = strlen(msj2.etiqueta);
    strncpy(mensaje, msj2.etiqueta, size);

    return r;
}