#include<fstream>

using namespace std;

class Processor {
  file_type type;
  bool good = true;
  string config_file_name;
  ifstream config_file;
  
public:

  enum file_type = {
    PLUGBOARD,
    REFLECTOR,
    ROTOR,
    ROTOR_POSITION
  }
  
  Processor(file_type type) : type(type) {
   
  };

  bool good() {
    int next_int = 0;
    if (!(config_file >> next_int)) {
      if (config_file.eof()) return false;
      config_file.putback(next_int);
    }
    return (this.good) ? config_file.good() : this.good;
  }

  int open(string file_path) {
     this->config_file.open(file_path);
      if (!config_file) {
      cerr <<  "Error occured whilst opening rotor file: " << config_file_name << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
    size_t found = config_file_path.find_last_of("/\\");
    this->config_file_name = config_file_path.substr(found+1);
  }
  
  int get_next_int(int* integer) {
     if (!(config_file >> integer)) {
       cerr << "Non-numeric character in reflector file" << this->config_file_name << endl;
       return NON_NUMERIC_CHARACTER;
     }
     if (integer < 0 || 25 < integer) {
        cerr << "Invalid Index used in reflector file " << config_file_name << endl;
        return INVALID_INDEX;
      }
     return 0;
  }

  int exists_within(int integer, int[] array, int array_length) {
     for(int i; i < config_int_count; i++) {
        if (current_int == config_file_integers[i]) {
          cerr << "Invalid reflector mapping in reflector file " << config_file_name << endl;
          return INVALID_REFLECTOR_MAPPING;
        }
      }
     return 0;
  }

  

  int check() {
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
  };

  

  int* get_result() {

  };

};
