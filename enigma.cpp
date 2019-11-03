//#include"errors.h"
#include"enigma.hpp"
#include<fstream>
#include<iostream>

void Plugboard::convert(char* input_char) {
  int input_int = static_cast<int>(*input_char) - 65;
  for(int current_int_index=0; current_int_index < config_int_count; current_int_index++) {  
    if (config_file_integers[current_int_index] == input_int) {
      int output_int = (current_int_index % 2) ? config_file_integers[current_int_index-1] : config_file_integers[current_int_index+1];
      *input_char = static_cast<char>(output_int + 65);
      return;
    }
    
  }
}

void Rotor::convert_forward(char* input_char) {	
  int input_int = static_cast<int>(*input_char) - 65;
  int output_int = config_file_integers[input_int];
  *input_char = static_cast<char>(output_int + 65);
}

void Rotor::convert_backward(char* input_char) {
  int input_int = static_cast<int>(*input_char) - 65;
  int input_index=0;
  for(;;input_index++) {
    if(input_int == config_file_integers[input_index]) break;
  }
  *input_char = static_cast<char>(input_index + 65);
}

void Rotor::rotate(int by_positions) {
  current_pos += by_positions;
  int new_config_file_integers[26], new_config_file_index;
  for(int old_config_file_index=0;old_config_file_index<26;old_config_file_index++) {
    new_config_file_index = old_config_file_index+by_positions;
    if (new_config_file_index > 25) {
      new_config_file_index -= 26;
    }
    new_config_file_integers[new_config_file_index] = config_file_integers[old_config_file_index];
  }

  for (int index=0; index < 26; index++) {
    config_file_integers[index] = new_config_file_integers[index];
  }
};  

void Reflector::convert(char* input_char) {
  int input_int = static_cast<int>(*input_char) - 65;
  for(int current_int_index=0; current_int_index < config_int_count; current_int_index++) {
    if (config_file_integers[current_int_index] == input_int) {
      int output_int = (current_int_index % 2) ? config_file_integers[current_int_index-1] : config_file_integers[current_int_index+1];
      *input_char = static_cast<char>(output_int + 65);
      return;
    }

  }
}

void EnigmaMachine::convert(char* input_char) {
  int rotors_to_rotate = 1;
  
  for(; rotors_to_rotate <= number_of_rotors; rotors_to_rotate++) {
    if(!((this->rotors[rotors_to_rotate-1])->at_rotation_notch())) break;
  }
  for(int current_rotor = 0; current_rotor < rotors_to_rotate && current_rotor < number_of_rotors; current_rotor++) {
    std::cout << " r ";
    (this->rotors[number_of_rotors-current_rotor-1])->rotate();
  }

  

  // Plugboard convert  
  plugboard->convert(input_char);

  // Rotor convert
  for(int current_rotor = (number_of_rotors-1); 0 <= current_rotor; current_rotor--) {
    (this->rotors[current_rotor])->convert_backward(input_char);
  }
 
  // Reflector convert
  reflector->convert(input_char);
  

  // Rotor convert
  for(int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
    (this->rotors[current_rotor])->convert_forward(input_char);
  }

 
  // Plugboard convert
  plugboard->convert(input_char);

  
  /*
  int rotors_to_rotate = 1;
  for(; rotors_to_rotate <= number_of_rotors; rotors_to_rotate++) {
    if(!((this->rotors[rotors_to_rotate-1])->at_rotation_notch())) break;
  }
  for(int current_rotor = (number_of_rotors-1); 0 <= current_rotor; current_rotor--) {
    (this->rotors[current_rotor])->rotate();
  }
  */
}
