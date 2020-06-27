#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "chrono.h"

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

    for(int k = 3; k <= n && !sumaEncontrada; k++)   {
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
    for(int i = 6; i <= limite; i = i+2) {
        primosSerial(i);
    }
    fin.readTime();

    fin-= inicio;
    printf("Tiempo: %ld seg, %ld nseg\n", fin.getSecs(), fin.getnSecs());
    
    return 0;
}