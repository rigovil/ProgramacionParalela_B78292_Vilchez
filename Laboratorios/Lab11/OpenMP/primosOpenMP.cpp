#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "chrono.h"

#define NUM_HILOS 8

bool esPrimo(int primo) {
	for(int i = primo/2; i > 2; i--) {
		if (primo%i == 0) {
			return false;
        }
    }

	return true;
}


void primosSerial(int n) {
    bool sumaEncontrada = false;

    for(int k = 3; k <= n && !sumaEncontrada; k+=2)   {
        if (esPrimo(k) && esPrimo(n-k)) {
            sumaEncontrada = true;
            /* 
            *  Descomentar para mostrar
            *  los resultados en consola
            *  std::cout << n << " = " << k << " + " << n-k << std::endl;
            */
        }
    }
}

int main() {
    int limite;
    Chrono inicio, fin;
    
    std::cout << "Ingrese el limite superior: " << "\n";
    std::cin >> limite;

    while(limite < 4 || limite % 2 != 0) {
        std::cout << "Ingrese el limite superior: " << "\n";
        std::cin >> limite;
    }

    inicio.readTime();
#   pragma omp parallel for num_threads(NUM_HILOS) \
    schedule(static, 1)
    for(int i = 6; i <= limite; i+=2) {
        primosSerial(i);
    }
    fin.readTime();

    fin-= inicio;
    printf("Tiempo: %ds, %dns\n", fin.getSecs(), fin.getnSecs());
    
    return 0;
}