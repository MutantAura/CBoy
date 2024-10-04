#include <stdio.h>
#include "cpu.h"

int tick_cpu(cpu_t* ctx, uint8_t* pool) {
    // If an instruction was fetched last cycle, execute it this cycle and fetch the next instruction.
    // TODO: check if program counter is placed at fetch op or excecute op? 
    if (ctx->fetch_op != NULL) {
        ctx->exec_op = ctx->fetch_op;
    }
    ctx->fetch_op = &pool[ctx->registers.pc];

    printf("%04x %02x %02x\n", ctx->registers.pc, ctx->exec_op[0], ctx->exec_op[1]);
    
    // Opcode switch tree - best way to group common opcodes?
    // TODO: Generic functions for similar opcodes?
    int cycles = 0;
    switch(ctx->exec_op[0]) {
        case 0x00: break; // NOP
        case 0x10: break; // TODO: STOP

        case 0x01: ld_r16_d16(&ctx->registers.bc, ctx->exec_op[0], ctx->exec_op[1], &ctx->registers.pc); cycles = 3; break;
        case 0x11: ld_r16_d16(&ctx->registers.de, ctx->exec_op[0], ctx->exec_op[1], &ctx->registers.pc); cycles = 3; break;
        case 0x21: ld_r16_d16(&ctx->registers.hl, ctx->exec_op[0], ctx->exec_op[1], &ctx->registers.pc); cycles = 3; break;
        case 0x31: ld_r16_d16(&ctx->registers.sp, ctx->exec_op[0], ctx->exec_op[1], &ctx->registers.pc); cycles = 3; break;
        
    }

    return cycles;
}

void ld_r16_d16(uint16_t* reg, uint8_t low, uint8_t high, uint16_t* pc) {
    *reg = (high << 8) | low;
    *pc += 3;
}

void adc_A_r8(uint16_t* reg, uint8_t value) {

}













void handle_exception(cpu_t* state) {
    printf("Exception in CPU interpreter. State dump.\nPC: %02x\n", state->registers.pc);
}