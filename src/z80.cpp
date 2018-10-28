#include <cstdio>
#include "cpu.hpp"

int main() {
  cpu c;

  c.r.set8(REG_A, 5);
  c.r.set8(REG_B, 5);
  c.add8(REG_A, REG_B);

  printf("%d\n", c.r.get8(REG_A));
}
