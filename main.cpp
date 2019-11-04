#include"errors.h"
#include"enigma.hpp"
#include<iostream>
#include<fstream>

// QUESTIONS
// how do we raise errors
// when do we rotate (before signal comes back or after)



int main(int argc, char** argv) {
  int r;
  
  //
  // parse command line arguments
  //
  if(argc < 3) {
    std::cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?" << std::endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
  std::string plugboard_config = argv[1], reflector_config = argv[2];
  int number_of_rotors = argc - 4; // argv[0]; argv[1]; argv[2]; argv[argc-1];

  //
  // init enigma machine
  //
  EnigmaMachine* enigma_machine = new EnigmaMachine();

  //
  // init plugboard
  //
  Plugboard* plugboard = new Plugboard();
  r = plugboard->config(plugboard_config);
  if (r) return r;
  enigma_machine->setPlugboard(plugboard);

  //
  // init rotors
  //

  if(number_of_rotors > 0) {
    std::string rotor_starting_pos = argv[argc-1];

    for (int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
      Rotor* rotor = new Rotor();
      r = rotor->config(argv[current_rotor+3], rotor_starting_pos, current_rotor);
      if (r) return r;
      enigma_machine->setRotor(rotor);
    }
  }

  //
  // init reflector
  //
  Reflector* reflector = new Reflector();
  r = reflector->config(reflector_config);
  if (r) return r;
  enigma_machine->setReflector(reflector);
  
  //
  // input -> output
  //
  char current_char;
  while(std::cin >> std::ws >> current_char) {
    // std::cin >> std::ws >>current_char;
    if ('A' <= current_char && current_char <= 'Z') {
      enigma_machine->convert(&current_char);
      std::cout << current_char;
      continue;
    }
    std::cerr << "Invalid character";
    return INVALID_INPUT_CHARACTER;    
  }
 
  return 0;
}
