SumaUnoSem: SumaUnoSem.o Semaphore.o
	g++ -g SumaUnoSem.o Semaphore.o -o SumaUnoSem -lpthread

SumaUnoSem.o: SumaUnoSem.cc
	g++ -c -g SumaUnoSem.cc -lpthread

Semaphore.o: Semaphore.cc Semaphore.h
	g++ -c -g Semaphore.cc