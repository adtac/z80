#ifndef _ROM_H_
#define _ROM_H_

#include <stdint.h>

class rom {
  uint8_t* data;

public:

  rom(char* filename);
};

#endif
