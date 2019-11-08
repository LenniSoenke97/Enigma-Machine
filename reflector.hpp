#ifndef REFLECTOR
#define REFLECTOR
#include<string>

using namespace std;

class Reflector {
  int config_file_integers[26];
  int config_int_count = 0;
  
public:
  /*
   * Name: Reflector constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_integers array, furthermore it checks whether the file (and thus the reflector) is valid
   * Input: file path to reflector config file
   * Output: initialised reflector
   */
  int config(string config_file_path);
  
  void convert(int* input_int);

};
#endif
