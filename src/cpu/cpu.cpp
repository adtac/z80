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
  int bit0 = -1;
  int bit1 = -1;
  char* c0 = NULL;
  char* c1 = NULL;

  switch (b1) {
    case 0x00:
    INST_NOP:
      INSTRUCTION("nop");
      break;

    // ld r0, x
    case 0x3E: reg0 = REG_A; c0 = "a"; goto INST_LD_R1_X;
    case 0x06: reg0 = REG_B; c0 = "b"; goto INST_LD_R1_X;
    case 0x0E: reg0 = REG_C; c0 = "c"; goto INST_LD_R1_X;
    case 0x16: reg0 = REG_D; c0 = "d"; goto INST_LD_R1_X;
    case 0x1E: reg0 = REG_E; c0 = "e"; goto INST_LD_R1_X;
    case 0x26: reg0 = REG_H; c0 = "h"; goto INST_LD_R1_X;
    case 0x2E: reg0 = REG_L; c0 = "l"; goto INST_LD_R1_X;
    INST_LD_R1_X:
      x = pc_read8();
      r->set8(reg0, x);
      INSTRUCTION("ld %s, %u", c0, x);
      break;

    // ld r0, r1
    case 0x7F: reg0 = REG_A; reg1 = REG_A; c0 = "a"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x78: reg0 = REG_A; reg1 = REG_B; c0 = "a"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x79: reg0 = REG_A; reg1 = REG_C; c0 = "a"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x7A: reg0 = REG_A; reg1 = REG_D; c0 = "a"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x7B: reg0 = REG_A; reg1 = REG_E; c0 = "a"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x7C: reg0 = REG_A; reg1 = REG_H; c0 = "a"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x7D: reg0 = REG_A; reg1 = REG_L; c0 = "a"; c1 = "l"; goto INST_LD_R0_R1;
    case 0x47: reg0 = REG_B; reg1 = REG_A; c0 = "b"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x40: reg0 = REG_B; reg1 = REG_B; c0 = "b"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x41: reg0 = REG_B; reg1 = REG_C; c0 = "b"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x42: reg0 = REG_B; reg1 = REG_D; c0 = "b"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x43: reg0 = REG_B; reg1 = REG_E; c0 = "b"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x44: reg0 = REG_B; reg1 = REG_H; c0 = "b"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x45: reg0 = REG_B; reg1 = REG_L; c0 = "b"; c1 = "l"; goto INST_LD_R0_R1;
    case 0x4F: reg0 = REG_C; reg1 = REG_A; c0 = "c"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x48: reg0 = REG_C; reg1 = REG_B; c0 = "c"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x49: reg0 = REG_C; reg1 = REG_C; c0 = "c"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x4A: reg0 = REG_C; reg1 = REG_D; c0 = "c"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x4B: reg0 = REG_C; reg1 = REG_E; c0 = "c"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x4C: reg0 = REG_C; reg1 = REG_H; c0 = "c"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x4D: reg0 = REG_C; reg1 = REG_L; c0 = "c"; c1 = "l"; goto INST_LD_R0_R1;
    case 0x57: reg0 = REG_D; reg1 = REG_A; c0 = "d"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x50: reg0 = REG_D; reg1 = REG_B; c0 = "d"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x51: reg0 = REG_D; reg1 = REG_C; c0 = "d"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x52: reg0 = REG_D; reg1 = REG_D; c0 = "d"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x53: reg0 = REG_D; reg1 = REG_E; c0 = "d"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x54: reg0 = REG_D; reg1 = REG_H; c0 = "d"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x55: reg0 = REG_D; reg1 = REG_L; c0 = "d"; c1 = "l"; goto INST_LD_R0_R1;
    case 0x5F: reg0 = REG_E; reg1 = REG_A; c0 = "e"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x58: reg0 = REG_E; reg1 = REG_B; c0 = "e"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x59: reg0 = REG_E; reg1 = REG_C; c0 = "e"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x5A: reg0 = REG_E; reg1 = REG_D; c0 = "e"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x5B: reg0 = REG_E; reg1 = REG_E; c0 = "e"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x5C: reg0 = REG_E; reg1 = REG_H; c0 = "e"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x5D: reg0 = REG_E; reg1 = REG_L; c0 = "e"; c1 = "l"; goto INST_LD_R0_R1;
    case 0x67: reg0 = REG_H; reg1 = REG_A; c0 = "h"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x60: reg0 = REG_H; reg1 = REG_B; c0 = "h"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x61: reg0 = REG_H; reg1 = REG_C; c0 = "h"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x62: reg0 = REG_H; reg1 = REG_D; c0 = "h"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x63: reg0 = REG_H; reg1 = REG_E; c0 = "h"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x64: reg0 = REG_H; reg1 = REG_H; c0 = "h"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x65: reg0 = REG_H; reg1 = REG_L; c0 = "h"; c1 = "l"; goto INST_LD_R0_R1;
    case 0x6F: reg0 = REG_L; reg1 = REG_A; c0 = "l"; c1 = "a"; goto INST_LD_R0_R1;
    case 0x68: reg0 = REG_L; reg1 = REG_B; c0 = "l"; c1 = "b"; goto INST_LD_R0_R1;
    case 0x69: reg0 = REG_L; reg1 = REG_C; c0 = "l"; c1 = "c"; goto INST_LD_R0_R1;
    case 0x6A: reg0 = REG_L; reg1 = REG_D; c0 = "l"; c1 = "d"; goto INST_LD_R0_R1;
    case 0x6B: reg0 = REG_L; reg1 = REG_E; c0 = "l"; c1 = "e"; goto INST_LD_R0_R1;
    case 0x6C: reg0 = REG_L; reg1 = REG_H; c0 = "l"; c1 = "h"; goto INST_LD_R0_R1;
    case 0x6D: reg0 = REG_L; reg1 = REG_L; c0 = "l"; c1 = "l"; goto INST_LD_R0_R1;
    INST_LD_R0_R1:
      r->set8(reg0, r->get8(reg1));
      INSTRUCTION("ld %s, %s", c0, c1);
      break;

    // ld r0, [hl]
    case 0x7E: reg0 = REG_A; c0 = "a"; goto INST_LD_R0_MHL;
    case 0x46: reg0 = REG_B; c0 = "b"; goto INST_LD_R0_MHL;
    case 0x4E: reg0 = REG_C; c0 = "c"; goto INST_LD_R0_MHL;
    case 0x56: reg0 = REG_D; c0 = "d"; goto INST_LD_R0_MHL;
    case 0x5E: reg0 = REG_E; c0 = "e"; goto INST_LD_R0_MHL;
    case 0x66: reg0 = REG_H; c0 = "h"; goto INST_LD_R0_MHL;
    case 0x6E: reg0 = REG_L; c0 = "l"; goto INST_LD_R0_MHL;
    INST_LD_R0_MHL:
      r->set8(reg0, m->read8(r->get16(REG_HL)));
      INSTRUCTION("ld %s, [hl]", c0);
      break;

    // ld [hl], r1
    case 0x77: reg1 = REG_A; c1 = "a"; goto INST_LD_MHL_R1;
    case 0x70: reg1 = REG_B; c1 = "b"; goto INST_LD_MHL_R1;
    case 0x71: reg1 = REG_C; c1 = "c"; goto INST_LD_MHL_R1;
    case 0x72: reg1 = REG_D; c1 = "d"; goto INST_LD_MHL_R1;
    case 0x73: reg1 = REG_E; c1 = "e"; goto INST_LD_MHL_R1;
    case 0x74: reg1 = REG_H; c1 = "h"; goto INST_LD_MHL_R1;
    case 0x75: reg1 = REG_L; c1 = "l"; goto INST_LD_MHL_R1;
    INST_LD_MHL_R1:
      m->write8(r->get16(REG_HL), r->get8(reg1));
      INSTRUCTION("ld [hl], %s", c1);
      break;

    // ld [hl], x
    case 0x36:
    INST_LD_MHL_X:
      x = pc_read8();
      m->write8(r->get16(REG_HL), x);
      INSTRUCTION("ld [hl], %u", x);
      break;

    // ld a, [r1]
    case 0x0A: reg1 = REG_BC; c1 = "bc"; goto INST_LD_A_MR1;
    case 0x1A: reg1 = REG_DE; c1 = "de"; goto INST_LD_A_MR1;
    INST_LD_A_MR1:
      r->set8(REG_A, m->read8(r->get16(reg1)));
      INSTRUCTION("ld a, [%s]", c1);
      break;

    // ld [r0], a
    case 0x02: reg0 = REG_BC; c0 = "bc"; goto INST_LD_MR0_A;
    case 0x12: reg0 = REG_DE; c0 = "de"; goto INST_LD_MR0_A;
    INST_LD_MR0_A:
      m->write8(r->get16(reg0), r->get8(REG_A));
      INSTRUCTION("ld [%s], a", c0);
      break;

    // ld a, [xx]
    case 0xFA:
    INST_LD_A_MXX:
      xx = pc_read16();
      r->set8(REG_A, m->read8(xx));
      INSTRUCTION("ld a, [%4x]", xx);
      break;

    // ld [xx], a
    case 0xEA:
    INST_LD_MXX_A:
      xx = pc_read16();
      m->write8(xx, r->get8(REG_A));
      INSTRUCTION("ld [0x%4X], a", xx);
      break;

    // ld a, [0xFF00 + c]
    case 0xF2:
    INST_LD_A_FC:
      r->set8(REG_A, m->read8(0xFF00 + (uint16_t) r->get8(REG_C)));
      INSTRUCTION("ld a, [0xFF00 + 0x%2X]", r->get8(REG_C));
      break;

    // ld [0xFF00 + c], a
    case 0xE2:
    INST_LD_FC_A:
      m->write8(0xFF00 + (uint16_t) r->get8(REG_C), r->get8(REG_A));
      INSTRUCTION("ld [0xFF00 + 0x%2X], a", r->get8(REG_C));
      break;

    // TODO: LDD instructions (see pages 71-75 of GBCPUman.pdf)

    // ld r0, xx
    case 0x01: reg0 = BC; c0 = "bc"; goto INST_LD_R0_XX;
    case 0x11: reg0 = DE; c0 = "de"; goto INST_LD_R0_XX;
    case 0x21: reg0 = HL; c0 = "hl"; goto INST_LD_R0_XX;
    case 0x31: reg0 = SP; c0 = "sp"; goto INST_LD_R0_XX;
    INST_LD_R0_XX:
      xx = pc_read16();
      r->write16(reg0, xx);
      INSTRUCTION("ld %s %u", c0, xx);
      break;

    // ld sp, hl
    case 0xF9:
    INST_LD_SP_HL:
      r->write16(REG_SP, r->read16(REG_HL));
      INSTRUCTION("ld sp, hl");
      break;

    // ldhl sp, n
    case 0xF8:
    INST_LDHL_SP_X:
      x = pc_read8();
      r->write16(REG_SP, r->read16(REG_HL) + x);
      INSTRUCTION("ld sp, %u", x);
      break;

    // ld [xx], sp
    case 0x08:
    INST_LD_MXX_SP:
      xx = pc_read16();
      m->write16(xx, r->get16(REG_SP));
      INSTRUCTION("ld [0x%4X], sp", xx);
      break;

    // push r0
    case 0xF5: reg0 = REG_AF; c0 = "af"; goto INST_PUSH_R0;
    case 0xC5: reg0 = REG_BC; c0 = "bc"; goto INST_PUSH_R0;
    case 0xD5: reg0 = REG_DE; c0 = "de"; goto INST_PUSH_R0;
    case 0xE5: reg0 = REG_HL; c0 = "hl"; goto INST_PUSH_R0;
    INST_PUSH_R0:
      r->set16(REG_SP, r->get16(REG_SP) - 2);
      m->write16(r->get16(REG_SP), r->get16(reg0));
      INSTRUCTION("push %s", c0);
      break;

    // pop r0
    case 0xF1: reg0 = REG_AF; c0 = "af"; goto INST_POP_R0;
    case 0xC1: reg0 = REG_BC; c0 = "bc"; goto INST_POP_R0;
    case 0xD1: reg0 = REG_DE; c0 = "de"; goto INST_POP_R0;
    case 0xE1: reg0 = REG_HL; c0 = "hl"; goto INST_POP_R0;
    INST_POP_R0:
      r->set16(reg0, m->read16(r->get16(REG_SP)));
      r->set16(REG_SP, r->get16(REG_SP) + 2);
      INSTRUCTION("pop %s", c0);
      break;

    // cp a, r1
    case 0xBF: reg1 = REG_A; c1 = "a"; goto INST_CP_A_R1;
    case 0xBF: reg1 = REG_B; c1 = "b"; goto INST_CP_A_R1;
    case 0xBF: reg1 = REG_C; c1 = "c"; goto INST_CP_A_R1;
    case 0xBF: reg1 = REG_D; c1 = "d"; goto INST_CP_A_R1;
    case 0xBF: reg1 = REG_E; c1 = "e"; goto INST_CP_A_R1;
    case 0xBF: reg1 = REG_H; c1 = "h"; goto INST_CP_A_R1;
    case 0xBF: reg1 = REG_L; c1 = "l"; goto INST_CP_A_R1;
    INST_CP_A_R1:
      r->mkbit8(REG_F, BIT_Z, r->get8(REG_A) == r->get8(reg1));
      r->mkbit8(REG_F, BIT_Z, r->get8(REG_A) < r->get8(reg1));
      r->setbit8(REG_F, BIT_N);
      r->setbit8(REG_F, BIT_H);
      INSTRUCTION("cp a, %s", c1);
      break;

    // TODO: SWAP
    // TODO: BCD

    // cpl a
    case 0x2F:
    INST_CPL_A:
      r->set8(REG_A, ~r->get8(REG_A));
      INSTRUCTION("cpl a");
      break;

    // ccf
    case 0x3F:
    INST_CCF:
      r->mkbit8(REG_F, BIT_C, !r->chkbit8(REG_F, BIT_C));
      INSTRUCTION("ccf");
      break;

    // scf
    case 0x37:
    INST_SCF:
      r->setbit8(REG_F, BIT_C);
      INSTRUCTION("scf");
      break;

    // halt
    case 0x76:
    INST_HALT:
      INSTRUCTION("HALT");
      INFO("power off");
      exit(1);
      break;

    // add hl, {bc,de,hl,sp}
    case 0x08: reg1 = REG_BC; c1 = "bc"; goto INST_ADD_HL_R1;
    case 0x19: reg1 = REG_DE; c1 = "de"; goto INST_ADD_HL_R1;
    case 0x29: reg1 = REG_HL; c1 = "hl"; goto INST_ADD_HL_R1;
    case 0x39: reg1 = REG_SP; c1 = "sp"; goto INST_ADD_HL_R1;
    INST_ADD_HL_R1:
      a->add16r(REG_HL, reg1);
      INSTRUCTION("add hl, %s", c1);
      break;

    // add a, {b,c,d,e,h,l,a}
    case 0x80: reg1 = REG_B; c1 = "b"; goto INST_ADD_A_R1;
    case 0x81: reg1 = REG_C; c1 = "c"; goto INST_ADD_A_R1;
    case 0x82: reg1 = REG_D; c1 = "d"; goto INST_ADD_A_R1;
    case 0x83: reg1 = REG_E; c1 = "e"; goto INST_ADD_A_R1;
    case 0x84: reg1 = REG_H; c1 = "h"; goto INST_ADD_A_R1;
    case 0x85: reg1 = REG_L; c1 = "l"; goto INST_ADD_A_R1;
    case 0x87: reg1 = REG_A; c1 = "a"; goto INST_ADD_A_R1;
    INST_ADD_A_R1:
      a->add8r(REG_A, reg1);
      INSTRUCTION("add a, %s", c1);
      break;

    // jp xx
    case 0xC3:
    INST_JP_XX:
      xx = pc_read16();
      r->set16(REG_PC, xx);
      INSTRUCTION("jp 0x%4X", xx);
      break;
    
    // jp cc, xx
    case 0xC2: bit0 = BIT_Z; bit1 = 0; c0 = "nz"; goto INST_JP_CC_XX;
    case 0xCA: bit0 = BIT_Z; bit1 = 1; c0 = "z"; goto INST_JP_CC_XX;
    case 0xD2: bit0 = BIT_CY; bit1 = 0; c0 = "nc"; goto INST_JP_CC_XX;
    case 0xDA: bit0 = BIT_CY; bit1 = 1; c0 = "c"; goto INST_JP_CC_XX;
    INST_JP_CC_XX:
      xx = pc_read6();
      if (r->chkbit8(REG_F, bit0) == bit1)
        r->set16(REG_PC, xx);
      INSTRUCTION("jp %s, 0x%4X", c0, xx);
      break;

    // jp [hl]
    case 0xE9:
    INST_JP_MHL:
      r->set16(REG_PC, r->get16(REG_HL));
      INSTRUCTION("jp [hl]");
      break;
    
    // jp x
    case 0x18:
    INST_JP_X:
      x = pc_read8();
      r->set16(REG_PC, r->get16(REG_PC) + (uint16_t) x);
      INSTRUCTION("jp %u", x);
      break;

    // jr cc, x
    case 0x20: bit0 = BIT_Z; bit1 = 0; c0 = "nz"; goto INST_JP_CC_X;
    case 0x28: bit0 = BIT_Z; bit1 = 1; c0 = "z"; goto INST_JP_CC_X;
    case 0x30: bit0 = BIT_CY; bit1 = 0; c0 = "nc"; goto INST_JP_CC_X;
    case 0x38: bit0 = BIT_CY; bit1 = 1; c0 = "c"; goto INST_JP_CC_X;
    INST_JP_CC_X:
      x = pc_read8();
      if (r->chkbit8(REG_F, bit0) == bit1)
        r->set16(REG_PC, r->get16(REG_PC) + (uint16_t) x);
      INSTRUCTION("jr %s, %d", c0, x);
      break;

    default:
      ERROR("invalid 1-byte instruction 0x%2X at address 0x%4X", b1, r.get16(REG_PC) - 1);
      return -1;
  }

  return 0;
}

void cpu::add8(int reg0, int reg1) {
  reg8 val0 = r.get8(reg0);
  reg8 val1 = r.get8(reg1);
  r.set8(reg0, val0 + val1);
}
