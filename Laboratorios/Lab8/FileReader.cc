#include "FileReader.h"
#include <thread>

#define MAPEO_BLOQUES 1
#define MAPEO_CICLICO 2
#define MAPEO_DINAMICO 3
#define MAPEO_PERSONALIZADO 4

FileReader :: FileReader(std::string nombreArchivo, int trabajadores, int estrategia, int total_lineas, std::mutex *mutex) {
    this->nombreArchivo = nombreArchivo;
    this->trabajadores = trabajadores;
    this->estrategia = estrategia;
    this->total_lineas = total_lineas;
    this->siguienteLinea = new int[trabajadores];
    this->mutex = mutex;

    switch(estrategia) {
        
        case MAPEO_BLOQUES:
            for(int i = 0; i < trabajadores; ++i) {
                siguienteLinea[i] = (ceil((double)total_lineas/(double)trabajadores)*i + 1);
            }
            break;

        case MAPEO_CICLICO:
            for(int i = 0; i < trabajadores; ++i) {
                siguienteLinea[i] = (i+1);
            }
            break;
        
        case MAPEO_PERSONALIZADO:
            for(int i = 0; i < trabajadores; ++i) {
                siguienteLinea[i] = (total_lineas-i);
            }
    }
}


FileReader :: ~FileReader() {
    delete siguienteLinea;
}

bool FileReader :: hasNext(int trabajador) {
    bool puedeLeer;

    switch(estrategia) {

        case MAPEO_BLOQUES:
            puedeLeer = siguienteLinea[trabajador] <= total_lineas && siguienteLinea[trabajador] < (ceil((double)total_lineas/(double)trabajadores)*(trabajador+1) + 1);
            break;
        
        case MAPEO_CICLICO:
            puedeLeer = siguienteLinea[trabajador] <= total_lineas;
            break;

        case MAPEO_DINAMICO:
            puedeLeer = lineasLeidas <= total_lineas;
            break;

        case MAPEO_PERSONALIZADO:
            puedeLeer = siguienteLinea[trabajador] > 0;
            break;
    }

    return puedeLeer;
}

std::string FileReader :: getNext(int trabajador) {
    std::ifstream input(nombreArchivo);
    std::string linea;

    switch(estrategia) {

        case MAPEO_BLOQUES: {
            for(int i = 1; i <= siguienteLinea[trabajador]; ++i) {
                std::getline(input, linea);
            }
            ++siguienteLinea[trabajador];
            break;
        }

        case MAPEO_CICLICO: {
            for(int i = 1; i <= siguienteLinea[trabajador]; ++i) {
                std::getline(input, linea);
            }
            siguienteLinea[trabajador]+= trabajadores;
            break;
        }

        case MAPEO_DINAMICO: {
            mutex->lock();
            int miLinea = ++lineasLeidas;
            mutex->unlock();
            
            if(miLinea <= total_lineas) {
                for(int i = 1; i <= miLinea; ++i) {
                    std::getline(input,linea);
                }
            }
            break;
        }

        case MAPEO_PERSONALIZADO: {
            for(int i = 1; i <= siguienteLinea[trabajador]; ++i) {
                std::getline(input, linea);
            }
            siguienteLinea[trabajador]-= trabajadores;
        }
    }

    input.close();
    return linea;
}