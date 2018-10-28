#ifndef _CPU_H_
#define _CPU_H_

#include "regfile.hpp"

class cpu {
private:

public:

  regfile r;

  cpu();

  void exec_inst();

  void add8(int, int);
};

#endif
