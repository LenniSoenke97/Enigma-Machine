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
  int rtl_list[26], mapped_input;
  for (int i=0; i < 26; i++) {
    mapped_input = config_file_offsets[i] + i;
    if (mapped_input > 25) mapped_input -= 26;
    if (mapped_input < 0) mapped_input += 26;
    rtl_list[i] = mapped_input;
  }
  
  int input_int = static_cast<int>(*input_char) - 65;
  int output_int = rtl_list[input_int];
  *input_char = static_cast<char>(output_int + 65);
}

void Rotor::convert_backward(char* input_char) {
  int rtl_list[26], mapped_input;
  for (int i=0; i < 26; i++) {
    mapped_input = config_file_offsets[i] + i;
    if (mapped_input > 25) mapped_input -= 26;
    if (mapped_input < 0) mapped_input += 26;
    rtl_list[i] = mapped_input;
  }
  
  int input_int = static_cast<int>(*input_char) - 65;
  int input_index=0;
  for(;;input_index++) {
    if(input_int == rtl_list[input_index]) {
      *input_char = static_cast<char>(input_index + 65);
      return;
    }
  }
}

void Rotor::rotate(int by_positions) {
  current_pos += by_positions;
  if (current_pos > 25) current_pos = 26 - current_pos;
  int new_config_file_offsets[26], new_config_file_index;
  /*for(int old_config_file_index=0;old_config_file_index<26;old_config_file_index++) {
    new_config_file_index = old_config_file_index+by_positions;
    if (new_config_file_index > 25) {
      new_config_file_index -= 26;
    }
    new_config_file_offsets[new_config_file_index] = config_file_offsets[old_config_file_index];
    }*/
  
  
  for(int old_config_file_index=0;old_config_file_index<26;old_config_file_index++) {
    new_config_file_index = old_config_file_index-by_positions;
    if (new_config_file_index < 0) {
      new_config_file_index += 26;
    }
    new_config_file_offsets[new_config_file_index] = config_file_offsets[old_config_file_index];
    }

  for (int index=0; index < 26; index++) {
    config_file_offsets[index] = new_config_file_offsets[index];
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
    if(!((this->rotors[number_of_rotors-rotors_to_rotate])->at_rotation_notch())) break;
  }
  for(int current_rotor = 0; current_rotor < rotors_to_rotate && current_rotor < number_of_rotors; current_rotor++) {
    (this->rotors[number_of_rotors-current_rotor-1])->rotate();
    }

  // Plugboard convert  
  plugboard->convert(input_char);

  // Rotor convert
  for(int current_rotor = (number_of_rotors-1); 0 <= current_rotor; current_rotor--) {
    // (this->rotors[current_rotor])->convert_backward(input_char);
    (this->rotors[current_rotor])->convert_forward(input_char);
    //std::cout << "arot: " << *in;
    // std::cout << " rotor " << current_rotor << ":";
    // (this->rotors[current_rotor])->display_rotor();
  }
  
  // Reflector convert
  reflector->convert(input_char);
  // std::cout << "aref: " << *input_char << std::endl;
				     
				      
  // Rotor convert
  for(int current_rotor = 0; current_rotor < number_of_rotors; current_rotor++) {
    // (this->rotors[current_rotor])->convert_forward(input_char);
    (this->rotors[current_rotor])->convert_backward(input_char);
    //std::cout << "arot: " << *input_char << std::endl;
  }
 
  // Plugboard convert
  plugboard->convert(input_char);

}
