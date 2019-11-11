#include"reflector.hpp"
#include"processor.hpp"

int Reflector::config(string config_file_path) {
  int error_code = 0;
  int config_file_integer;
  Processor* reflector_processor = 
	  new Processor(Processor::file_type::reflector);

  error_code = reflector_processor->open(config_file_path);
  if (error_code) return error_code;

  while(reflector_processor->good()) {
    error_code = reflector_processor->get_next_int(&config_file_integer);
    if (error_code) return error_code;

    if (reflector_processor->at_eof()) break;
    if (config_int_count == 26) { config_int_count++; break; }
     
    error_code = reflector_processor->exists_within(config_file_integer, 
		    config_file_integers, 
		    config_int_count);
    if (error_code) return error_code;
     
    config_file_integers[config_int_count] = config_file_integer;
    config_int_count++;
  }

   error_code = reflector_processor->
	   correct_number_of_parameters(config_int_count);
   if (error_code) return error_code;

   delete reflector_processor;
  return error_code;
  
};


void Reflector::convert(int* input_int) {

  for(int current_int_index=0; 
		  current_int_index < config_int_count; 
		  current_int_index++) {

    if (config_file_integers[current_int_index] == *input_int) {

      *input_int = (current_int_index % 2) ? 
	      config_file_integers[current_int_index-1] : \
	config_file_integers[current_int_index+1];
      return;
      
    }

  }
  
}
