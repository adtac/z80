#include <stdio.h>

#include "clock.hpp"
#include "logging.hpp"

clock::clock(cpu* _c, int (**_tick_callbacks)()) {
  c = _c;

  tick_callbacks = _tick_callbacks;

  num_ticks = 0;
}

void clock::tick() {
  num_ticks++;

  int (**tmp)() = tick_callbacks;
  while (*tmp != NULL) {
    (**tmp)();
    tmp++;
  }

  c->exec_inst();
}

void clock::start_ticking() {
  while (true) {
    tick();
  }
}
