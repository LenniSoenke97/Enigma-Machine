#include"errors.h"
#include"enigma.hpp"
#include<iostream>
#include<fstream>

using namespace std;

int main(int argc, char** argv) {
  int error_code = 0;
  
  // parse command line arguments
  if(argc < 4) {
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
  string plugboard_config = argv[1], reflector_config = argv[2];
  int number_of_rotors = argc - 4;

  // initialisig enigma machine
  EnigmaMachine* enigma_machine = new EnigmaMachine();
  
  error_code = enigma_machine->setPlugboard(plugboard_config);
  if (error_code) return error_code;

  error_code = enigma_machine->setReflector(reflector_config);
  if (error_code) return error_code;

  if(number_of_rotors > 0) {
    string rotor_starting_pos = argv[argc-1];
    for (int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
      error_code = enigma_machine->setRotor(argv[current_rotor+3], rotor_starting_pos, current_rotor);
      if (error_code) return error_code;
    }
  }

  // input -> output
  char current_char;
  while(cin >> ws >> current_char) {
    if ('A' <= current_char && current_char <= 'Z') {
      enigma_machine->convert(&current_char);
      cout << current_char;
      continue;
    }
    cerr << current_char << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
    return INVALID_INPUT_CHARACTER;
  }

  delete enigma_machine;
  
  return 0;
}
