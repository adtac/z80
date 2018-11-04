#include <cstdio>
#include "cpu.hpp"
#include "cartridge.hpp"
#include "logging.hpp"
#include "mmu.hpp"
#include "joypad.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    ERROR("usage: z80 ROM_FILE");
    return 1;
  }


  cartridge * crt = new cartridge();
  crt->init(argv[1]);

  joypad * j = new joypad();

  mmu * m = new mmu(j, crt);

  cpu c;
  c.r.set8(REG_A, 5);
  c.r.set8(REG_B, 5);
  c.add8(REG_A, REG_B);

  printf("%d\n", c.r.get8(REG_A));

  int (*tick_callbacks[])() = {NULL};

  clock cl(&c, tick_callbacks);

  int (**tmp)() = fns;
  while (*tmp != NULL) {
    printf("ret=%d\n", (**tmp)());
    tmp++;
  }
}
