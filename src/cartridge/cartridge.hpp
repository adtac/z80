#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include <stdint.h>

class cartridge {

	private :
  	uint64_t file_size;
  	uint64_t ext_ram_size;
  	uint64_t rom_size;
    uint8_t* data;
  	uint8_t* extram;
  	uint8_t* read_file(char*, uint64_t*);
    bool ram_rtc_flag;
    uint8_t rom_bank_number;
    uint8_t ram_rtc_bank_number;

	public:
  	cartridge();
  	int init(char*);
  	uint8_t read(uint16_t address);
  	int write(uint16_t address, uint8_t data);
};

#endif
