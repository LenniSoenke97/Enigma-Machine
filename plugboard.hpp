#ifndef PLUGBOARD
#define PLUGBOARD

#include<string>

using namespace std;

class Plugboard {

  /*
   * Main Attributes
   */
  int config_file_integers[26];
  int config_int_count = 0;

public:
  /*
   * Description: Takes a file path and loads all the numbers from that file 
   * path into the config_file_integers array, furthermore it checks whether 
   * the file (and thus plugboard) is valid
   * Input: file path to plugboard config file
   * Output: initialised plugboard
   */
  int config(string config_file_path);

  /*           
   * Description: converts an input char into an output char. To do this it 
   * performs the normal plugboard action of "switching" letters that are 
   * connected by a plugboard wire.                                                  
   * Input: a pointer to an input char                                   
   * Output: void                                                                                 
   */
  void convert(int* input_int);

};

#endif
