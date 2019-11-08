#ifndef ROTOR
#define ROTOR

#include<string>

using namespace std;


/*
 * Rotor Class
 */
class Rotor {
  int rotate_notches[25];
  int rotator_notch_number = 0;
  int config_file_offsets[26];
  int rotor_mapping[26];
  int config_int_count = 0;
  int current_pos = 0;
    
public:
  /*
   * Name: Rotor constructor
   * Description: Takes a file path and loads all the numbers from that file path into the config_file_offsets array, furthermore it checks whether the file (and thus the rotor) is valid
   * Input: file path to rotor config file
   * Output: initialised rotor
   */
  int config(string config_file_path, string starting_pos_config_file_path, int rotor_pos);

  /*
   *
   * THE BELOW MUST BE DESCRIBED
   *
   */
  void remap(); // make private
  void convert_forward(int* input_int);
  void convert_backward(int* input_int);
  void rotate(int by_positions=1);
  bool at_rotation_notch();

};
#endif
