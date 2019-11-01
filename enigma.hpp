#ifndef ENIGMA
#define ENIGMA
#include<fstream>
#include<iostream>
#include"errors.h"

/*
 * Plugboard Class
 */
class Plugboard {
  int config_file_integers[26];
  int config_int_count = 0;
  std::ifstream config;
public:
  /*
   * Name: Plugboard constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus plugboard) is valid
   * Input: file path to plugboard config file
   * Output: initialised plugboard
   */
  Plugboard(std::string config_file_path) {
    config.open(config_file_path);
    if (!config) {
      std::cout << "ERROR_OPENING_CONFIGURATION_FILE" << std::endl;
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    int current_int;
    
    while(config.good()) {
      if (config_int_count == 26) {
	std::cout << "INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS" << std::endl;
	throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
      }

      if (!(config >> current_int)) {
	if (config.eof()) break;
	std::cout << "NON_NUMERIC_CHARACTER" << std::endl;
	throw NON_NUMERIC_CHARACTER;
      }
      if (current_int < 0 || 25 < current_int) {
	std::cout << "INVALID_INDEX" << std::endl;
	throw INVALID_INDEX;
      }
      for(int i; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
	  std::cout << "IMPOSSIBLE_PLUGBOARD_CONFIGURATION" << std::endl;
	  throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
        }
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;
    }
    if(config_int_count%2) {
      std::cout << "INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS: " << config_int_count << std::endl;
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }


  };

  /*                                                                     
   * Name: Plugboard convert                                                                                       
   * Description: converts an input char into an output char. To do this it performs the normal plugboard action of "switching" letters that are connected by a plugboard wire.                                                  
   * Input: a pointer to an input char                                   
   * Output: void                                                                                 
   */
  void convert(char* input_char);
};

class Rotor {
  int rotate_notch_pos;
  int config_file_integers[26];
  int config_int_count = 0;
  int current_pos = 0;
  std::ifstream config;
  
public:
   /*
   * Name: Rotor constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus the rotor) is valid
   * Input: file path to rotor config file
   * Output: initialised rotor
   */
  Rotor(std::string config_file_path, std::string starting_pos_config_file_path, int rotor_pos) {
     config.open(config_file_path);
    std::ifstream starting_config(starting_pos_config_file_path);
    if (!config || !starting_config) {
      std::cout << "ERROR_OPENING_CONFIGURATION_FILE" << std::endl;
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    
    int current_int;

    while(config.good() && config_int_count <= 26) {
      if (!(config >> current_int)) {
        std::cout << "NON_NUMERIC_CHARACTER" << std::endl;
        throw NON_NUMERIC_CHARACTER;
      }
      if (current_int < 0 || 25 < current_int) {
        std::cout << "INVALID_INDEX" << std::endl;
        throw INVALID_INDEX;
      }
      
      if (config_int_count == 26) {
        config >> rotate_notch_pos;
        break;
      }

      for(int i; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
          std::cout << "INVALID_ROTOR_MAPPING: " << current_int << " and " << config_file_integers[i] << std::endl;
          throw INVALID_ROTOR_MAPPING;
        }
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;
    }
    if(config_int_count != 26) {
      std::cout << "INAVLID_ROTOR_MAPPING" << std::endl;
      throw INVALID_ROTOR_MAPPING;
    }

    int starting_pos;
    for (int i=0; i<=rotor_pos; i++) {
      if(!(starting_config >> starting_pos)) {
        std::cout << "NO_ROTOR_STARTING_POSITION" << std::endl;
        throw NO_ROTOR_STARTING_POSITION;
      }
    }
    if (starting_pos == 0) return;
    this->rotate(starting_pos);

  }

  void convert_forward(char* input_char);
  void convert_backward(char* input_char);

  void rotate(int by_positions=1);
  bool at_rotation_notch() { return (current_pos == rotate_notch_pos); };



   void display_rotor() { ///// DELETE THIS
    for(int index=0; index < 26; index++) {
      std::cout << config_file_integers[index] << " ";
    }
  }
};

class Reflector {
  int config_file_integers[26];
  int config_int_count = 0;
  std::ifstream config;
  
public:
   /*
   * Name: Reflector constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus the reflector) is valid
   * Input: file path to reflector config file
   * Output: initialised reflector
   */
  Reflector(std::string config_file_path) {
    config.open(config_file_path);
    if (!config) {
      std::cout << "ERROR_OPENING_CONFIGURATION_FILE" << std::endl;
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    int current_int;

    while(config.good()) {
      if (!(config >> current_int)) {
        if (config.eof()) break;
        std::cout << "NON_NUMERIC_CHARACTER" << std::endl;
        throw NON_NUMERIC_CHARACTER;
      }

      if (current_int < 0 || 25 < current_int) {
        std::cout << "INVALID_INDEX" << std::endl;
        throw INVALID_INDEX;
      }
      for(int i; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
          std::cout << "INVALID_REFLECTOR_MAPPING" << std::endl;
          throw INVALID_REFLECTOR_MAPPING;
        }
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;
    }
    
    if(config_int_count != 26) {
      std::cout << "INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS: " << config_int_count << std::endl;
      throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
  };

  void convert(char* input_char);
};


class EnigmaMachine {
  Plugboard* plugboard;
  Rotor* rotors[100];
  int number_of_rotors = 0;
  Reflector* reflector;

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
