#ifndef _MMU_H_
#define _MMU_H_

#include <stdint.h>
#include "../cartridge/cartridge.hpp"
#include "../io/joypad.hpp"
// #include "../io/lcd.hpp"
// #include "../io/sound.hpp"

class mmu {

	private:
		// I/O memory
		joypad * j;
		// lcd * l;
		// sound * s;
		
		// static memory 
		uint8_t * wram;
		uint8_t * vram;

		// cartridge memory 
		cartridge * crt;

	public:
	  mmu(joypad *, cartridge *);

	  uint8_t read8(uint16_t address);
	  int write8(uint16_t address, uint8_t data);
	  uint16_t read16(uint16_t address);
	  int write16(uint16_t address, uint16_t data);

};

#endif

