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

  /*
   * Main Attributes
   */
  Plugboard* plugboard;
  Reflector* reflector;
  Rotor* rotors[100];
  int number_of_rotors = 0;
  void rotateRotors();

public:
  /*
   * Description: sets and configurates that plugboard on the enigma machine
   * Input: path to the plugboard config file
   * Output: an error code (0 = no error; any other integer = error)
   */
  int setPlugboard(string plugboard_config);

   /*
   * Description: sets and configurates that plugboard on the enigma machine
   * Input: path to the plugboard config file
   * Output: an error code (0 = no error; any other integer = error)
   */
  int setReflector(string reflector_config);

   /*
   * Description: sets and configurates that plugboard on the enigma machine
   * Input: path to the plugboard config file
   * Output: an error code (0 = no error; any other integer = error)
   */
  int setRotor(string rotor_config, string starting_pos_config, int rotor_pos);

   /*
   * Description: converts an input char to an encryped output char by passing it through
   * its plugboard, rotors and reflector
   * Input: pointer to a char you want to convert
   * Output: void (input char gets changed since it is passed by reference)
   */
  void convert(char* input_char);

   /*
   * Description: Enigma Machine Destructor, deletes all enigma machine parts (the plugboard,
   * the reflector and all the rotors)
   * Input: none
   * Output: none
   */
  ~EnigmaMachine() {
    delete plugboard;
    delete reflector;
    for (int rotor_index=0;rotor_index<number_of_rotors;rotor_index++) {
      delete rotors[rotor_index];
    }
  }
};

#endif
