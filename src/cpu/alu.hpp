#ifndef _ALU_HPP_
#define _ALU_HPP_

#include "../mmu/mmu.hpp"
#include "regfile.hpp"

class alu {
  regfile* r;
  mmu* m;

  void resetcy8(uint16_t);
  void resetcy16(uint32_t);

public:

  alu(regfile*, mmu*);

  void add8c(int, uint8_t);
  void add8r(int, int);
  void add16c(int, uint16_t);
  void add16r(int, int);

  void adc8c(int, uint8_t);
  void adc8r(int, int);
  void adc16c(int, uint16_t);
  void adc16r(int, int);

  void sub8r(int, int);
  void sub8c(int, uint8_t);
  void sub16r(int, int);
  void sub16c(int, uint16_t);

  void sbc8r(int, int);
  void sbc8c(int, uint8_t);
  void sbc16r(int, int);
  void sbc16c(int, uint16_t);

  void inc8r(int);
  void inc16r(int);
  void inc16m(uint16_t);

  void dec8r(int);
  void dec16r(int);
};

#endif
