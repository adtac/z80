#include <cstdio>
#include "clock.hpp"
#include "cpu.hpp"
#include "cartridge.hpp"
#include "logging.hpp"
#include "mmu.hpp"
#include "mmu.cpp"
#include "joypad.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    ERROR("usage: z80 ROM_FILE");
    return 1;
  }


  cartridge crt;
  crt.init(argv[1]);

  joypad j;

  mmu m(&j, &crt);

  cpu c(&m);

  int (*tick_callbacks[])() = {NULL};

  clock clk(&c, tick_callbacks);

  clk.start_ticking();
}
