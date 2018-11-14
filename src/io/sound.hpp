#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdint.h>

class sound {
private:
	uint8_t * locC1; 		// 5B  :FF10-FF14
	uint8_t * locC2; 		// 4B  :FF16-FF19
	uint8_t * locC3; 		// 5B  :FF1A-FF1E
	uint8_t * locC3WV; 		// 16B :FF30-FF3F
	uint8_t * locC4; 		// 4B  :FF20-FF23
	uint8_t * locCTRL; 		// 3B  :FF24-FF26

	void generateInterrupt();

public:

  sound();

  uint8_t read(uint16_t address);
  int write(uint16_t address, uint8_t data);
  
  void printMemory();

};

#endif
