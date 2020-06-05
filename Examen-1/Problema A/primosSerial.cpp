#include <stdlib.h>
#include <stdio.h>
#include <iostream>


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
            std::cout << n << " = " << k << " + " << n-k << std::endl;
        }
    }
}

int main() {
    clock_t start = clock();
    int limite;
    std::cout << "Ingrese el limite superior: " << "\n";
    std::cin >> limite;

    while(limite < 4 || limite % 2 != 0) {
        std::cout << "Ingrese el limite superior: " << "\n";
        std::cin >> limite;
    }

    for(int i = 6; i <= limite; i = i+2) {
        primosSerial(i);
    }
    clock_t end = clock();

    double time = (double)(end-start) / CLOCKS_PER_SEC;

    printf("Tiempo: %f\n", time);
    return 0;
}