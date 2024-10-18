#include <stdio.h>
#include "optable.h"
#include "functions.h"

cpu_t* state;
uint8_t* ram;
registers_t* regs;
int cycle_cost;

int tick_cpu(cpu_t* ctx, uint8_t* pool) {
    state = ctx;
    ram = pool;
    regs = &ctx->registers;
    
    // If an instruction was fetched last cycle, execute it this cycle and fetch the next instruction.
    // TODO: check if program counter is placed at fetch op or excecute op?
    if (state->fetch_op != NULL) {
        state->exec_op = state->fetch_op;
    }
    state->fetch_op = &ram[regs->pc];

    printf("%04x %02x %02x\n", regs->pc, state->exec_op[0], state->exec_op[1]);

    // Opcode switch tree - best way to group common opcodes?
    switch(state->exec_op[0]) {
        case 0x00: regs->pc++; break; // NOP
        case 0x01: ld_r16_d16(&regs->bc.reg16); break;
        case 0x02: ld_ra16_r8(regs->bc.reg16, regs->af.high); break;
        case 0x03: inc_r16(&regs->bc.reg16); break;
        case 0x04: inc_r8(&regs->bc.high); break;
        case 0x05: dec_r8(&regs->bc.high); break;
        case 0x06: ld_r8_d8(&regs->bc.high); break;
        case 0x07: unimplemented_exception(state, "RLCA", 1, 1); break; // TODO: RLCA
        case 0x08: ld_a16_sp16(); break;
        case 0x09: add_r16_r16(&regs->hl.reg16, regs->bc.reg16); break;
        case 0x0A: ld_r8_ra16(&regs->af.high, regs->bc.reg16); break;
        case 0x0B: dec_r16(&regs->bc.reg16); break;
        case 0x0C: inc_r8(&regs->bc.low); break; // Cast of &bc to uint8_t* + 1 will take &c
        case 0x0D: dec_r8(&regs->bc.low); break;
        case 0x0E: ld_r8_d8(&regs->bc.low); break;
        case 0x0F: unimplemented_exception(state, "RRCA", 1, 1); break;

        case 0x10: unimplemented_exception(state, "STOP", 1, 2); break; // TODO: STOP
        case 0x11: ld_r16_d16(&regs->de.reg16); break;
        case 0x12: ld_ra16_r8(regs->de.reg16, regs->af.high); break;
        case 0x13: inc_r16(&regs->de.reg16); break;
        case 0x14: inc_r8(&regs->de.high); break; // Cast of &de to uint8_t* will take &d
        case 0x15: dec_r8(&regs->de.high); break;
        case 0x16: ld_r8_d8(&regs->de.high); break;
        case 0x17: unimplemented_exception(state, "RLA", 1, 1); break;
        case 0x18: jr_s8((int8_t)state->exec_op[1]); break;
        case 0x19: add_r16_r16(&regs->hl.reg16, regs->de.reg16); break;
        case 0x1A: ld_r8_ra16((uint8_t*)&regs->af, regs->de.reg16); break;
        case 0x1B: dec_r16(&regs->de.reg16); break;
        case 0x1C: inc_r8(&regs->de.low); break;
        case 0x1D: dec_r8(&regs->de.low); break;
        case 0x1E: ld_r8_d8(&regs->de.low); break;
        case 0x1F: unimplemented_exception(state, "RRA", 1, 1); break;

        case 0x20: jr_nz_s8(state->exec_op[1]); break;
        case 0x21: ld_r16_d16(&regs->hl.reg16); break;
        case 0x22: ld_ra16_r8(regs->hl.reg16, regs->af.high); regs->hl.reg16++; break;
        case 0x23: inc_r16(&regs->hl.reg16); break;
        case 0x24: inc_r8(&regs->hl.high); break; // Cast of &hl to uint8_t* will take &h
        case 0x25: dec_r8(&regs->hl.high); break;
        case 0x26: ld_r8_d8(&regs->hl.high); break;
        case 0x27: unimplemented_exception(state, "DAA", 1, 1); break;
        case 0x28: jr_z_s8((int8_t)state->exec_op[1]); break;
        case 0x29: add_r16_r16(&regs->hl.reg16, regs->hl.reg16); break;
        case 0x2A: ld_r8_ra16(&regs->af.high, regs->hl.reg16); regs->hl.reg16++; break;
        case 0x2B: dec_r16(&regs->hl.reg16); break;
        case 0x2C: inc_r8(&regs->hl.low); break;
        case 0x2D: dec_r8(&regs->hl.low); break;
        case 0x2E: ld_r8_d8(&regs->hl.low); break;
        case 0x2F: unimplemented_exception(state, "CPL", 1, 1); break;
        
        case 0x30: jr_nc_s8((int8_t)state->exec_op[1]); break;
        case 0x31: ld_r16_d16(&regs->sp.reg16); break;
        case 0x32: ld_ra16_r8(regs->hl.reg16, regs->af.high); regs->hl.reg16--; break;
        case 0x33: inc_r16(&regs->sp.reg16); break;
        case 0x34: inc_ra16(regs->hl.reg16); break;
        case 0x35: inc_ra16(regs->hl.reg16); break;
        case 0x36: ld_ra16_d8(regs->hl.reg16); break;
        case 0x37: set_flag(CARRY, 1); set_flag(SUB, 0); set_flag(HALF, 0); regs->pc++; cycle_cost = 1; break;
        case 0x38: jr_c_s8((int8_t)state->exec_op[1]); break;
        case 0x39: add_r16_r16(&regs->hl.reg16, regs->sp.reg16); break;
        case 0x3A: ld_r8_ra16(&regs->af.high, regs->hl.reg16); regs->hl.reg16--; break;
        case 0x3B: dec_r16(&regs->sp.reg16); break;
        case 0x3C: inc_r8(&regs->af.high); break;
        case 0x3D: dec_r8(&regs->af.high); break;
        case 0x3E: ld_r8_d8(&regs->af.high); break;
        case 0x3F: set_flag(SUB, 0); set_flag(HALF, 0); flip_flag(CARRY); regs->pc++; cycle_cost = 1; break;

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
        case 0x76: unimplemented_exception(state, "HALT", 1, 1); break;
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
        
        case 0xCB: // Secondary switch table for 16-bit instructions.
            switch (state->exec_op[1]) {
                // TODO
            }
            break;

        default: unimplemented_exception(state, "Unknown", 1, 1); break;
    }

    return cycle_cost;
}

void unimplemented_exception(cpu_t* state, char* name, int cost, int pc) {
    printf("Unimplemented instruction hit: %s \"%02X\"\n", name, state->exec_op[0]);

    cycle_cost = cost;
    regs->pc += pc;
}