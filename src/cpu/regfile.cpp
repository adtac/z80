#include "regfile.hpp"

regfile::regfile() {
  for (int i = 0; i < 12; i++)
    set8(i, 0);
}

reg8 regfile::get8(int idx) {
  return *((reg8*) (regs + idx));
}

reg16 regfile::get16(int idx) {
  return *((reg16*) (regs + idx));
}

void regfile::set8(int idx, reg8 val) {
  *((reg8*) (regs + idx)) = val;
}

void regfile::set16(int idx, reg16 val) {
  *((reg16*) (regs + idx)) = val;
}
