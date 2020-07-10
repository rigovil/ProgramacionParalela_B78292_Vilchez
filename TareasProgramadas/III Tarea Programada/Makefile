all: primosSerial primosPThread primosOMP primosMPI

primosSerial: chrono.cc primosSerial.cpp
	g++ primosSerial.cpp -w chrono.cc -o primosSerial

primosPThread: chrono.cc primosPThread.cpp
	g++ primosPThread.cpp -w chrono.cc -o primosPThread -lpthread

primosOMP: chrono.cc primosOpenMP.cpp
	g++ primosOpenMP.cpp -w chrono.cc -o primosOMP -fopenmp

primosMPI: chrono.cc primosMPI.cpp
	mpic++ primosMPI.cpp -w chrono.cc -o primosMPI
