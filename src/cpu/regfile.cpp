#include "regfile.hpp"

regfile::regfile() {
  for (int i = 0; i < 12; i++)
    set8(i, 0);
}

uint8_t regfile::get8(int idx) {
  return *((uint8_t*) (regs + idx));
}

void regfile::set8(int idx, uint8_t val) {
  *((uint8_t*) (regs + idx)) = val;
}

uint16_t regfile::get16(int idx) {
  return *((uint16_t*) (regs + idx));
}

void regfile::set16(int idx, uint16_t val) {
  *((uint16_t*) (regs + idx)) = val;
}

void regfile::setbit8(int idx, int bit) {
  *((uint8_t*) (regs + idx)) |= (1 << (bit - 1));
}

void regfile::clrbit8(int idx, int bit) {
  *((uint8_t*) (regs + idx)) &= ~(1 << (bit - 1));
}

bool regfile::chkbit8(int idx, int bit) {
  return (*((uint8_t*) (regs + idx)) & (1 << (bit - 1))) > 0
}
