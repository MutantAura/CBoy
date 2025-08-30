#include <stdio.h>
#include "optable.h"
#include "functions.h"

cpu_t* state;
uint8_t* ram;
registers_t* regs;
int cycle_cost;

int tick_cpu(cpu_t* ctx, uint8_t* pool, uint8_t* cart_buff) {
    state = ctx;
    ram = pool;
    regs = &ctx->registers;
    
    // If an instruction was fetched last cycle, execute it this cycle and fetch the next instruction.
    // TODO: check if program counter is placed at fetch op or excecute op?
    state->exec_op = state->fetch_op;
    printf("%04x %02x %02x\n", regs->pc.reg16, state->exec_op[0], state->exec_op[1]);

    // Opcode switch tree - best way to group common opcodes?
    switch(state->exec_op[0]) {
        case 0x00: regs->pc.reg16++; cycle_cost = 1; break; // NOP
        case 0x01: ld_r16_d16(&regs->bc.reg16); break;
        case 0x02: ld_ra16_r8(regs->bc.reg16, regs->af.high); break;
        case 0x03: inc_r16(&regs->bc.reg16); break;
        case 0x04: inc_r8(&regs->bc.high); break;
        case 0x05: dec_r8(&regs->bc.high); break;
        case 0x06: ld_r8_d8(&regs->bc.high); break;
        case 0x07: unimplemented_exception("RLCA", 1, 1); break;
        case 0x08: ld_a16_sp16(); break;
        case 0x09: add_r16_r16(&regs->hl.reg16, regs->bc.reg16); break;
        case 0x0A: ld_r8_ra16(&regs->af.high, regs->bc.reg16); break;
        case 0x0B: dec_r16(&regs->bc.reg16); break;
        case 0x0C: inc_r8(&regs->bc.low); break;
        case 0x0D: dec_r8(&regs->bc.low); break;
        case 0x0E: ld_r8_d8(&regs->bc.low); break;
        case 0x0F: unimplemented_exception("RRCA", 1, 1); break;

        case 0x10: unimplemented_exception("STOP", 1, 2); break;
        case 0x11: ld_r16_d16(&regs->de.reg16); break;
        case 0x12: ld_ra16_r8(regs->de.reg16, regs->af.high); break;
        case 0x13: inc_r16(&regs->de.reg16); break;
        case 0x14: inc_r8(&regs->de.high); break;
        case 0x15: dec_r8(&regs->de.high); break;
        case 0x16: ld_r8_d8(&regs->de.high); break;
        case 0x17: unimplemented_exception("RLA", 1, 1); break;
        case 0x18: jr_s8((int8_t)state->exec_op[1]); break;
        case 0x19: add_r16_r16(&regs->hl.reg16, regs->de.reg16); break;
        case 0x1A: ld_r8_ra16((uint8_t*)&regs->af, regs->de.reg16); break;
        case 0x1B: dec_r16(&regs->de.reg16); break;
        case 0x1C: inc_r8(&regs->de.low); break;
        case 0x1D: dec_r8(&regs->de.low); break;
        case 0x1E: ld_r8_d8(&regs->de.low); break;
        case 0x1F: unimplemented_exception("RRA", 1, 1); break;

        case 0x20: jr_nz_s8(state->exec_op[1]); break;
        case 0x21: ld_r16_d16(&regs->hl.reg16); break;
        case 0x22: ld_ra16_r8(regs->hl.reg16, regs->af.high); regs->hl.reg16++; break;
        case 0x23: inc_r16(&regs->hl.reg16); break;
        case 0x24: inc_r8(&regs->hl.high); break;
        case 0x25: dec_r8(&regs->hl.high); break;
        case 0x26: ld_r8_d8(&regs->hl.high); break;
        case 0x27: unimplemented_exception("DAA", 1, 1); break;
        case 0x28: jr_z_s8((int8_t)state->exec_op[1]); break;
        case 0x29: add_r16_r16(&regs->hl.reg16, regs->hl.reg16); break;
        case 0x2A: ld_r8_ra16(&regs->af.high, regs->hl.reg16); regs->hl.reg16++; break;
        case 0x2B: dec_r16(&regs->hl.reg16); break;
        case 0x2C: inc_r8(&regs->hl.low); break;
        case 0x2D: dec_r8(&regs->hl.low); break;
        case 0x2E: ld_r8_d8(&regs->hl.low); break;
        case 0x2F: unimplemented_exception("CPL", 1, 1); break;
        
        case 0x30: jr_nc_s8((int8_t)state->exec_op[1]); break;
        case 0x31: ld_r16_d16(&regs->sp.reg16); break;
        case 0x32: ld_ra16_r8(regs->hl.reg16, regs->af.high); regs->hl.reg16--; break;
        case 0x33: inc_r16(&regs->sp.reg16); break;
        case 0x34: inc_ra16(regs->hl.reg16); break;
        case 0x35: inc_ra16(regs->hl.reg16); break;
        case 0x36: ld_ra16_d8(regs->hl.reg16); break;
        case 0x37: set_flag(CARRY, 1); set_flag(SUB, 0); set_flag(HALF, 0); regs->pc.reg16++; cycle_cost = 1; break;
        case 0x38: jr_c_s8((int8_t)state->exec_op[1]); break;
        case 0x39: add_r16_r16(&regs->hl.reg16, regs->sp.reg16); break;
        case 0x3A: ld_r8_ra16(&regs->af.high, regs->hl.reg16); regs->hl.reg16--; break;
        case 0x3B: dec_r16(&regs->sp.reg16); break;
        case 0x3C: inc_r8(&regs->af.high); break;
        case 0x3D: dec_r8(&regs->af.high); break;
        case 0x3E: ld_r8_d8(&regs->af.high); break;
        case 0x3F: set_flag(SUB, 0); set_flag(HALF, 0); flip_flag(CARRY); regs->pc.reg16++; cycle_cost = 1; break;

        case 0x40: ld_r8_r8(&regs->bc.high, regs->bc.high); break;
        case 0x41: ld_r8_r8(&regs->bc.high, regs->bc.low); break;
        case 0x42: ld_r8_r8(&regs->bc.high, regs->de.high); break;
        case 0x43: ld_r8_r8(&regs->bc.high, regs->de.low); break;
        case 0x44: ld_r8_r8(&regs->bc.high, regs->hl.high); break;
        case 0x45: ld_r8_r8(&regs->bc.high, regs->hl.low); break;
        case 0x46: ld_r8_ra16(&regs->bc.high, regs->hl.reg16); break;
        case 0x47: ld_r8_r8(&regs->bc.high, regs->af.high); break;
        case 0x48: ld_r8_r8(&regs->bc.low, regs->bc.high); break;
        case 0x49: ld_r8_r8(&regs->bc.low, regs->bc.low); break;
        case 0x4A: ld_r8_r8(&regs->bc.low, regs->de.high); break;
        case 0x4B: ld_r8_r8(&regs->bc.low, regs->de.low); break;
        case 0x4C: ld_r8_r8(&regs->bc.low, regs->hl.high); break;
        case 0x4D: ld_r8_r8(&regs->bc.low, regs->hl.low); break;
        case 0x4E: ld_r8_ra16(&regs->bc.low, regs->hl.reg16); break;
        case 0x4F: ld_r8_r8(&regs->bc.low, regs->af.high); break;

        case 0x50: ld_r8_r8(&regs->de.high, regs->bc.high); break;
        case 0x51: ld_r8_r8(&regs->de.high, regs->bc.low); break;
        case 0x52: ld_r8_r8(&regs->de.high, regs->de.high); break;
        case 0x53: ld_r8_r8(&regs->de.high, regs->de.low); break;
        case 0x54: ld_r8_r8(&regs->de.high, regs->hl.high); break;
        case 0x55: ld_r8_r8(&regs->de.high, regs->hl.low); break;
        case 0x56: ld_r8_ra16(&regs->de.high, regs->hl.reg16); break;
        case 0x57: ld_r8_r8(&regs->de.high, regs->af.high); break;
        case 0x58: ld_r8_r8(&regs->de.low, regs->bc.high); break;
        case 0x59: ld_r8_r8(&regs->de.low, regs->bc.low); break;
        case 0x5A: ld_r8_r8(&regs->de.low, regs->de.high); break;
        case 0x5B: ld_r8_r8(&regs->de.low, regs->de.low); break;
        case 0x5C: ld_r8_r8(&regs->de.low, regs->hl.high); break;
        case 0x5D: ld_r8_r8(&regs->de.low, regs->hl.low); break;
        case 0x5E: ld_r8_ra16(&regs->de.low, regs->hl.reg16); break;
        case 0x5F: ld_r8_r8(&regs->de.low, regs->af.high); break;

        case 0x60: ld_r8_r8(&regs->hl.high, regs->bc.high); break;
        case 0x61: ld_r8_r8(&regs->hl.high, regs->bc.low); break;
        case 0x62: ld_r8_r8(&regs->hl.high, regs->de.high); break;
        case 0x63: ld_r8_r8(&regs->hl.high, regs->de.low); break;
        case 0x64: ld_r8_r8(&regs->hl.high, regs->hl.high); break;
        case 0x65: ld_r8_r8(&regs->hl.high, regs->hl.low); break;
        case 0x66: ld_r8_ra16(&regs->hl.high, regs->hl.reg16); break;
        case 0x67: ld_r8_r8(&regs->hl.high, regs->af.high); break;
        case 0x68: ld_r8_r8(&regs->hl.low, regs->bc.high); break;
        case 0x69: ld_r8_r8(&regs->hl.low, regs->bc.low); break;
        case 0x6A: ld_r8_r8(&regs->hl.low, regs->de.high); break;
        case 0x6B: ld_r8_r8(&regs->hl.low, regs->de.low); break;
        case 0x6C: ld_r8_r8(&regs->hl.low, regs->hl.high); break;
        case 0x6D: ld_r8_r8(&regs->hl.low, regs->hl.low); break;
        case 0x6E: ld_r8_ra16(&regs->hl.low, regs->hl.reg16); break;
        case 0x6F: ld_r8_r8(&regs->hl.low, regs->af.high); break;

        case 0x70: ld_ra16_r8(regs->hl.reg16, regs->bc.high); break;
        case 0x71: ld_ra16_r8(regs->hl.reg16, regs->bc.low); break;
        case 0x72: ld_ra16_r8(regs->hl.reg16, regs->de.high); break;
        case 0x73: ld_ra16_r8(regs->hl.reg16, regs->de.high); break;
        case 0x74: ld_ra16_r8(regs->hl.reg16, regs->hl.high); break;
        case 0x75: ld_ra16_r8(regs->hl.reg16, regs->hl.high); break;
        case 0x76: unimplemented_exception("HALT", 1, 1); break;
        case 0x77: ld_ra16_r8(regs->hl.reg16, regs->af.high); break;
        case 0x78: ld_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0x79: ld_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x7A: ld_r8_r8(&regs->af.high, regs->de.high); break;
        case 0x7B: ld_r8_r8(&regs->af.high, regs->de.low); break;
        case 0x7C: ld_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0x7D: ld_r8_r8(&regs->af.high, regs->hl.low); break;
        case 0x7E: ld_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0x7F: ld_r8_r8(&regs->af.high, regs->af.high); break;

        // General r8 add/adc opcodes
        case 0x80: add_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0x81: add_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x82: add_r8_r8(&regs->af.high, regs->de.high); break;
        case 0x83: add_r8_r8(&regs->af.high, regs->de.low); break;
        case 0x84: add_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0x85: add_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x86: add_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0x87: add_r8_r8(&regs->af.high, regs->af.high); break;
        case 0x88: adc_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0x89: adc_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x8A: adc_r8_r8(&regs->af.high, regs->de.high); break;
        case 0x8B: adc_r8_r8(&regs->af.high, regs->de.low); break;
        case 0x8C: adc_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0x8D: adc_r8_r8(&regs->af.high, regs->hl.low); break;
        case 0x8E: adc_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0x8F: adc_r8_r8(&regs->af.high, regs->af.high); break;

        // General r8 sub/sbc opcodes
        case 0x90: sub_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0x91: sub_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x92: sub_r8_r8(&regs->af.high, regs->de.high); break;
        case 0x93: sub_r8_r8(&regs->af.high, regs->de.low); break;
        case 0x94: sub_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0x95: sub_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x96: sub_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0x97: sub_r8_r8(&regs->af.high, regs->af.high); break;
        case 0x98: sbc_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0x99: sbc_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0x9A: sbc_r8_r8(&regs->af.high, regs->de.high); break;
        case 0x9B: sbc_r8_r8(&regs->af.high, regs->de.low); break;
        case 0x9C: sbc_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0x9D: sbc_r8_r8(&regs->af.high, regs->hl.low); break;
        case 0x9E: sbc_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0x9F: sbc_r8_r8(&regs->af.high, regs->af.high); break;

        // General r8 bitwise AND/XOR
        case 0xA0: and_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0xA1: and_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0xA2: and_r8_r8(&regs->af.high, regs->de.high); break;
        case 0xA3: and_r8_r8(&regs->af.high, regs->de.low); break;
        case 0xA4: and_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0xA5: and_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0xA6: and_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0xA7: and_r8_r8(&regs->af.high, regs->af.high); break;
        case 0xA8: xor_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0xA9: xor_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0xAA: xor_r8_r8(&regs->af.high, regs->de.high); break;
        case 0xAB: xor_r8_r8(&regs->af.high, regs->de.low); break;
        case 0xAC: xor_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0xAD: xor_r8_r8(&regs->af.high, regs->hl.low); break;
        case 0xAE: xor_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0xAF: xor_r8_r8(&regs->af.high, regs->af.high); break;

        // General r8 bitwise OR/CMP opcodes
        case 0xB0: or_r8_r8(&regs->af.high, regs->bc.high); break;
        case 0xB1: or_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0xB2: or_r8_r8(&regs->af.high, regs->de.high); break;
        case 0xB3: or_r8_r8(&regs->af.high, regs->de.low); break;
        case 0xB4: or_r8_r8(&regs->af.high, regs->hl.high); break;
        case 0xB5: or_r8_r8(&regs->af.high, regs->bc.low); break;
        case 0xB6: or_r8_ra16(&regs->af.high, regs->hl.reg16); break;
        case 0xB7: or_r8_r8(&regs->af.high, regs->af.high); break;
        case 0xB8: cmp_r8_r8(regs->af.high, regs->bc.high); break;
        case 0xB9: cmp_r8_r8(regs->af.high, regs->bc.low); break;
        case 0xBA: cmp_r8_r8(regs->af.high, regs->de.high); break;
        case 0xBB: cmp_r8_r8(regs->af.high, regs->de.low); break;
        case 0xBC: cmp_r8_r8(regs->af.high, regs->hl.high); break;
        case 0xBD: cmp_r8_r8(regs->af.high, regs->hl.low); break;
        case 0xBE: cmp_r8_ra16(regs->af.high, regs->hl.reg16); break;
        case 0xBF: cmp_r8_r8(regs->af.high, regs->af.high); break;

        // Stack flow + misc arithmetic.
        case 0xC0: ret_nf(ZERO); break;
        case 0xC1: pop_r16(&regs->bc); break;
        case 0xC2: jmp_nf(ZERO); break;
        case 0xC3: jmp(); break;
        case 0xC4: call_nf_a16(ZERO); break; // TODO: I trust literally none of my call implementations!
        case 0xC5: push_r16(&regs->bc); break;
        case 0xC6: add_r8_d8(&regs->af.high); break;
        case 0xC7: rst(0); break;
        case 0xC8: ret_f(ZERO); break;
        case 0xC9: ret(); break;
        case 0xCA: jmp_f(ZERO); break;
        // case 0xCB: 16-bit opcodes, implemented in alternate tree below.
        case 0xCC: call_f_a16(ZERO); break; // TODO: I trust literally none of my call implementations!
        case 0xCD: call_a16(); break; // TODO: I trust literally none of my call implementations!
        case 0xCE: adc_r8_d8(&regs->af.high); break;
        case 0xCF: rst(1); break;

        case 0xD0: ret_nf(CARRY); break;
        case 0xD1: pop_r16(&regs->de); break;
        case 0xD2: jmp_nf(CARRY); break;
        case 0xD3: unimplemented_exception("INVALID", 0, 0); break;
        case 0xD4: call_nf_a16(CARRY); break;
        case 0xD5: push_r16(&regs->de);
        case 0xD6: sub_r8_d8(&regs->af.high);
        case 0xD7: rst(2); break;
        case 0xD8: ret_f(CARRY); break;
        case 0xD9: unimplemented_exception("RETI", 4, 1); break;
        case 0xDA: jmp_f(CARRY); break;
        case 0xDB: unimplemented_exception("INVALID", 0, 0); break;
        case 0xDC: call_f_a16(CARRY); break;
        case 0xDD: unimplemented_exception("INVALID", 0, 0); break;
        case 0xDE: sbc_r8_d8(&regs->af.high); break;
        case 0xDF: rst(3); break;

        case 0xE0: unimplemented_exception("LD (a8), A", 3, 2); break;
        case 0xE1: pop_r16(&regs->hl); break;
        case 0xE2: unimplemented_exception("LD (C), A", 3, 1); break;
        case 0xE3: unimplemented_exception("INVALID", 0, 0); break;
        case 0xE4: unimplemented_exception("INVALID", 0, 0); break;
        case 0xE5: push_r16(&regs->hl); break;
        case 0xE6: and_d8(&regs->af.high); break;
        case 0xE7: rst(4); break;
        case 0xE8: add_r16_s8(&regs->sp.reg16); break;
        case 0xE9: jmp_r16(regs->hl.reg16); break;
        case 0xEA: ld_a16_r8(regs->af.high); break;
        case 0xEB: unimplemented_exception("INVALID", 0, 0); break;
        case 0xEC: unimplemented_exception("INVALID", 0, 0); break;
        case 0xED: unimplemented_exception("INVALID", 0, 0); break;
        case 0xEE: xor_r8_d8(&regs->af.high); break;
        case 0xEF: rst(5); break;

        case 0xF0: ld_r8_a8(&regs->af.high); break;
        case 0xF1: pop_r16(&regs->af); break;
        case 0xF2: unimplemented_exception("LD A, (C)", 2, 1); break;
        case 0xF3: unimplemented_exception("DI", 1, 1); break;
        case 0xF4: unimplemented_exception("INVALID", 0, 0);
        case 0xF5: push_r16(&regs->af); break;
        case 0xF6: or_r8_d8(&regs->af.high); break;
        case 0xF7: rst(6); break;
        case 0xF8: ld_r16_sps8(&regs->hl.reg16); break; // 16-bit carry to implement
        case 0xF9: ld_r16_r16(regs->hl.reg16, &regs->sp.reg16);
        case 0xFA: ld_r8_a16(&regs->af.high); break;
        case 0xFB: unimplemented_exception("EI", 1, 1); break;
        case 0xFC: unimplemented_exception("INVALID", 0, 0); break;
        case 0xFD: unimplemented_exception("INVALID", 0, 0); break;
        case 0xFE: cmp_r8_d8(regs->af.high); break;
        case 0xFF: rst(7); break;
        
        case 0xCB: // Secondary switch table for 16-bit instructions.
            switch (state->exec_op[1]) {
                // TODO
            }
            break;

        default: unimplemented_exception("Unknown", 1, 1); break;
    }

    state->fetch_op = &ram[regs->pc.reg16];

    return cycle_cost;
}

void unimplemented_exception(char* name, int cost, int pc) {
    printf("Unimplemented instruction hit: %s\n", name);
    printf("Opcode: \"%02X\"\n", state->exec_op[0]);
    printf("Cost: %i\n", cost);
    printf("PC delta: %i\n", pc);

    cycle_cost = -1;
}