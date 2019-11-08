#ifndef PROCESSOR
#define PROCESSOR
#include<string>
#include<fstream>

using namespace std;

class Processor {

  /*
   * Main Attributes
   */
  string config_file_name;
  ifstream config_file;

  /*
   * Main Constants (defining expected parameter number for each file)
   */
  const int PLUGBOARD_MIN = 0;
  const int PLUGBOARD_MAX = 26;
  
  const int REFLECTOR_MIN = 26;
  const int REFLECTOR_MAX = 26;
  
  const int ROTOR_MIN = 26;
  const int ROTOR_MAX = 26;
  
public:

  /*
   * file type enum so the processor knows which kind of file it is validating
   */
  enum file_type {plugboard, reflector, rotor, rotor_position};
  file_type type;

  
 
  /*
   * Description: initiation the processor with the correct type
   * Input: a variable named type which is of type Processor::file_type, so that the processor knows
   * what file it is processing
   * Output: a processor object
   */
  Processor(file_type type) : type(type) {};

   void print_error(string error_msg);
  int get_next_int(int* integer);
  bool good();
  bool at_eof();
  int open(string file_path);
  int exists_within(int integer, int* integer_array, int integer_array_length);
  int correct_number_of_parameters(int number_of_parameters);

  ~Processor() { config_file.close(); }

};
#endif
