#include"reflector.hpp"

/*
  Reflector
*/
int Reflector::config(string config_file_path) {
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

    config_file.close();
    
    return 0;
  };


void Reflector::convert(char* input_char) {
  int input_int = static_cast<int>(*input_char) - 65;
  for(int current_int_index=0; current_int_index < config_int_count; current_int_index++) {
    if (config_file_integers[current_int_index] == input_int) {
      int output_int = (current_int_index % 2) ? config_file_integers[current_int_index-1] : config_file_integers[current_int_index+1];
      *input_char = static_cast<char>(output_int + 65);
      return;
    }

  }
}
