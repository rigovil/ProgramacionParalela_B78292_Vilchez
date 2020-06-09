lector: main.o FileReader.o
	g++ -g main.o FileReader.o -o lector -lpthread

main.o: main.cc
	g++ -c -g main.cc -lpthread

FileReader.o: FileReader.cc FileReader.h
	g++ -c -g FileReader.cc -lpthread