#include "logging.hpp"
#include "file.hpp"

#include <stdint.h>
#include <stdlib.h>

uint8_t* read_file(char* filename, uint64_t* size) {
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
