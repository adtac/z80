#include "cpu.hpp"

cpu::cpu() {

}

void cpu::exec_inst() {
}

void cpu::add8(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);
  r.set8(reg0, val0 + val1);
}
