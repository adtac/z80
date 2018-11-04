#ifndef _CLOCK_HPP_
#define _CLOCK_HPP_

#include "cpu.hpp"

class clock {
  cpu* c;

  void** interrupt_callbacks;

  int (**tick_callbacks)();

public:

  int num_ticks;

  clock(cpu*, int (**)());

  void tick();

  void start_ticking();
};

#endif
