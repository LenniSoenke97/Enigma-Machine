#include"processor.hpp"
#include"rotor.hpp"
#include<string>
#include"errors.h"
#include<iostream>
using namespace std;

int Rotor::config(string config_file_path, string starting_pos_config_file_path, int rotor_pos) {
  int error_code = 0;
  int config_file_integer;
  Processor* rotor_processor = new Processor(Processor::file_type::rotor);
    
  error_code = rotor_processor->open(config_file_path);
  if (error_code) return error_code;

  while(rotor_processor->good()) {
    error_code = rotor_processor->get_next_int(&config_file_integer);
    if (error_code) return error_code;

    if (rotor_processor->at_eof()) break;

    if (config_int_count >= 26) {	
      rotate_notches[rotator_notch_number] = config_file_integer;
      rotator_notch_number++;
      continue;
    }
     
    error_code = rotor_processor->exists_within(config_file_integer, rotor_mapping, config_int_count);
    if (error_code) return error_code;
     
    rotor_mapping[config_int_count] = config_file_integer;
    config_file_offsets[config_int_count] = config_file_integer - config_int_count;
    config_int_count++;
  }
  
  error_code = rotor_processor->correct_number_of_parameters(config_int_count);
  if (error_code) return error_code;

  // STARTING POSITION
  Processor* starting_position_processor = new Processor(Processor::file_type::rotor_position);
  int starting_pos, starting_position_index = 0;
  
  error_code = starting_position_processor->open(starting_pos_config_file_path);
  if (error_code) return error_code;

  while(starting_position_processor->good()) {
    error_code = starting_position_processor->get_next_int(&starting_pos);
    if (error_code) return error_code;

    if (starting_position_processor->at_eof()) {
      starting_position_processor->print_error("No starting position for rotor " + to_string(rotor_pos));
      return NO_ROTOR_STARTING_POSITION;
    }

    if (starting_position_index == rotor_pos) break;
    
    starting_position_index++;
  }
  if (starting_pos > 0) this->rotate(starting_pos);

  error_code = starting_position_processor->get_next_int(&config_file_integer);
  if (error_code) return error_code;
  

  delete starting_position_processor;
  delete rotor_processor;
  
  return error_code;
}

bool Rotor::at_rotation_notch() {
  for(int current_notch=0; current_notch < rotator_notch_number; current_notch++) {
    if ((current_pos+1) == rotate_notches[current_notch]) return true;
  }
  return false;
}

void Rotor::remap() {
  int  mapped_input;
  for (int i=0; i < 26; i++) {
    mapped_input = config_file_offsets[i] + i;
    if (mapped_input > 25) mapped_input -= 26;
    if (mapped_input < 0) mapped_input += 26;
    rotor_mapping[i] = mapped_input;
  }
}

void Rotor::convert_forward(int* input_int) {
  *input_int = rotor_mapping[*input_int];
}

void Rotor::convert_backward(int* input_int) { 
  int input_index=0;
  for(;;input_index++) {
    if(*input_int == rotor_mapping[input_index]) {
      *input_int = input_index;
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
}
