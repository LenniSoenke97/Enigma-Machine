enigma: main.o enigma.o plugboard.o rotor.o reflector.o processor.o 
	g++ main.o enigma.o plugboard.o rotor.o reflector.o -o enigma

main.o: main.cpp errors.h enigma.hpp
	g++ -Wall -Wextra -g -c main.cpp

enigma.o: enigma.cpp plugboard.hpp rotor.hpp reflector.hpp enigma.hpp 
	g++ -Wall -Wextra -g -c enigma.cpp

plugboard.o: plugboard.cpp plugboard.hpp processor.hpp
	g++ -Wall -Wextra -g -c plugboard.cpp

rotor.o: rotor.cpp rotor.hpp processor.hpp
	g++ -Wall -Wextra -g -c rotor.cpp

reflector.o: reflector.cpp reflector.hpp processor.hpp
	g++ -Wall -Wextra -g -c reflector.cpp

processor.o: errors.h processor.hpp
	g++ -Wall -Wextra -g -c processor.hpp
