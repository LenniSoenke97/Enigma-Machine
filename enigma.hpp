#ifndef ENIGMA
#define ENIGMA
#include<fstream>
#include<iostream>
#include"errors.h"

//std::string FindFileName(const std::string& str)
//{
  
//}

/*
 * Plugboard Class
 */
class Plugboard {
  int config_file_integers[26];
  int config_int_count = 0;
  std::ifstream config_file;
public:
  /*
   * Name: Plugboard constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus plugboard) is valid
   * Input: file path to plugboard config file
   * Output: initialised plugboard
   */
  int config(std::string config_file_path) {
     size_t found = config_file_path.find_last_of("/\\");
    std::string config_file_name = config_file_path.substr(found+1);
    config_file.open(config_file_path);
    if (!config_file) {
      std::cerr << "ERROR_OPENING_CONFIGURATION_FILE" << std::endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    int current_int;
    
    while(config_file.good()) {
      if (!(config_file >> current_int)) {
	if (config_file.eof()) break;
	std::cerr << "Non-numeric character in plugboard file " << config_file_name << std::endl;
	return NON_NUMERIC_CHARACTER;
      }
      if (current_int < 0 || 25 < current_int) {
	std::cerr << "Invalid Index used in plugboard file " << config_file_name << std::endl;
	return INVALID_INDEX;
      }
      for(int i; i < config_int_count; i++) {
	if (current_int == config_file_integers[i]) {
	  std::cerr << "Impossible plugboard configuration found in plugboard file" << config_file_name << std::endl;
	  return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	}
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;
    }
    if((config_int_count%2) || (config_int_count > 26)) {
      std::cerr << "Incorrect number of parameters in plugboard file " << config_file_name << std::endl;
      return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }

    return 0;

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
  int rotate_notches[25];
  int rotator_notch_number = 0;
  int config_file_integers[26];
  int config_int_count = 0;
  int current_pos = 0;
  std::ifstream config_file;
  std::ifstream starting_config;
  
public:
  /*
   * Name: Rotor constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus the rotor) is valid
   * Input: file path to rotor config file
   * Output: initialised rotor
   */
  int config(std::string config_file_path, std::string starting_pos_config_file_path, int rotor_pos) {
     size_t found = config_file_path.find_last_of("/\\");
    std::string config_file_name = config_file_path.substr(found+1);
     found = starting_pos_config_file_path.find_last_of("/\\");
    std::string starting_pos_config_file_name = config_file_path.substr(found+1);
    
    config_file.open(config_file_path);
    starting_config.open(starting_pos_config_file_path);
    if (!config_file || !starting_config) {
      std::cerr <<  "Error occured whilst opening rotor file: " << config_file_name << std::endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    if (!starting_config) {
      std::cerr << "Error occured whilst opening rotor position file: " << starting_pos_config_file_name << std::endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    
    int current_int;

    while(config_file.good()) {
      if (!(config_file >> current_int)) {
	if (config_file.eof()) break;
        std::cerr << "Non-numeric character for mapping in rotor file " << config_file_name << std::endl;
        return NON_NUMERIC_CHARACTER;
      }

     if (current_int < 0 || 25 < current_int) {
        std::cerr << "Invalid Index used in rotor file " << config_file_name << std::endl;
        return INVALID_INDEX;
      }
      
      if (config_int_count >= 26) {	
        rotate_notches[rotator_notch_number] = current_int;
	rotator_notch_number++;
	continue;
      }

      for(int i=0; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
          std::cerr << "Invalid mapping of input " << config_int_count << " to output " << current_int << " (output " << current_int << " is already mapped to from input " << i << ") in rotor file " << config_file_name << std::endl;
          return INVALID_ROTOR_MAPPING;
        }
      }
      
      config_file_integers[config_int_count] = current_int;
      config_int_count++;
    }

    if (config_int_count < 26) {
      std::cerr << "Not all inputs mapped in rotor file: " << config_file_name << std::endl;
      return INVALID_ROTOR_MAPPING;
    }

    int starting_pos;
    for (int i=0; i<=rotor_pos; i++) {
      if(!(starting_config >> starting_pos)) {
	if (starting_config.eof()) {
	  std::cerr << "No starting position for rotor " << rotor_pos << " in rotor position file: " << starting_pos_config_file_name << std::endl;
	  return NO_ROTOR_STARTING_POSITION;
	}
	std::cerr << "Non-numeric character in rotor positions file " << starting_pos_config_file_name << std::endl;
        return NON_NUMERIC_CHARACTER;
       
      }
    }

    //if (starting_pos > 0) {
      //this->rotate(starting_pos);
    this->current_pos = starting_pos;
	//}


    int next_pos;
    if(!(starting_config >> next_pos)) {
      if (starting_config.eof()) return 0;
      std::cerr << "Non-numeric character in rotor positions file " << starting_pos_config_file_name << std::endl;
        return NON_NUMERIC_CHARACTER;
    }

    return 0;
  }

  void convert_forward(char* input_char);
  void convert_backward(char* input_char);

  void rotate(int by_positions=1);
  bool at_rotation_notch() {
    for(int current_notch=0; current_notch < rotator_notch_number; current_notch++) {
      if (current_pos == rotate_notches[current_notch]) return true;
    }
    return false;
  };

  void display_rotor() { ///// DELETE THIS
    for(int index=0; index < 26; index++) {
      std::cerr << " " << config_file_integers[index];
    }
    // std::cout << " current pos: " << current_pos << " - notches: " << rotator_notch_number << "! ";
  }
};

class Reflector {
  int config_file_integers[26];
  int config_int_count = 0;
  std::ifstream config_file;
  
public:
  /*
   * Name: Reflector constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus the reflector) is valid
   * Input: file path to reflector config file
   * Output: initialised reflector
   */
  int config(std::string config_file_path) {
    size_t found = config_file_path.find_last_of("/\\");
    std::string config_file_name = config_file_path.substr(found+1);
    config_file.open(config_file_path);
    if (!config_file) {
      std::cerr <<  "Error occured whilst opening rotor file: " << config_file_name << std::endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    int current_int;

    while(config_file.good()) {
      if (!(config_file >> current_int)) {
        if (config_file.eof()) break;
        std::cerr << "Non-numeric character in reflector file " << config_file_name << std::endl;
        return NON_NUMERIC_CHARACTER;
      }

      if (current_int < 0 || 25 < current_int) {
        std::cerr << "Invalid Index used in reflector file " << config_file_name << std::endl;
        return INVALID_INDEX;
      }
      for(int i; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
          std::cerr << "Invalid reflector mapping in reflector file " << config_file_name << std::endl;
          return INVALID_REFLECTOR_MAPPING;
        }
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;

    }

    if(config_int_count % 2) {
      std::cerr << "Incorrect (odd) number of parameters in reflector file " << config_file_name << std::endl;
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
    
    if(config_int_count != 26) {
      std::cerr << "Insufficient number of mappings in reflector file: " << config_file_name << std::endl;
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }

    return 0;
  };

  void convert(char* input_char);
};


class EnigmaMachine {
  Plugboard* plugboard;
  Reflector* reflector;
  Rotor* rotors[100];
  int number_of_rotors = 0;

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
