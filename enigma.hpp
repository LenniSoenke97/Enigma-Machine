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
  void setPlugboard(Plugboard* plugboard) { this->plugboard = plugboard; };
  void setReflector(Reflector* reflector) { this->reflector = reflector; };
  void setRotor(Rotor* rotor) {    
    rotors[number_of_rotors] = rotor;
    number_of_rotors++;
  };
  void convert(char* input_char);
};

#endif
