#include "rom.hpp"
#include "file.hpp"
#include "logging.hpp"

rom::rom() {
  data = NULL;
}

int rom::load_file(char* filename) {
  if ((data = read_file(filename, &rom_size)) == NULL) {
    ERROR("fatal error reading ROM file");
    return -1;
  }

  DEBUG("loaded ROM file %s (%lld bytes)", filename, rom_size);

  return 0;
}
