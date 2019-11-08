#include"plugboard.hpp"

void Plugboard::convert(char* input_char) {
  int input_int = static_cast<int>(*input_char) - 65;
  for(int current_int_index=0; current_int_index < config_int_count; current_int_index++) {  

    if (config_file_integers[current_int_index] == input_int) {

      int output_int = (current_int_index % 2) ? config_file_integers[current_int_index-1] : \
	config_file_integers[current_int_index+1];
      *input_char = static_cast<char>(output_int + 65);
      return;

    }
    
  }
}

int Plugboard::config(string config_file_path) {
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

  config_file.close();
  return 0;

};
