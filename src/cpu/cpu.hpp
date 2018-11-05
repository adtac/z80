#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#include "regfile.hpp"
#include "alu.hpp"
#include "../mmu/mmu.hpp"

class cpu {
private:
  uint16_t pc;
  uint8_t pc_read8();
  uint16_t pc_read16();

  alu* a;

public:

  regfile* r;

  mmu* m;

  cpu(mmu*);

  int exec_inst();

  void add8(int, int);
};

#endif
