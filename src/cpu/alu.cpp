#include "alu.hpp"
#include "regfile.hpp"
#include "../mmu/mmu.hpp"
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

void alu::adc8c(int reg0, uint8_t val) {
  uint16_t res = r->get8(reg0);
  res += val;
  res += r->chkbit8(REG_F, BIT_CY);
  r->set8(reg0, (uint8_t) res);
  resetcy8(res);
}

void alu::adc8r(int reg0, int reg1) {
  adc8c(reg0, r->get8(reg1));
}

void alu::adc16c(int reg0, uint16_t val) {
  uint32_t res = r->get16(reg0);
  res += val;
  res += r->chkbit8(REG_F, BIT_CY);
  r->set8(reg0, (uint16_t) res);
  resetcy16(res);
}

void alu::adc16r(int reg0, int reg1) {
  adc16c(reg0, r->get16(reg1));
}

void alu::sub8c(int reg0, uint8_t val) {
  uint32_t res = r->get8(reg0);
  res += ~val + 1;
  r->set8(reg0, (uint8_t) res);
}

void alu::sub8r(int reg0, int reg1) {
  sub8c(reg0, r->get8(reg1));
}

void alu::sub16c(int reg0, uint16_t val) {
  uint32_t res = r->get16(reg0);
  res += ~val + 1;
  r->set16(reg0, (uint16_t) res);
}

void alu::sub16r(int reg0, int reg1) {
  add16r(reg0, r->get16(reg1));
}

void alu::sbc8c(int reg0, uint8_t val) {
  sub8c(reg0, val);
  if (r->chkbit8(REG_F, BIT_CY))
    sub8c(reg0, 0xFF);
}

void alu::sbc8r(int reg0, int reg1) {
  sub8c(reg0, r->get8(reg1));
}

void alu::sbc16c(int reg0, uint16_t val) {
  sub16c(reg0, val);
  if (r->chkbit8(REG_F, BIT_CY))
    sub16c(reg0, 0xFFFF);
}

void alu::sbc16r(int reg0, int reg1) {
  sbc16c(reg0, r->get16(reg1));
}

// TODO: do flag stuff for inc/dec

void alu::inc8r(int reg0) {
  r->set8(reg0, r->get8(reg0) + 1);
}

void alu::inc16r(int reg0) {
  r->set16(reg0, r->get16(reg0) + 1);
}

void alu::inc16m(uint16_t addr) {
  m->write16(addr, m->read16(addr) + 1);
}

void alu::dec8r(int reg0) {
  sub8c(reg0, 1);
}

void alu::dec16r(int reg0) {
  sub16c(reg0, 1);
}
