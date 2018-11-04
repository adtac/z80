#ifndef _CPU_H_
#define _CPU_H_

#include "regfile.hpp"
#include "alu.hpp"
#include "mmu.hpp"

class cpu {
private:
  uint16_t pc;
  uint8_t pc8_read();
  uint16_t pc16_read();

  alu* a;

public:

  regfile* r;

  mmu* m;

  cpu();

  void exec_inst();

  void add8(int, int);
};

#endif
