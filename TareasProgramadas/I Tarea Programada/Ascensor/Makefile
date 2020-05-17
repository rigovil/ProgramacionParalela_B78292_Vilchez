programada: main.o Ascensor.o Persona.o
	g++ -g main.o Ascensor.o Persona.o -o ascensor -lpthread

main.o: main.cc
	g++ -c -g main.cc -lpthread

Ascensor.o: Ascensor.cc Ascensor.h
	g++ -c -g Ascensor.cc -lpthread

Persona.o: Persona.cc Persona.h
	g++ -c -g Persona.cc -lpthread