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

#define BIT_Z 8
#define BIT_N  7
#define BIT_H  6
#define BIT_CY 5

class regfile {
private:

  uint8_t regs[12];

public:
  
  regfile();

  uint8_t get8(int);
  void set8(int, uint8_t);
  
  uint16_t get16(int);
  void set16(int, uint16_t);

  void mkbit8(int, int, bool);
  void setbit8(int, int);
  void clrbit8(int, int);
  bool chkbit8(int, int);

  void print();
};

#endif
