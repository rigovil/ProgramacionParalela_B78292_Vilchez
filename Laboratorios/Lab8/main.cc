#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include "FileReader.h"

#define NUMERO_ARCHIVOS 1       // para el lab8 se prueba con solo un archivo html

void maestroLector(std::string, int, int);
void contarEtiquetas(FileReader *, int);
std::mutex *mutex;
std::mutex mutexPrint;


int main(int argc, char ** argv) {
    mutex = new std::mutex;

    std::thread lectores[NUMERO_ARCHIVOS];

    for(int i = 0; i < NUMERO_ARCHIVOS; ++i) {
        lectores[i] = std::thread(maestroLector, argv[1], atoi(argv[2]), atoi(argv[3]));
    }

    for(int i = 0; i < NUMERO_ARCHIVOS; ++i) {
        lectores[i].join();
    }

    return 0;   
}

void maestroLector(std::string archivo, int cantidadTrabajadores, int estrategia) {
    std::ifstream input(archivo);
    std::string linea;
    int lineas = 0;

    while(std::getline(input, linea)) {     // Cuenta la cantidad total de líneas del archivo
        lineas++;
    }

    std::thread trabajadores[cantidadTrabajadores];
    FileReader *lector = new FileReader(archivo, cantidadTrabajadores, estrategia, lineas, mutex);

    for(int i = 0; i < cantidadTrabajadores; ++i) {
          trabajadores[i] = std::thread(contarEtiquetas, lector, i);
    }

    for(int i = 0; i < cantidadTrabajadores; ++i) {
        trabajadores[i].join();
    }

    input.close();
}

void contarEtiquetas(FileReader *lector, int trabajador) {
    std::string linea;

    while(lector->hasNext(trabajador)) {
        linea = lector->getNext(trabajador);
        mutexPrint.lock();
        std::cout << "Trabajador #" << trabajador << ": " << linea << "\n";
        mutexPrint.unlock();
    }
}