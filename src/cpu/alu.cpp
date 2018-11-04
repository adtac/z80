#include "alu.hpp"
#include "regfile.hpp"
#include "mmu.hpp"
#include "cpu.hpp"

alu::alu(regfile* _r, mmu* _m) {
  r = _r;
  m = _m;
}

void alu::resetcy8(uint16_t res) {
  if (res > 0xFF)
    r->setbit8(REG_F, BIT_CY);
  else
    r->clrbit8(REG_F, BIT_CY);
}

void alu::resetcy16(uint32_t res) {
  if (res > 0xFFFF)
    r->setbit8(REG_F, BIT_CY);
  else
    r->clrbit8(REG_F, BIT_CY);
}

void alu::add8c(int reg0, uint8_t val) {
  uint16_t res = r->get8(reg0);
  res += val;
  r->set8(reg0, (uint8_t) res);
  resetcy8(res);
}

void alu::add8r(int reg0, int reg1) {
  add8c(reg0, r->get8(reg1));
}

void alu::add16c(int reg0, uint16_t val) {
  uint32_t res = r->get16(reg0);
  res += val;
  r->set8(reg0, (uint16_t) res);
  resetcy16(res);
}

void alu::add16r(int reg0, int reg1) {
  add16c(reg0, r->get16(reg1));
}

void cpu::adc8c(int reg0, uint8_t val) {
  uint16_t res = r->get8(reg0);
  res += val;
  res += r->chkbit8(REG_F, BIT_CY);
  r->set8(reg0, (uint8_t) res);
  resetcy8(res);
}

void cpu::adc8r(int reg0, int reg1) {
  adc8c(reg0, r->get8(reg1));
}

void cpu::adc16c(int reg0, uint16_t val) {
  uint32_t res = r->get16(reg0);
  res += val;
  res += r->chkbit8(REG_F, BIT_CY);
  r->set8(reg0, (uint16_t) res);
  resetcy16(res);
}

void cpu::add16r(int reg0, int reg1) {
  adc16c(reg0, r->get16(reg1));
}

void cpu::sub8c(int reg0, uint8_t val) {
  uint32_t res = r->get8(reg0);
  res += ~val + 1;
  r->set8(reg0, (uint8_t) res);
}

void cpu::sub8r(int reg0, int reg1) {
  sub8c(reg0, r->get8(reg1));
}

void cpu::sub16c(int reg0, uint16_t val) {
  uint32_t res = r->get16(reg0);
  res += ~val + 1;
  r->set16(reg0, (uint16_t) res);
}

void cpu::sub16r(int reg0, int reg1) {
  add16r(reg0, r->get16(reg1));
}

void cpu::sbc8c(int reg0, uint8_t val) {
  sub8c(reg0, val);
  if (r->chkbit8(REG_F, BIT_CY))
    sub8c(reg0, 0xFF);
}

void cpu::sbc8r(int reg0, int reg1) {
  sub8c(reg0, r->get8(reg1));
}

void cpu::sbc16c(int reg0, uint16_t val) {
  sub16c(reg0, val);
  if (r->chkbit8(REG_F, BIT_CY))
    sub16c(reg0, 0xFFFF);
}

void cpu::sbc16r(int reg0, int reg1) {
  sbc16c(reg0, r->get16(reg1));
}

void cpu::inc8r(int reg0) {
  r->set8(reg0, r->get8(reg0) + 1);
}

void cpu::inc16r(int reg0) {
  r->set16(reg0, r->get16(reg0) + 1);
}

void cpu::dec8r(int reg0) {
  sub8c(reg0, 1);
}

void cpu::dec16r(int reg0) {
  sub16c(reg0, 1);
}
