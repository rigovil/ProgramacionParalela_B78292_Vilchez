#include "Semaphore.h"

Semaphore :: Semaphore(int valorInicial) {

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    }arg;

    id = semget(KEY, 1, IPC_CREAT | 0600);
    int sctl = semctl(id, 0, SETVAL, arg);
    arg.val = valorInicial;
}

Semaphore :: ~Semaphore() {
    semctl(id, 0, IPC_RMID);
}

int Semaphore :: Signal() {
    struct sembuf sops_s = {0, 1, 0};    /* respectivamente correponde al numero de semaforo, operacion a realizar y la bandera */

    semop(id, &sops_s, 1);

}

int Semaphore :: Wait() {
    struct sembuf sops_w = {0, -1, 0};

    semop(id, &sops_w, 1);
}