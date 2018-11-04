#ifndef _CPU_H_
#define _CPU_H_

#include "regfile.hpp"
#include "../mmu/mmu.hpp"

class cpu {
private:
  mmu * m; 

public:

  regfile r;

  cpu();

  void exec_inst();

  void add8(int, int);
};

#endif
