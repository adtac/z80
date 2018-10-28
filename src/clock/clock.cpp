#include "clock.hpp"

clock::clock() {
  num_ticks = 0;
}

void clock::tick() {
  num_ticks++;
}

void clock::start_ticking() {
  while (true) {
    tick();
  }
}
