#include "Buzon.h"

Buzon :: Buzon() {
    this->id = msgget(KEY, 0600 | IPC_CREAT);
}

int Buzon :: Enviar(struct my_msgbuf mensaje) {

    return msgsnd(this->id, (const void *) &mensaje, sizeof(struct my_msgbuf), 0);
}


struct my_msgbuf Buzon :: Recibir() {
    int parcial;

    struct my_msgbuf msj;

    msgrcv(this->id, (void *) &msj, sizeof(msj), 2020, IPC_NOWAIT);

    return msj;
}