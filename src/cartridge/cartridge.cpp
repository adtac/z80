#include "cartridge.hpp"
#include "../utils/logging.hpp"
#include <stdlib.h>
#include <stdio.h>

cartridge::cartridge() {
  data = NULL;
  ram_rtc_flag = false;
  rom_bank_number = 0x01;
}

uint8_t* cartridge::read_file(char* filename, uint64_t* size) {
  if (size == NULL) {
    ERROR("invalid size pointer NULL");
    return NULL;
  }

  *size = 0;

  FILE* rom_file = fopen(filename, "rb");
  if (rom_file == NULL) {
    ERROR("cannot open file '%s'", filename);
    return NULL;
  }

  fseek(rom_file, 0, SEEK_END);
  uint64_t fsize = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);

  uint8_t* data = (uint8_t*) malloc(fsize * sizeof(uint8_t));
  if (data == NULL) {
    ERROR("cannot allocate %d bytes of memory", size);
    return NULL;
  }

  fread(data, fsize, 1, rom_file);
  *size = fsize;

  return data;
}

int cartridge::init(char* filename) {
  
  // load the ROM in the array 
  if ((this->data = this->read_file(filename, &this->file_size)) == NULL) {
    ERROR("fatal error reading ROM file");
    return -1;
  }

  // extracting from cartridge header
  ext_ram_size = this->data[0x0149];
  rom_size = this->data[0x0148];

  // initializing the external RAM
  if(ext_ram_size == 0x00)
    extram = NULL;
  else if(ext_ram_size == 0x01)
    extram = (uint8_t *)calloc(2*1024, sizeof(uint8_t));
  else if(ext_ram_size == 0x02)
    extram = (uint8_t *)calloc(8*1024, sizeof(uint8_t));
  else if(ext_ram_size == 0x03)
    extram = (uint8_t *)calloc(32*1024, sizeof(uint8_t));

  return 0;
}

uint8_t cartridge::read(uint16_t address) {
  uint8_t data;
  if(address >= 0x0000 && address <= 0x3FFF) {
    // reading the rom bank 0
    memcpy(&data, this->data + address, 8);
    return data;
  }
  else if(address >= 0x4000 && address <= 0x7FFF) {
    // reading the rom bank 1 .. n
    address = address - 0x4000;
    memcpy(&data, this->data + this->rom_bank_number*16*1024 + address, 8);
    return data;
  }
  else if(address >= 0xA000 && address <= 0xBFFF) {
    // reading from ram or rtc registers
    if(ram_rtc_bank_number >= 0x00 && ram_rtc_bank_number <= 0x03) {
      // reading ram
      memcpy(&data, this->extram + this->ram_rtc_bank_number*8*1024 + address, 8);
      return data;
    }
    else if(ram_rtc_bank_number >= 0x08 && ram_rtc_bank_number <= 0x0c) {
      // --------- case incomplete ------------
    }
  }
}

int cartridge::write(uint16_t address, uint8_t data) {
  if(address >= 0x0000 && address <= 0x1FFF) {
    // enable/disable ram and rtc registers
    if(data == 0x0a)
      ram_rtc_flag = true;
    else if(data == 0x00)
      ram_rtc_flag - false;
  }
  else if(address >= 0x2000 && address <= 0x3FFF) {
    // rom bank number
    data = data & 0x01111111;
    rom_bank_number = data;
    if (rom_bank_number == 0x00)
      rom_bank_number = 0x01;
  }
  else if(address >= 0x4000 && address <= 0x5FFF) {
    // ram bank number and RTC register select
    ram_rtc_bank_number = data;
  }
  else if(address >= 0x6000 && address <= 0x7FFF) {
    // ---------latch clock data----------
  }
  else if(address >= 0xA000 && address <= 0xBFFF) {
    // write depending upon the current bank number/RTC - multiplex
    if(ram_rtc_bank_number >= 0x00 && ram_rtc_bank_number <= 0x03) {
      // writing to ram
      *(this->extram + this->ram_rtc_bank_number*8*1024 + address) = data;
      return 0;
    }
    else if(ram_rtc_bank_number >= 0x08 && ram_rtc_bank_number <= 0x0c) {
      // --------case incomplete-------------
    }
  }
}


// int main() {
// 	cartridge * crt = new cartridge();
// 	crt->init("../../data/pokemon-gold.gbc");
// }