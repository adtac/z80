#include <cstdio>
#include "cpu.hpp"
#include "rom.hpp"
#include "logging.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    ERROR("usage: z80 ROM_FILE");
    return 1;
  }

  cpu c;

  rom r;
  r.load_file(argv[1]);

  c.r.set8(REG_A, 5);
  c.r.set8(REG_B, 5);
  c.add8(REG_A, REG_B);

  printf("%d\n", c.r.get8(REG_A));
}
