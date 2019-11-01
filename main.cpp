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
  if(argc < 4) {
    std::cout << "INSUFFICIENT_NUMBER_OF_PARAMETERS" << std::endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
  std::string plugboard_config = argv[1], reflector_config = argv[2];
  int number_of_rotors = argc - 4; // argv[0]; argv[1]; argv[2]; argv[argc-1];
  std::string rotor_configs[number_of_rotors];
  for (int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
    rotor_configs[current_rotor] = argv[current_rotor+3];
  }

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
  std::string rotor_starting_pos = argv[argc-1];
  for (int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
    Rotor* rotor = new Rotor(rotor_configs[current_rotor], rotor_starting_pos, current_rotor);
    enigma_machine->setRotor(rotor);
  }
 

  //
  // init reflector
  //
  Reflector* reflector = new Reflector(reflector_config);
  enigma_machine->setReflector(reflector);
  
  //
  // input -> output
  //
  char output[1024];
  int output_length = 0;
  char current_char = '1';
  while(current_char != '0') {
    std::cin >> std::ws >>current_char;
    if (current_char == '0') continue;
    if ('A' <= current_char && current_char <= 'Z') {
      enigma_machine->convert(&current_char);
      std::cout << current_char;
      output[output_length] = current_char;
      output_length++;
      
    }
    // return INVALID_INPUT_CHARACTER;
    
  }
  // output
  // for (int index=0; index<output_length; index++) { std::cout << output[index]; };
}
