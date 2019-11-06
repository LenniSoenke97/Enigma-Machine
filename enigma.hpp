#ifndef ENIGMA
#define ENIGMA
#include<iostream>
#include<fstream>
#include"errors.h"

using namespace std;

/*
 * Plugboard Class
 */
class Plugboard {

  int config_file_integers[26];
  int config_int_count = 0;
  ifstream config_file;

public:
  /*
   * Name: Plugboard constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus plugboard) is valid
   * Input: file path to plugboard config file
   * Output: initialised plugboard
   */
  int config(string config_file_path) {
    size_t found = config_file_path.find_last_of("/\\");
    string config_file_name = config_file_path.substr(found+1);
    config_file.open(config_file_path);
    int current_int;

    // check file can be opened
    if (!config_file) {
      cerr << "ERROR_OPENING_CONFIGURATION_FILE" << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
       
    while(config_file.good()) {
      // check if we can fetch next int
      if (!(config_file >> current_int)) {
	if (config_file.eof()) break;
	cerr << "Non-numeric character in plugboard file " << config_file_name << endl;
	return NON_NUMERIC_CHARACTER;
      }
      // check if int is within range
      if (current_int < 0 || 25 < current_int) {
	cerr << "Invalid Index used in plugboard file " << config_file_name << endl;
	return INVALID_INDEX;
      }
      // check mapping has not been used before
      for(int i; i < config_int_count; i++) {
	if (current_int == config_file_integers[i]) {
	  cerr << "Impossible plugboard configuration found in plugboard file" << config_file_name << endl;
	  return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	}
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;
    }

    // check plugboard configurations are even and below 26
    if((config_int_count%2) || (config_int_count > 26)) {
      cerr << "Incorrect number of parameters in plugboard file " << config_file_name << endl;
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

/*
 * Rotor Class
 */
class Rotor {
  int rotate_notches[25];
  int rotator_notch_number = 0;
  int config_file_offsets[26];
  int rotor_mapping[26];
  int config_int_count = 0;
  int current_pos = 0;
  ifstream config_file;
  ifstream starting_config;
  
public:
  /*
   * Name: Rotor constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_offsets array, furthermore it checks whether the file (and thus the rotor) is valid
   * Input: file path to rotor config file
   * Output: initialised rotor
   */
  int config(string config_file_path, string starting_pos_config_file_path, int rotor_pos) {
    size_t found = config_file_path.find_last_of("/\\");
    string config_file_name = config_file_path.substr(found+1);
    found = starting_pos_config_file_path.find_last_of("/\\");
    string starting_pos_config_file_name = starting_pos_config_file_path.substr(found+1);    
    config_file.open(config_file_path);
    starting_config.open(starting_pos_config_file_path);

    // Check config file opens
    if (!config_file) {
      cerr <<  "Error occured whilst opening rotor file: " << config_file_name << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    // Check starting position file opens
    if (!starting_config) {
      cerr << "Error occured whilst opening rotor position file: " << starting_pos_config_file_name << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    
    int current_int;

    while(config_file.good()) {
       // check if we can fetch next int
      if (!(config_file >> current_int)) {
	if (config_file.eof()) break;
        cerr << "Non-numeric character for mapping in rotor file " << config_file_name << endl;
        return NON_NUMERIC_CHARACTER;
      }
      // check int is within range
      if (current_int < 0 || 25 < current_int) {
        cerr << "Invalid Index used in rotor file " << config_file_name << endl;
        return INVALID_INDEX;
      }
      // check if next int is a notch or a rotor position
      if (config_int_count >= 26) {	
        rotate_notches[rotator_notch_number] = current_int;
	rotator_notch_number++;
	continue;
      }
      // check that rotor position has not already been mapped to
      for(int i=0; i < config_int_count; i++) {
        if (current_int == rotor_mapping[i]) {
          cerr << "Invalid mapping of input " << config_int_count << " to output " << current_int << " (output " << current_int << " is already mapped to from input " << i << ") in rotor file " << config_file_name << endl;
          return INVALID_ROTOR_MAPPING;
        }
      }

      rotor_mapping[config_int_count] = current_int;
      config_file_offsets[config_int_count] = current_int - config_int_count;
      config_int_count++;
    }

    // check every rotor position has been mapped to
    if (config_int_count < 26) {
      cerr << "Not all inputs mapped in rotor file: " << config_file_name << endl;
      return INVALID_ROTOR_MAPPING;
    }

    int starting_pos;
    for (int i=0; i<=rotor_pos; i++) {
      // check if we can fetch next in
      if(!(starting_config >> starting_pos)) {
	// check whether we have reached the end of the file
	if (starting_config.eof()) {
	  cerr << "No starting position for rotor " << rotor_pos << " in rotor position file: " << starting_pos_config_file_name << endl;
	  return NO_ROTOR_STARTING_POSITION;
	}
	cerr << "Non-numeric character in rotor positions file " << starting_pos_config_file_name << endl;
        return NON_NUMERIC_CHARACTER;
       
      }
    }

    // check if we should rotate
    if (starting_pos > 0) {
      this->rotate(starting_pos);
    }


    int next_pos;
    // check if there is an invalid character at the end of the starting position file
    if(!(starting_config >> next_pos)) {
      if (starting_config.eof()) return 0;
      cerr << "Non-numeric character in rotor positions file " << starting_pos_config_file_name << endl;
      return NON_NUMERIC_CHARACTER;
    }

    return 0;
  }

  /*
   *
   * THE BELOW MUST BE DESCRIBED
   *
   */
  void remap(); // make private
  void convert_forward(char* input_char);
  void convert_backward(char* input_char);
  void rotate(int by_positions=1);
  bool at_rotation_notch() {
    for(int current_notch=0; current_notch < rotator_notch_number; current_notch++) {
      if ((current_pos+1) == rotate_notches[current_notch]) return true;
    }
    return false;
  };

};

class Reflector {
  int config_file_integers[26];
  int config_int_count = 0;
  ifstream config_file;
  
public:
  /*
   * Name: Reflector constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus the reflector) is valid
   * Input: file path to reflector config file
   * Output: initialised reflector
   */
  int config(string config_file_path) {
    size_t found = config_file_path.find_last_of("/\\");
    string config_file_name = config_file_path.substr(found+1);
    config_file.open(config_file_path);
    if (!config_file) {
      cerr <<  "Error occured whilst opening rotor file: " << config_file_name << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    int current_int;

    while(config_file.good()) {
      if (!(config_file >> current_int)) {
        if (config_file.eof()) break;
        cerr << "Non-numeric character in reflector file " << config_file_name << endl;
        return NON_NUMERIC_CHARACTER;
      }

      if (current_int < 0 || 25 < current_int) {
        cerr << "Invalid Index used in reflector file " << config_file_name << endl;
        return INVALID_INDEX;
      }
      for(int i; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
          cerr << "Invalid reflector mapping in reflector file " << config_file_name << endl;
          return INVALID_REFLECTOR_MAPPING;
        }
      }
      config_file_integers[config_int_count] = current_int;
      config_int_count++;

    }

    if(config_int_count % 2) {
      cerr << "Incorrect (odd) number of parameters in reflector file " << config_file_name << endl;
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
    
    if(config_int_count != 26) {
      cerr << "Insufficient number of mappings in reflector file: " << config_file_name << endl;
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
