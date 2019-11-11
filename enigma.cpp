#include"plugboard.hpp"
#include"rotor.hpp"
#include"reflector.hpp"
#include"enigma.hpp"

int EnigmaMachine::setPlugboard(string plugboard_config) {
  int error_code;
  plugboard = new Plugboard();
  return plugboard->config(plugboard_config);
  return error_code;
}

int EnigmaMachine::setReflector(string reflector_config) {
  int error_code;
  reflector = new Reflector();
  error_code = reflector->config(reflector_config);
  return error_code;
}

int EnigmaMachine::setRotor(string rotor_config, 
		string starting_pos_config, 
		int rotor_pos) {
  int error_code;
  Rotor* rotor = new Rotor();
  error_code = rotor->config(rotor_config, starting_pos_config, rotor_pos);
  this->rotors[number_of_rotors] = rotor;
  number_of_rotors++;
  return error_code;
}

void EnigmaMachine::rotateRotors() {
  int rotors_to_rotate = 1;
  for(; rotors_to_rotate <= number_of_rotors; rotors_to_rotate++) {
    if(!((this->rotors[number_of_rotors-rotors_to_rotate])
			    ->at_rotation_notch())) break;
  }
  for(int current_rotor = 0; 
		  current_rotor < rotors_to_rotate && 
		  current_rotor < number_of_rotors; 
		  current_rotor++) {
    (this->rotors[number_of_rotors-current_rotor-1])->rotate();

  }
}

void EnigmaMachine::convert(char* input_char) {
  this->rotateRotors();

  int input_int = static_cast<int>(*input_char) - 65;
     
  // Plugboard convert  
  plugboard->convert(&input_int);

  // Rotor convert
  for(int current_rotor = (number_of_rotors-1); 
		  0 <= current_rotor; 
		  current_rotor--) {
    (this->rotors[current_rotor])->convert_forward(&input_int);
  }
  
  // Reflector convert
  reflector->convert(&input_int);				     
				      
  // Rotor convert
  for(int current_rotor = 0; 
		  current_rotor < number_of_rotors; 
		  current_rotor++) {
    (this->rotors[current_rotor])->convert_backward(&input_int);
  }
 
  // Plugboard convert
  plugboard->convert(&input_int);

  *input_char =  static_cast<char>(input_int + 65);

}
