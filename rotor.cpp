#include"rotor.hpp"

/*
  Rotor
*/
int Rotor::config(string config_file_path, string starting_pos_config_file_path, int rotor_pos) {
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

    config_file.close(); 
    return 0;
  }

bool Rotor::at_rotation_notch() {
    for(int current_notch=0; current_notch < rotator_notch_number; current_notch++) {
      if ((current_pos+1) == rotate_notches[current_notch]) return true;
    }
    return false;
  };

void Rotor::remap() {
  int  mapped_input;
  for (int i=0; i < 26; i++) {
    mapped_input = config_file_offsets[i] + i;
    if (mapped_input > 25) mapped_input -= 26;
    if (mapped_input < 0) mapped_input += 26;
    rotor_mapping[i] = mapped_input;
  }
}

void Rotor::convert_forward(char* input_char) {
  int input_int = static_cast<int>(*input_char) - 65;
  int output_int = rotor_mapping[input_int];
  *input_char = static_cast<char>(output_int + 65);
}

void Rotor::convert_backward(char* input_char) { 
  int input_int = static_cast<int>(*input_char) - 65;
  int input_index=0;
  for(;;input_index++) {
    if(input_int == rotor_mapping[input_index]) {
      *input_char = static_cast<char>(input_index + 65);
      return;
    }
  }
}

void Rotor::rotate(int by_positions) {
  current_pos += by_positions;
  if (current_pos > 25) current_pos = 26 - current_pos;
  int new_config_file_offsets[26], new_config_file_index;
   
  for(int old_config_file_index=0;old_config_file_index<26;old_config_file_index++) {
    new_config_file_index = old_config_file_index-by_positions;
    if (new_config_file_index < 0) {
      new_config_file_index += 26;
    }
    new_config_file_offsets[new_config_file_index] = config_file_offsets[old_config_file_index];
  }

  for (int index=0; index < 26; index++) {
    config_file_offsets[index] = new_config_file_offsets[index];
  }
  this->remap();
};
