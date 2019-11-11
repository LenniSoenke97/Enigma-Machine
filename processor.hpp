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

  /*
   * Description: print error outputs an error message to the standard error stream and conveniently appends the file name and file type
   * Input: the error message we want to output
   * Output: void
   */
  void print_error(string error_msg);

  /*
   * Description: get_next_int fetches the next integer from the given config file, checks it is a valid
   * index and that it is numeric, and changes the value of the argument that is passed by reference
   * to this integer.
   * Input: a pointer to a variable of type integer. This variable will have it value changed to the integer
   * that is fetched from the file.
   * Output: an integer (zero = no error, non zero = the error code of the error that has occured)
   */
  int get_next_int(int* integer);

  /*
   * Description: this function checks whether the config file is good and not at the end of the file
   * Input: none
   * Output: true if the file is good and not at the end of the file, false otherwise
   */
  bool good();

  /*
   * Description: this function can be called to check whether the opened config file is at the end
   * this is necessary because the config file is a private member of the class.
   * Input: none 
   * Output: a processor object
   */
  bool at_eof();

  /*
   * Description: this functions opens the file and checks that it was openend properly, furthermore
   * it sets the filename attribute so it can be used in future error messages
   * Input: the relative file_path to the configuration file
   * Output: an integer (zero = no error, non zero = the error code of the error that has occured)
   */
  int open(string file_path);

  /*
   * Description: this function checks if a certain integer can be found in a given array of given length
   * and outputs the correct error code if the integer is found
   * Input: needle (the integer to search for), the haystack (the array in which the integer should be searched
   * for) and the length of the array (so we can effectively loop through it)
   * Output: an integer (zero = no error, non zero = the error code of the error that has occured)
   */
  int exists_within(int integer, int* integer_array, int integer_array_length);

  /*
   * Description: this function checks if the config file contains the correct number of characters
   * for the given enigma machine part. It checks whether the number of parameters is between the
   * expected maximum and minimum and is not an odd number.
   * Input: in integer representing the number of parameters in a given config file
   * Output: an integer (zero = no error, non zero = the error code of the error that has occured)
   */
  int correct_number_of_parameters(int number_of_parameters);

  /*
   * Description: the processor destructor closes the open file stream
   * Input: none
   * Output: none
   */
  ~Processor() { config_file.close(); }

};
#endif
