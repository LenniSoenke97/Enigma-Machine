#ifndef ROTOR
#define ROTOR

#include<string>

using namespace std;

class Rotor {

  /*
   * Main Attributes
   */
  int rotate_notches[25];
  int rotator_notch_number = 0;
  int config_file_offsets[26];
  int rotor_mapping[26];
  int config_int_count = 0;
  int current_pos = 0;

  /*
   * Description: this function updates the rotor_mapping array based on the 
   * config_file_offsets 
   * Input: none
   * Output: void
   */
  void remap(); 
    
public:
  /*
   * Description: Takes a file path and loads all the numbers from that file 
   * path into the config_file_offsets array, furthermore it checks whether 
   * the file (and thus the rotor) is valid
   * Input: file path to rotor config file
   * Output: initialised rotor
   */
  int config(string config_file_path, 
		  string starting_pos_config_file_path, 
		  int rotor_pos);

  /*
   * Description: this function encrypts the given input integer (representing
   * a letter) whilst going from right to left through a rotor. 
   * Input: input integer pointer (representing an input letter)
   * Output: void
   */
  void convert_forward(int* input_int);

  /*
   * Description: this function encrypts the given input integer (representing 
   * a letter) whilst going from left to right through a rotor. 
   * Input: input integer pointer (representing an input letter)
   * Output: void
   */
  void convert_backward(int* input_int);

  /*
   * Description: this function rotates the given rotor by a given number of 
   * positions (default 1), it does this by shifting the config_file_offsets 
   * by x positions and incrementing the current position by x 
   * Input: by_positions represents the integer that determines how many times
   * the given rotor should be
   * rotated (default value is 1)
   * Output: void
   */
  void rotate(int by_positions=1);

  /*
   * Description: this function lets the caller know whether the rotor is 
   * currently at a rotation notch and thus whether to rotor to the left of the 
   * given rotor should also be rotated during the next rotation. 
   * Input: none
   * Output: true if the rotor is at a rotation notch; false otherwise
   */
  bool at_rotation_notch();

};
#endif
