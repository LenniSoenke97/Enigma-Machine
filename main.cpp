#include"errors.h"
#include"enigma.hpp"
#include<iostream>
#include<fstream>

// QUESTIONS
// how do we raise errors
// when do we rotate (before signal comes back or after)



int main(int argc, char** argv) {
  //
  // parse command line arguments
  //
  if(argc < 4) {
    std::cout << "INSUFFICIENT_NUMBER_OF_PARAMETERS" << std::endl;
    throw INSUFFICIENT_NUMBER_OF_PARAMETERS;
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
  Plugboard* plugboard = new Plugboard(plugboard_config);
  enigma_machine->setPlugboard(plugboard);

  //
  // init rotors
  //
  std::string rotor_starting_pos = argv[argc-1];
  Rotor* rotors[number_of_rotors]; // delete
  for (int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
    Rotor* rotor = new Rotor(rotor_configs[current_rotor], rotor_starting_pos, current_rotor);
    rotors[current_rotor] = rotor; // delete
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
      //enigma_machine->convert(&current_char);



  plugboard->convert(&current_char);
  std::cout << "plugboad: " << current_char << std::endl;
  
  // Rotor convert
  for(int current_rotor = (number_of_rotors-1); 0 <= current_rotor; current_rotor--) {
    (rotors[current_rotor])->convert_forward(&current_char);
    std::cout << std::endl;
    rotors[current_rotor]->display_rotor();
     std::cout << std::endl;
    std::cout << "rotor: " << current_char << std::endl;
  }
   
  // Reflector convert
  reflector->convert(&current_char);
 
  // Rotor convert
  for(int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
    (rotors[current_rotor])->convert_backward(&current_char);
  }

  // Plugboard convert
  plugboard->convert(&current_char);
 
  int rotors_to_rotate = 1;
  for(; rotors_to_rotate <= number_of_rotors; rotors_to_rotate++) {
    if(!((rotors[rotors_to_rotate-1])->at_rotation_notch())) break;
  }
  for(int current_rotor = (number_of_rotors-1); 0 <= current_rotor; current_rotor--) {
    (rotors[current_rotor])->rotate();
  }

      
      output[output_length] = current_char;
      output_length++;
      
    }
    // display error INVALID_INPUT_CHARACTER
  }

  // output
  std::cout << std::endl << "Here is your output: ";
  for (int index=0; index<output_length; index++) { std::cout << output[index]; };
  std::cout << std::endl;
}
