#ifndef _ROM_H_
#define _ROM_H_

#include <stdint.h>

class rom {
  uint64_t rom_size;
  uint8_t* data;

public:

  rom();

  int load_file(char* filename);
};

#endif
