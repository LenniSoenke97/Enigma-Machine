#include<fstream>
#include<iostream>
#include"processor.hpp"
#include"errors.h"
#include<string>


void Processor::print_error(string error_msg) {
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
    type_name = "rotor positions";
    break;
  default:
    break;
  }

  cerr << error_msg << " in " << type_name << " file " << config_file_name << endl;
}

bool Processor::good() {
  return config_file.good() && !config_file.eof();
}

bool Processor::at_eof() {
  return config_file.eof();
}

int Processor::open(string file_path) {
  this->config_file.open(file_path);

  if (!config_file) {
    print_error("Error occued whilst opening config file");
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
    
  size_t found = file_path.find_last_of("/\\");
  this->config_file_name = file_path.substr(found+1);

  return 0;
}
  
int Processor::get_next_int(int* integer) {
  if (!(this->config_file >> *integer)) {

    if (config_file.eof()) return 0;
    string error_string = "Non-numeric character";
    if (type == rotor) error_string += " for mapping";
    print_error(error_string);
    return NON_NUMERIC_CHARACTER;

  }
    
  if (*integer < 0 || 25 < *integer) {

    print_error("Invalid index");
    return INVALID_INDEX;

  }

  return 0;
}

int Processor::exists_within(int integer, int* integer_array, int integer_array_length) {
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
}

int Processor::correct_number_of_parameters(int number_of_parameters) {
   int min, max, error_code = 0;
  string error_string, odd_error_string;
  switch(type) {
  case plugboard:
    max = PLUGBOARD_MAX;
    min =  PLUGBOARD_MIN;
    error_string = "Incorrect number of parameters";
    odd_error_string = "Incorrect number of parameters";
    error_code = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    break;
  case reflector:
    max = REFLECTOR_MAX;
    min = REFLECTOR_MIN;
    error_string = "Insufficient number of mappings";
    odd_error_string = "Incorrect (odd) number of parameters";
    error_code =  INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    break;
  case rotor:
    max = ROTOR_MAX;
    min = ROTOR_MIN;
    error_string = "Not all inputs mapped";
    odd_error_string = "Incorrect (odd) number of parameters";
    error_code = INVALID_ROTOR_MAPPING;
    break;
  default:
    error_string = "No correct type specified";
    odd_error_string = "No correct type specified";
    break;
  }

  cerr << "number of parameters: " << number_of_parameters;

  if (number_of_parameters > max || number_of_parameters < min) {
    print_error(error_string);
    return error_code;
  }

  if (number_of_parameters % 2) {
    print_error(odd_error_string);
    return error_code;
  };

  return 0;
}
  
