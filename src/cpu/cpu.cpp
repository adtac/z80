#include "cpu.hpp"
#include "regfile.hpp"
#include "mmu.hpp"
#include "logging.hpp"

cpu::cpu(mmu* _m) {
  r = (regfile*) malloc(sizeof(regfile));
  new (r) regfile();

  m = _m;

  a = (alu*) malloc(sizeof(alu));
  new (a) alu(r, m);

  r->set16(r->G_AF, 0x01B0);
  r->set16(r->G_BC, 0x0013);
  r->set16(r->G_DE, 0x00D8);
  r->set16(r->G_HL, 0x014D);
  r->set16(r->G_SP, 0xFFFE);
  m->write8(0xFF05, 0x00); // TIMA
  m->write8(0xFF06, 0x00); // TMA
  m->write8(0xFF07, 0x00); // TAC
  m->write8(0xFF10, 0x80); // NR10
  m->write8(0xFF11, 0xBF); // NR11
  m->write8(0xFF12, 0xF3); // NR12
  m->write8(0xFF14, 0xBF); // NR14
  m->write8(0xFF16, 0x3F); // NR21
  m->write8(0xFF17, 0x00); // NR22
  m->write8(0xFF19, 0xBF); // NR24
  m->write8(0xFF1A, 0x7F); // NR30
  m->write8(0xFF1B, 0xFF); // NR31
  m->write8(0xFF1C, 0x9F); // NR32
  m->write8(0xFF1E, 0xBF); // NR33
  m->write8(0xFF20, 0xFF); // NR41
  m->write8(0xFF21, 0x00); // NR42
  m->write8(0xFF22, 0x00); // NR43
  m->write8(0xFF23, 0xBF); // NR30
  m->write8(0xFF24, 0x77); // NR50
  m->write8(0xFF25, 0xF3); // NR51
  m->write8(0xFF26, 0xF1); // NR52 (0xF1 for GBC, 0xF0 for SuperGB)
  m->write8(0xFF40, 0x91); // LCDC
  m->write8(0xFF42, 0x00); // SCY
  m->write8(0xFF43, 0x00); // SCX
  m->write8(0xFF45, 0x00); // LYC
  m->write8(0xFF47, 0xFC); // BGP
  m->write8(0xFF48, 0xFF); // OBP0
  m->write8(0xFF49, 0xFF); // OBP1
  m->write8(0xFF4A, 0x00); // WY
  m->write8(0xFF4B, 0x00); // WX
  m->write8(0xFFFF, 0x00); // IE
}

uint8_t cpu::pc_read8() {
  pc = r.get16(REG_PC);
  uint8_t x = m->read8(pc);
  a->inc16r(REG_PC);
  return x;
}

uint16_t cpu::pc_read16() {
  pc = r.get16(REG_PC);
  uint16_t x = m->read16(pc);
  a->inc16r(REG_PC);
  a->inc16r(REG_PC);
  return x;
}

int cpu::exec_inst() {
  uint8_t b1 = pc_read8();

  uint8_t x;
  uint16_t xx;
  int reg0 = -1;
  int reg1 = -1;
  char* c0 = NULL;
  char* c1 = NULL;

  switch (b1) {
    // nop
    case 0x00:
      INSTRUCTION("nop");
      break;

    case 0x09:
      if (reg1 != -1) reg1 = REG_BC, c1 = "bc";
    case 0x19:
      if (reg1 != -1) reg1 = REG_DE, c1 = "de";
    case 0x29:
      if (reg1 != -1) reg1 = REG_HL, c1 = "hl";
    case 0x39:
      if (reg1 != -1) reg1 = REG_SP, c1 = "sp";
      a->add16r(REG_HL, reg1);
      INSTRUCTION("add hl, %s", c1);
      break;

    case 0x80:
      if (reg1 != -1) reg1 = REG_B, c1 = "b";
    case 0x81:
      if (reg1 != -1) reg1 = REG_C, c1 = "c";
    case 0x82:
      if (reg1 != -1) reg1 = REG_D, c1 = "d";
    case 0x83:
      if (reg1 != -1) reg1 = REG_E, c1 = "e";
    case 0x84:
      if (reg1 != -1) reg1 = REG_H, c1 = "H";
    case 0x85:
      if (reg1 != -1) reg1 = REG_L, c1 = "l";
    case 0x87:
      if (reg1 != -1) reg1 = REG_A, c1 = "a";
      a->add8r(REG_A, reg1);
      INSTRUCTION("add a, %s", c1);

    // jr z, x
    case 0x28:
      x = pc_read8();
      if (r->check(REG_F, FLAG_Z))
        a->add16c(REG_PC, (uint16_t) x);
      INSTRUCTION("jr Z, %2x", x);
      break;

    // jp xx
    case 0xC3:
      xx = pc_read16();
      r->set16(REG_PC, xx);
      INSTRUCTION("jp %4x", xx);
      break;

    default:
      ERROR("invalid 1-byte instruction %x at address %x", b1, r.get16(REG_PC) - 1);
      return -1;
  }

  return 0;
}

void cpu::add8(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);
  r.set8(reg0, val0 + val1);
}
