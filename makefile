enigma: main.o enigma.o
	g++ main.o enigma.o -o enigma

main.o: main.cpp errors.h enigma.hpp
	g++ -Wall -g -c main.cpp

enigma.o: enigma.cpp errors.h enigma.hpp
	g++ -Wall -g -c enigma.cpp
