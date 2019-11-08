enigma: main.o enigma.o plugboard.o rotor.o reflector.o
	g++ main.o enigma.o plugboard.o rotor.o reflector.o -o enigma

main.o: main.cpp errors.h enigma.hpp
	g++ -Wall -Wextra -g -c main.cpp

enigma.o: enigma.cpp plugboard.hpp errors.h enigma.hpp 
	g++ -Wall -Wextra -g -c enigma.cpp

plugboard.o: plugboard.cpp errors.h plugboard.hpp
	g++ -Wall -Wextra -g -c plugboard.cpp

rotor.o: rotor.cpp errors.h rotor.hpp
	g++ -Wall -Wextra -g -c rotor.cpp

reflector.o: reflector.cpp errors.h reflector.hpp
	g++ -Wall -Wextra -g -c reflector.cpp
