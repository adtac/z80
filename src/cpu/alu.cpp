#include "cpu.hpp"

// FLAG support

#define RESET_FLAG                0x00
#define       ZERO_FLAG           0x80
#define RESET_ZERO_FLAG           0x70
#define       BCD_CARRY_FLAG      0x40
#define RESET_BCD_CARRY_FLAG      0xB0
#define       HALFBCD_CARRY_FLAG  0x20
#define RESET_HALFBCD_CARRY_FLAG  0xC0
#define       CARRY_FLAG          0x10
#define RESET_CARRY_FLAG          0xD0
#define SET_FLAG                  0xFF



#define MAXSUM8   0xFF
#define MAXSUM16  0xFFFF

#define FIX_CARRY(x , y)   \
  (x) > (y) ? mask8r(REG_F, CARRY_FLAG) : unmask8r(REG_F, RESET_CARRY_FLAG);
#define FIX_ZERO(x, y)     \
  (x) == (y) ? mask8r(REG_F, ZERO_FLAG) : unmask8r(REG_F, RESET_ZERO_FLAG);


void cpu::addi16r(int reg0, int reg1) {
  reg16 val0 = r.get16(reg0);
  reg16 val1 = r.get16(reg1);

  r.set16(reg0, val0 + val1);
  FIX_CARRY((val0 + val1), MAXSUM16);
}




typedef uint8_t flag8;

void cpu::mask8r(int reg0, flag8 mask) {
   reg8 val0 = r.get8(reg0);  
   
   r.set(reg0, val0 | mask);
}

void cpu::unmask8r(int reg0, flag8 mask) {
   reg8 val0 = r.get8(reg0);

   r.set(reg0, val0 & mask);
}

bool cpu::isflagset(int reg0, flag8 mask) {
   reg8 val0 = r.get8(reg0);
   
   return (val0 | mask);
}

cpu::cpu() {
}

void cpu::exec_inst() {
}

// addition support

void cpu::add8r(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);
 
  r.set8(reg0, val0 + val1);
  FIX_CARRY((val0 + val1), MAXSUM8);
}



void cpu::add8cy(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  reg8 carry = r.isflagset(REG_F, CARRY_FLAG) ? 1 : 0;
  r.set8(reg0, val0 + val1 + carry);
  FIX_CARRY((val0 + val1 + carry), MAX8);
}

void cpu::add8const(int reg0, uint8_t val1) {
  reg8 val0 = r.get8(reg0);

  r.set8(reg0, val0 + val1);
  FIX_CARRY((val0 + val1), MAXSUM8);
}

void cpu::add8constcy(int reg0, uint8_t val1) {
  reg8 val0 = r.get8(reg0);

  reg8 carry = r.isflagset(REG_F, CARRY_FLAG) ? 1 : 0;
  r.set8(reg0, val0 + val1 + carry);
  FIX_CARRY((val0 + val1 + carry), MAX8);
}

// subtraction support

void cpu::sub8r(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  r.set8(reg0, val0 - val1);
}

void cpu::sub8cy(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  reg8 carry = r.isflagset(REG_F, CARRY_FLAG) ? 1 : 0;
  r.set8(reg0, val0 - (val1 + carry));
}

void cpu::sub8const(int reg0, uint8_t val1) {
  reg8 val0 = r.get8(reg0);

  r.set8(reg0, val0 - val1);
}

void cpu::sub8constcy(int reg0, uint8_t val1) {
  reg8 val0 = r.get8(reg0);

  reg8 carry = r.isflagset(REG_F, CARRY_FLAG) ? 1 : 0;
  r.set8(reg0, val0 - (val1 + carry));
}

// AND support
void cpu::and8r(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  r.set8(reg0, val0 & val1);
}

void cpu::and8const(int reg0, int val1) {
  reg8 val0 = r.get8(reg0);

  r.set8(reg0, val0 & val1);
}

// OR support
void cpu::or8r(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  r.set8(reg0, val0 | val1);
}

void cpu::or8const(int reg0, int val1) {
  reg8 val0 = r.get8(reg0);

  r.set8(reg0, val0 | val1);
}

// XOR support
void cpu::xor8r(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  r.set8(reg0, val0 ^ val1);
}

void cpu::xor8const(int reg0, int val1) {
  reg8 val0 = r.get8(reg0);

  r.set8(reg0, val0 ^ val1);
}

// weird subtraction : CP support
void cpu::cp8r(int reg0, int reg1) {
  reg8 acc  = r.get8(reg0);
  reg8 val1 = r.get8(reg1);

  FIX_ZERO((acc), (val1));

  if (val1 >= acc)
  {
      val1 = 1;
      acc  = 0;
  }  

  FIX_CARRY((val1), (acc));
}

void cpu::cp8const(int reg0, int val1) {
  reg8 acc = r.get8(reg0);

  FIX_ZERO((acc), (val1));

  if (val1 >= acc)
  {
      val1 = 1;
      acc  = 0;
  }

  FIX_CARRY((val1), (acc));
}

// INCREMENT support
void cpu::inc8r(int reg0) {
  reg8 val1 = r.get8(reg0);

  r.set8(reg0, val1 + 1);
//  FIX_CARRY(val1, MAXSUM8);
}


void cpu::inc16r(int reg0) {
  reg16 val1 = r.get16(reg0);
  
  r.set16(reg0, val1 + 1);
//  FIX_CARRY(val1, MAXSUM16);
}

// DECREMENT support
void cpu::dec8r(int reg0) {
  reg8 val1 = r.get8(reg0);
  
  r.set8(reg0, val1 - 1);
//  FIX_CARRY(val1, MAXSUM8);
}


void cpu::dec16r(int reg0) {
  reg16 val1 = r.get16(reg0);

  r.set16(reg0, val1 - 1);
//  FIX_CARRY(val1, MAXSUM16);
}





