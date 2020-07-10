#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "chrono.h"
#include <mpi.h>

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

    if(n%2 == 0 && n > 4) {
        for(int k = 3; k <= n && !sumaEncontrada; k+=2)   {
            if (esPrimo(k) && esPrimo(n-k)) {
                sumaEncontrada = true;
                // std::cout << n << " = " << k << " + " << n-k << std::endl;
            }
        }
    }
}

int main(int argc, char **argv) {
    int inc, done = 0, limite, my_rank, comm_sz, local_a, local_b;
    Chrono inicio, fin;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank == 0) {
        std::cout << "Ingrese el limite superior: " << "\n";
        std::cin >> limite;

        while(limite < 4 || limite % 2 != 0) {
            std::cout << "Ingrese el limite superior: " << "\n";
            std::cin >> limite;
        }
    }

    inicio.readTime();

    if(my_rank != 0) {
        MPI_Recv(&local_a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&local_b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for(int i = local_a; i <= local_b; ++i) {
            primosSerial(i);
        }

        done = 1;
        MPI_Send(&done, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        inc = limite/comm_sz;
        for(int i = 1; i < comm_sz; ++i) {
            local_a = (i*inc) + 1;
            local_b = (i+1) * inc;
            MPI_Send(&local_a, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&local_b, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        local_a = 4;
        local_b = inc;

        for(int i = local_a; i <= local_b; ++i) {
            primosSerial(i);
        }
        
        for(int i = 1; i < comm_sz; ++i) {
            MPI_Recv(&done, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        fin.readTime();
        fin-= inicio;

        printf("Tiempo: %ds, %dns\n", fin.getSecs(), fin.getnSecs());
    }

    MPI_Finalize();
    
    return 0;
}