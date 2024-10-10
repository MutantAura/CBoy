#include "functions.h"
#include "optable.h"
#include "../models/cpu.h"

// INFO: All functions cycle_cost = their cycle cost.

void ld_r16_d16(uint16_t* reg) {
    *reg = (state->exec_op[2] << 8) | state->exec_op[1];

    state->registers.pc += 3;
    cycle_cost = 3;
}

void ld_ra16_r8(uint16_t adr, uint8_t value) {
    ram[adr] = value;

    state->registers.pc += 2;
    cycle_cost = 2;
}

void inc_r16(uint16_t* reg) {
    *reg++;

    state->registers.pc++;
    cycle_cost = 2;
}

void inc_r8(uint8_t* reg) {
    uint16_t overflow = *reg + 1;
    overflow > UINT8_MAX ? set_flags(ZERO, 1) : set_flags(ZERO, 0);
    overflow & MASK_LOW_4 ? set_flags(HALF, 1) : set_flags(HALF, 0);
    set_flags(SUB, 0);

    *reg++;

    state->registers.pc++;
    cycle_cost = 1;
}

void inc_ra16(uint16_t adr) {
    uint16_t overflow = ram[adr] + 1;

    // Handle flags/overflow

    ram[adr]++;

    state->registers.pc++;
    cycle_cost = 3;
}

void dec_r16(uint16_t* reg) {
    *reg--;

    state->registers.pc += 2;
    cycle_cost = 2;
}

void dec_r8(uint16_t* reg) {
    *reg--;

    state->registers.pc++;
    cycle_cost = 1;
}

void adc_A_r8(uint16_t* reg) {

}

void set_flags(flag_t target, uint8_t set) {
    // Zero - Set when something becomes 0, simples.
    // Carry - Set when 8 bit addition > 0xFF, 16 bit addition > 0xFFFF, subtraction < 0, when rotation shifts out a "1".
    // Sub - Is instruction a subtraction operation? Simples.
    // Half - Set when 4 bit addition > 0xF?

    // Extract register `f` via 8 bit cast + 8 bits.
    uint8_t* reg_f = (uint8_t*)&state->registers.af + 1;

    if (set) {
        switch (target) {
            case ZERO: *reg_f |= (1 << 7); return;
            case SUB: *reg_f |= (1 << 6); return;
            case HALF: *reg_f |= (1 << 5); return;
            case CARRY: *reg_f |= (1 << 4); return;
        }
    } else {
        switch (target) {
            case ZERO: *reg_f &= ~(1 << 7); return;
            case SUB: *reg_f &= ~(1 << 6); return;
            case HALF: *reg_f &= ~(1 << 5); return;
            case CARRY: *reg_f &= ~(1 << 4); return;
        }
    }
}