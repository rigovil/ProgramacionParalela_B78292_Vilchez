#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <time.h>
#include <mutex>
#include "Semaphore.h"

#define CANTIDAD_HILOS 100

int oxigeno;
int hidrogeno;

std::mutex mutexO;
std::mutex mutexH;

Semaphore sO;
Semaphore sH;

void H(int);
void O(int);

int main() {
   srand(time(NULL));
   std::thread hilos[CANTIDAD_HILOS];

   for(int i = 0; i < CANTIDAD_HILOS; ++i) {
      int r = rand();
      if(r % 2) {
         printf("Se creo un atomo de oxigeno [%d]\n", i);
         hilos[i] = std::thread(O,i);
      }
      else {
         printf("Se creo un atomo de hidrogeno [%d]\n", i);
         hilos[i] = std::thread(H, i);
      }
   }

   for(int i = 0; i < CANTIDAD_HILOS; ++i) {
      hilos[i].join();
   }
}

void H(int i) {

   if(hidrogeno > 0 && oxigeno > 0) {
      printf("Molecula de agua creada por un H [%d]\n", i);
      sH.Signal();
      sO.Signal();

      mutexH.lock();
      hidrogeno--;
      mutexH.unlock();

      mutexO.lock();
      oxigeno--;
      mutexO.unlock();
   } 
   else {
      mutexH.lock();
      hidrogeno++;
      mutexH.unlock();
      sH.Wait();
   }
}

void O(int i) {
   if(hidrogeno > 1) {
      printf("Molecula de agua creada por un O [%d]\n", i);
      sH.Signal();
      sH.Signal();

      mutexH.lock();
      hidrogeno-= 2;
      mutexH.unlock();
   } 
   else {
      mutexO.lock();
      oxigeno++;
      mutexO.lock();
      sO.Wait();
   }
}