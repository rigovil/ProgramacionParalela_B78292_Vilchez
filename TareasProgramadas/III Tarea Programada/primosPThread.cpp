#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include "chrono.h"

#define NUM_HILOS 4

bool esPrimo(int primo) {
	for(int i = primo/2; i > 2; i--) {
		if (primo%i == 0) {
			return false;
        }
    }

	return true;
}

void primosParalelo(int inicio, int fin) {

    if(inicio == 2) {
        inicio = 6;
    }

    for(int n = inicio; n <= fin; ++n) {
        bool sumaEncontrada = false;
        if(n%2 == 0) {
            for(int k = 3; k <= n && !sumaEncontrada; k++)   {
                if (esPrimo(k) && esPrimo(n-k)) {
                    sumaEncontrada = true;
                    // printf("%d = %d + %d\n", n, k, n-k);
                }
            }
        }
    }
}

int main() {
    Chrono inicio, fin;
    std::thread hilos[NUM_HILOS];

    int limite, proceso, primero, ultimo;
    std::cout << "Ingrese el limite superior: " << "\n";
    std::cin >> limite;

    while(limite < 4 || limite % 2 != 0) {
        std::cout << "Ingrese el limite superior: " << "\n";
        std::cin >> limite;
    }

    int incremento = limite/NUM_HILOS;

    inicio.readTime();
    for(proceso = 0; proceso < NUM_HILOS; proceso++) {
        primero = (proceso * incremento) + 1;
        ultimo = (proceso + 1) * incremento;
        hilos[proceso] = std::thread(primosParalelo, primero+1, ultimo);
    }

    for(int i = 0; i < NUM_HILOS; ++i) {
        hilos[i].join();
    }
    fin.readTime();

    fin-= inicio;
    printf("Tiempo: %lds, %ldns\n", fin.getSecs(), fin.getnSecs());

    return 0;
}