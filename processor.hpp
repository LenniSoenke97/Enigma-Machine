#ifndef PROCESSOR
#define PROCESSOR

#include<fstream>
#include<iostream>
#include"errors.h"
#include<string>

using namespace std;

class Processor {
  string config_file_name;
  ifstream config_file;
  
  const int PLUGBOARD_MIN = 0;
  const int PLUGBOARD_MAX = 26;
  
  const int REFLECTOR_MIN = 26;
  const int REFLECTOR_MAX = 26;
  
  const int ROTOR_MIN = 26;
  const int ROTOR_MAX = 26;
  
public:
  enum file_type {plugboard, reflector, rotor, rotor_position};
  file_type type;

  void print_error(string error_msg) {
    string type_name;
    switch(type) {
    case plugboard:
      type_name = "plugboard";
      break;
    case reflector:
      type_name = "reflector";
      break;
    case rotor:
      type_name = "rotor";
      break;
    case rotor_position:
      type_name = "rotor position";
      break;
    default:
      break;
    }

    cerr << error_msg << " in " << type_name << " file: " << config_file_name << endl;
  }
  
  Processor(file_type type) : type(type) {};


  bool good() {
    return config_file.good() && !config_file.eof();
  };

  bool at_eof() {
    return config_file.eof();
  }

  int open(string file_path) {
    this->config_file.open(file_path);

    if (!config_file) {
      print_error("Error occued whilst opening config file");
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    
    size_t found = file_path.find_last_of("/\\");
    this->config_file_name = file_path.substr(found+1);

    return 0;
  };
  
  int get_next_int(int* integer) {
    if (!(this->config_file >> *integer)) {

      if (config_file.eof()) return 0;

      print_error("Non-numeric character");
      return NON_NUMERIC_CHARACTER;

    }
    
    if (*integer < 0 || 25 < *integer) {

      print_error("Invalid index");
      return INVALID_INDEX;

    }

    return 0;
  };

  int exists_within(int integer, int* integer_array, int integer_array_length) {
    for(int current_index=0; current_index < integer_array_length; current_index++) {

      if (integer == integer_array[current_index]) {
	int error_code;
	string error_string;
	switch(type) {
	case plugboard:
	  error_string = "Impossible plugboard configuration found";
	  error_code = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	  break;
	case reflector:
	  error_string = "Invalid reflector mapping";
	  error_code = INVALID_REFLECTOR_MAPPING;
	  break;
	case rotor:
	  error_string =  "Invalid mapping of input " + to_string(integer_array_length) + \
	    " to output " + to_string(integer) + " (output " + to_string(integer) + \
	    " is already mapped to from input " + to_string(current_index) + ")";
	  error_code = INVALID_ROTOR_MAPPING;
	  break;
	default:
	  break;
	}

	print_error(error_string);
	return error_code;
      }
      
    }

    return 0;
  };

  int correct_number_of_parameters(int number_of_parameters) {
    int min, max, error_code = 0;
    string error_string;
    switch(type) {
    case plugboard:
      max = PLUGBOARD_MAX;
      min =  PLUGBOARD_MIN;
      error_string = "Incorrect number of parameters";
      error_code = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
      break;
    case reflector:
      max = REFLECTOR_MAX;
      min = REFLECTOR_MIN;
      error_string = "Insufficient number of mapping";
      error_code =  INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
      break;
    case rotor:
      max = ROTOR_MAX;
      min = ROTOR_MIN;
      error_string = "Not all inputs mapped";
      error_code = INVALID_ROTOR_MAPPING;
      break;
    default:
      break;
    }
    
    if (number_of_parameters > max || number_of_parameters < min) {
      print_error(error_string);
	return error_code;
    }

    if (number_of_parameters % 2) {
      print_error("Incorrect (odd) number of parameters");
	return error_code;
    };

    return error_code;
  };

  

  //int check() {
  // --- PLUGBOARD
  // *CAN BE OPENED
  // *NON NUMERIC CHARACTER used in FILETYPE: FILE NAME
  // *INVALID INDEX used in FILETYPE: FILE NAME
  // *IMPOSSIBLE CONFIGURATION (SAME LETTER TWICE)
  // INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS

  // --- ROTOR
  // * CAN BE OPENED
  // * NO NUMBERIC CHAR
  // * INVALID INDEX
  // CHECK IF NEXT POSITION IS NOTCH
  // * INVALID_ROTOR_MAPPING
  // EVERY ROTOR POSITION HAS BEEN MAPPED

  // --- STARTING POS
  // NO ROTOR STARTING POS
  // NON NUMERIC CHARACTER
  // NON NUMERIC CHAR 2

  // NON NUMERIC CHARACTER
  // INVALID INDEX
  // INVALID ROTOR MAPPING
  // ODD NUMBER OF PARAMETERS
  // INSUFFICIENT NUMBER OF MAPPINGS
  //};

  


};
#endif
