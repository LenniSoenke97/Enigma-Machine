#include"plugboard.hpp"
#include"processor.hpp"

int Plugboard::config(string config_file_path) {
  int error_code = 0;
  int config_file_integer;
  Processor* plugboard_processor = new Processor(Processor::file_type::reflector);

  error_code = plugboard_processor->open(config_file_path);
  if (error_code) return error_code;

  while(plugboard_processor->good()) {
    error_code = plugboard_processor->get_next_int(&config_file_integer);
    if (error_code) return error_code;

    if (plugboard_processor->at_eof()) return 0;

    error_code = plugboard_processor->exists_within(config_file_integer, config_file_integers, config_int_count);
    if (error_code) return error_code;
     
    config_file_integers[config_int_count] = config_file_integer;
    config_int_count++;
  }

   error_code = plugboard_processor->correct_number_of_parameters(config_file_integer);
   if (error_code) return error_code;


  return error_code;

};

void Plugboard::convert(int* input_int) {
  for(int current_int_index=0; current_int_index < config_int_count; current_int_index++) {  

    if (config_file_integers[current_int_index] == *input_int) {

      *input_int = (current_int_index % 2) ? config_file_integers[current_int_index-1] : \
	config_file_integers[current_int_index+1];
      return;

    }
    
  }
}
