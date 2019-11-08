#ifndef REFLECTOR
#define REFLECTOR
#include<string>

using namespace std;

class Reflector {
  int config_file_integers[26];
  int config_int_count = 0;
  
public:
  /*
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers
   * array, furthermore it checks whether the file (and thus the reflector) is valid
   * Input: file path to reflector config file
   * Output: initialised reflector
   */
  int config(string config_file_path);

   /*
   * Description: convert takes in a pointer to an integer representing a character (zero based, ie. 0=A, 1=B, ...)
   * and switches the value of this character with the character the original input character is mapped to in the
   * reflector config file 
   * Input: pointer to an input integer which should be converted
   * Output: void
   */
  void convert(int* input_int);

};
#endif
