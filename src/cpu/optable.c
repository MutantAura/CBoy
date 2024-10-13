#include <stdio.h>
#include "optable.h"
#include "functions.h"

int tick_cpu(cpu_t* ctx, uint8_t* pool) {
    state = ctx;
    ram = pool;

    // If an instruction was fetched last cycle, execute it this cycle and fetch the next instruction.
    // TODO: check if program counter is placed at fetch op or excecute op?
    if (state->fetch_op != NULL) {
        state->exec_op = state->fetch_op;
    }
    state->fetch_op = &ram[state->registers.pc];

    printf("%04x %02x %02x\n", state->registers.pc, state->exec_op[0], state->exec_op[1]);
    
    // Opcode switch tree - best way to group common opcodes?
    switch(state->exec_op[0]) {
        case 0x00: state->registers.pc++; break; // NOP
        case 0x01: ld_r16_d16(&state->registers.bc.reg16); break;
        case 0x02: ld_ra16_r8(state->registers.bc.reg16, state->registers.af.reg8.high); break;
        case 0x03: inc_r16(&state->registers.bc.reg16); break;
        case 0x04: inc_r8(&state->registers.bc.reg8.high); break;
        case 0x05: dec_r8(&state->registers.bc.reg8.high); break;
        case 0x06: ld_r8_d8(&state->registers.bc.reg8.high); break;
        case 0x07: unimplemented_exception(state, "RLCA", 1, 1); break; // TODO: RLCA
        case 0x08: ld_a16_r16(&state->registers.sp.reg16); break;
        case 0x09: add_r16_r16(&state->registers.hl.reg16, state->registers.bc.reg16); break;
        case 0x0A: ld_r8_ra16(&state->registers.af.reg8.high, state->registers.bc.reg16); break;
        case 0x0B: dec_r16(&state->registers.bc.reg16); break;
        case 0x0C: inc_r8(&state->registers.bc.reg8.low); break; // Cast of &bc to uint8_t* + 1 will take &c
        case 0x0D: dec_r8(&state->registers.bc.reg8.low); break;
        case 0x0E: ld_r8_d8(&state->registers.bc.reg8.low); break;
        case 0x0F: unimplemented_exception(state, "RRCA", 1, 1);

        case 0x10: unimplemented_exception(state, "STOP", 1, 2); break; // TODO: STOP
        case 0x11: ld_r16_d16(&state->registers.de.reg16); break;
        case 0x12: ld_ra16_r8(state->registers.de.reg16, state->registers.af.reg8.high); break;
        case 0x13: inc_r16(&state->registers.de.reg16); break;
        case 0x14: inc_r8(&state->registers.de.reg8.high); break; // Cast of &de to uint8_t* will take &d
        case 0x15: dec_r8(&state->registers.de.reg8.high); break;
        case 0x16: ld_r8_d8(&state->registers.de.reg8.high); break;
        case 0x17: unimplemented_exception(state, "RLA", 1, 1); break;
        case 0x18: jr_s8((int8_t)state->exec_op[1]); break;
        case 0x19: add_r16_r16(&state->registers.hl.reg16, state->registers.de.reg16); break;
        case 0x1A: ld_r8_ra16((uint8_t*)&state->registers.af, state->registers.de.reg16); break;
        case 0x1B: dec_r16(&state->registers.de.reg16); break;
        case 0x1C: inc_r8(&state->registers.de.reg8.low); break;
        case 0x1D: dec_r8(&state->registers.de.reg8.low); break;
        case 0x1E: ld_r8_d8(&state->registers.de.reg8.low); break;
        case 0x1F: unimplemented_exception(state, "RRA", 1, 1); break;

        case 0x20: jr_nz_s8(state->exec_op[1]); break;
        case 0x21: ld_r16_d16(&state->registers.hl.reg16); break;
        case 0x22: ld_ra16_r8(state->registers.hl.reg16, state->registers.af.reg8.high); state->registers.hl.reg16++; break;
        case 0x23: inc_r16(&state->registers.hl.reg16); break;
        case 0x24: inc_r8(&state->registers.hl.reg8.high); break; // Cast of &hl to uint8_t* will take &h
        case 0x25: dec_r8(&state->registers.hl.reg8.high); break;
        case 0x26: ld_r8_d8(&state->registers.hl.reg8.high); break;
        case 0x27: unimplemented_exception(state, "DAA", 1, 1); break;
        case 0x28: jr_z_s8((int8_t)state->exec_op[1]); break;
        case 0x29: add_r16_r16(&state->registers.hl.reg16, state->registers.hl.reg16); break;
        case 0x2A: ld_r8_ra16(&state->registers.af.reg8.high, state->registers.hl.reg16); state->registers.hl.reg16++; break;
        case 0x2B: dec_r16(&state->registers.hl.reg16); break;
        case 0x2C: inc_r8(&state->registers.hl.reg8.low); break;
        case 0x2D: dec_r8(&state->registers.hl.reg8.low); break;
        case 0x2E: ld_r8_d8(&state->registers.hl.reg8.low); break;
        case 0x2F: unimplemented_exception(state, "CPL", 1, 1); break;
        
        case 0x31: ld_r16_d16(&state->registers.sp.reg16); break;
        case 0x32: ld_ra16_r8(state->registers.hl.reg16, state->registers.af.reg8.high); state->registers.hl.reg16--; break;
        case 0x33: inc_r16(&state->registers.sp.reg16); break;
        case 0x34: inc_ra16(state->registers.hl.reg16); break;
        case 0x35: inc_ra16(state->registers.hl.reg16); break;
        case 0x36: ld_ra16_d8(state->registers.hl.reg16); break;
        case 0x3B: dec_r16(&state->registers.sp.reg16); break;
        case 0x3C: inc_r8(&state->registers.af.reg8.high); break;
        
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
    state->registers.pc += pc;
}