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
  int setPlugboard(string plugboard_config);
  int setReflector(string reflector_config);
  int setRotor(string rotor_config, string starting_pos_config, int rotor_pos);
  void convert(char* input_char);

  ~EnigmaMachine() {
    delete plugboard;
    delete reflector;
    for (int rotor_index=0;rotor_index<number_of_rotors;rotor_index++) {
      delete rotors[rotor_index];
    }
  }
};

#endif
