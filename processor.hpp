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

  
  void print_error(string error_msg);
  
  Processor(file_type type) : type(type) {};

  int get_next_int(int* integer);
  bool good();
  bool at_eof();
  int open(string file_path);
  int exists_within(int integer, int* integer_array, int integer_array_length);
  int correct_number_of_parameters(int number_of_parameters);

  ~Processor() { config_file.close(); }
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
