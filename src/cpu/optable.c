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
        case 0x10: break; // TODO: STOP

        case 0x01: ld_r16_d16(&state->registers.bc); break;
        case 0x02: ld_ra16_r8(state->registers.bc, state->registers.af >> 8); break;
        case 0x03: inc_r16(&state->registers.bc); break;
        case 0x04: inc_r8((uint8_t*)&state->registers.bc); break; // Cast of &bc to uint8_t* will take &b
        case 0x05: dec_r8((uint8_t*)&state->registers.bc); break;
        case 0x08: dec_r16(&state->registers.bc); break;
        case 0x0C: inc_r8((uint8_t*)&state->registers.bc + 1); break; // Cast of &bc to uint8_t* + 1 will take &c









        case 0x11: ld_r16_d16(&state->registers.de); break;
        case 0x12: ld_ra16_r8(state->registers.de, state->registers.af >> 8); break;
        case 0x13: inc_r16(&state->registers.de); break;
        case 0x14: inc_r8((uint8_t*)&state->registers.de); break; // Cast of &de to uint8_t* will take &d
        case 0x15: dec_r8((uint8_t*)&state->registers.de); break;
        case 0x18: dec_r16(&state->registers.de); break;
        case 0x1C: inc_r8((uint8_t*)&state->registers.de + 1); break;







        case 0x21: ld_r16_d16(&state->registers.hl); break;
        case 0x22: ld_ra16_r8(state->registers.hl, state->registers.af >> 8); state->registers.hl++; break;
        case 0x23: inc_r16(&state->registers.hl); break;
        case 0x24: inc_r8((uint8_t*)&state->registers.hl); break; // Cast of &hl to uint8_t* will take &h
        case 0x25: dec_r8((uint8_t*)&state->registers.hl); break;
        case 0x28: dec_r16(&state->registers.hl); break;
        case 0x2C: inc_r8((uint8_t*)&state->registers.hl + 1); break;










        case 0x31: ld_r16_d16(&state->registers.sp); break;
        case 0x32: ld_ra16_r8(state->registers.hl, state->registers.af >> 8); state->registers.hl--; break;
        case 0x33: inc_r16(&state->registers.sp); break;
        case 0x34: inc_ra16(state->registers.hl); break;
        case 0x35: dec_ra16(state->registers.hl); break;
        case 0x38: dec_r16(&state->registers.sp); break;
        case 0x3C: inc_r8((uint8_t*)&state->registers.af); break;
        
        case 0xCB: // Secondary switch table for 16-bit instructions.
            switch (state->exec_op[1]) {
                // TODO
            }
            break;
    }

    return cycle_cost;
}

void handle_exception(cpu_t* state) {
    printf("Exception in CPU interpreter. State dump.\nPC: %02x\n", state->registers.pc);
}