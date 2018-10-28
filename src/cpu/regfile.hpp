#ifndef _REGFILE_HPP_
#define _REGFILE_HPP_

#include <stdint.h>

#define REG_A  0
#define REG_F  1
#define REG_AF 0

#define REG_B  2
#define REG_C  3
#define REG_BC 2

#define REG_D  4
#define REG_E  5
#define REG_DE 4

#define REG_H  6
#define REG_L  7
#define REG_HL 6

#define REG_SP 8

#define REG_PC 10

typedef uint8_t reg8;
typedef uint16_t reg16;

class regfile {
private:

  uint8_t regs[12];

public:
  
  regfile();

  reg8 get8(int);
  
  reg16 get16(int);

  void set8(int, reg8);

  void set16(int, reg16);
};

#endif
