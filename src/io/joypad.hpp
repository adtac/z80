#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include <stdint.h>

class joypad {
private:
	uint8_t * locFF00;
	void generateInterrupt();

public:

  joypad();

  uint8_t read(uint16_t address);
  int write(uint16_t address, uint8_t data);
  
  void printMemory();

};

#endif
