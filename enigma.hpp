#ifndef ENIGMA
#define ENIGMA
#include<iostream>
#include<fstream>
#include"errors.h"
#include"plugboard.hpp"
#include"rotor.hpp"
#include"reflector.hpp"

using namespace std;



class EnigmaMachine {
  Plugboard* plugboard;
  Reflector* reflector;
  Rotor* rotors[100];
  int number_of_rotors = 0;
  void rotateRotors();

public:
  int setPlugboard(string plugboard_config) {
     int error_code;
    plugboard = new Plugboard();
    error_code = plugboard->config(plugboard_config);
    return error_code;
  };
  int setReflector(string reflector_config) {
     int error_code;
    reflector = new Reflector();
    error_code = reflector->config(reflector_config);
    return error_code;
  };
  int setRotor(string rotor_config, string starting_pos_config, int rotor_pos) {
    int error_code;
    Rotor* rotor = new Rotor();
    error_code = rotor->config(rotor_config, starting_pos_config, rotor_pos);
    this->rotors[number_of_rotors] = rotor;
    number_of_rotors++;
    return error_code;
  };
  void convert(char* input_char);
};

#endif
