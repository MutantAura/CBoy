#include "functions.h"
#include "optable.h"
#include "../models/cpu.h"

// INFO: All functions cycle_cost = their cycle cost.

// LOAD INSTRUCTIONS
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

void ld_r8_d8(uint8_t* reg) {
    *reg = state->exec_op[1];

    state->registers.pc += 2;
    cycle_cost = 2;
}

void ld_r8_r8(uint8_t* reg) {
    *reg = state->exec_op[1];

    state->registers.pc += 2;
    cycle_cost = 2;
}

void ld_a16_r16(uint16_t* reg) {
    uint16_t adr = *((uint16_t*)state->exec_op);

    ram[adr] = state->registers.sp.reg8.low;
    ram[adr++] = state->registers.sp.reg8.high;

    state->registers.pc += 3;
    cycle_cost = 5;
}

void ld_r8_ra16(uint8_t* reg, uint16_t adr) {
    *reg = ram[adr];

    state->registers.pc++;
    cycle_cost = 2;
}

// INCREMENT/DECREMENT INSTRUCTIONS
void inc_r16(uint16_t* reg) {
    *reg++;

    state->registers.pc++;
    cycle_cost = 2;
}

void inc_r8(uint8_t* reg) {
    *reg++;

    set_flag(ZERO, *reg == 0);
    set_flag(HALF, *reg & MASK_8_LOW4 == 0);
    set_flag(SUB, 0);

    state->registers.pc++;
    cycle_cost = 1;
}

void inc_ra16(uint16_t adr) {
    ram[adr]++;

    set_flag(ZERO, ram[adr] == 0);
    set_flag(HALF, ram[adr] & MASK_8_LOW4 == 0);
    set_flag(SUB, 0);

    state->registers.pc++;
    cycle_cost = 3;
}

void dec_r16(uint16_t* reg) {
    *reg--;

    state->registers.pc += 2;
    cycle_cost = 2;
}

void dec_r8(uint8_t* reg) {
    *reg--;

    set_flag(ZERO, *reg == 0);
    set_flag(HALF, *reg == 0xFF); // Only possible if underflow (0-1 -> 0xFF)
    set_flag(SUB, 1);

    state->registers.pc++;
    cycle_cost = 1;
}

void dec_ra16(uint16_t adr) {
    ram[adr]--;

    set_flag(ZERO, ram[adr] == 0);
    set_flag(HALF, ram[adr] == 0xFF); // Only possible if underflow (0-1 -> 0xFF)
    set_flag(SUB, 1);

    state->registers.pc++;
    cycle_cost = 3;
}

// GENERAL ARITHMETIC INSTRUCTIONS
void add_r16_r16(uint16_t* reg, uint16_t value) {
    uint32_t overflow = *reg + value;

    set_flag(CARRY, overflow > UINT16_MAX);
    set_flag(HALF, (*reg & 0x0FFF) + (value & 0x0FFF) >> 12);
    set_flag(SUB, 0);

    *reg += value;

    state->registers.pc++;
    cycle_cost = 2;
}

// FLOW INSTRUCTIONS
void jr_s8(int8_t steps) {
    state->registers.pc += steps;
    cycle_cost = 3;
}

void jr_nz_s8(int8_t steps) {
    if (get_flag(ZERO) == 0) {
        state->registers.pc += steps;
        cycle_cost = 2;

        return;
    }

    state->registers.pc += 2;
    cycle_cost = 3;
}

void jr_z_s8(int8_t steps) {
    if (get_flag(ZERO) == 1) {
        state->registers.pc += steps;
        cycle_cost = 2;

        return;
    }

    state->registers.pc += 2;
    cycle_cost = 3;
}

void set_flag(flag_t target, uint8_t set) {
    // Zero - Set when something becomes 0, simples.
    // Carry - Set when 8 bit addition > 0xFF, 16 bit addition > 0xFFFF, subtraction < 0, when rotation shifts out a "1".
    // Sub - Is instruction a subtraction operation? Simples.
    // Half - Set when there is a carry out of bit 3 (u8 xxxx Xxxx) or bit 11 (u16 (xxxx Xxxx xxxx xxxx).

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

int get_flag(flag_t flag) {
    uint8_t* reg_f = (uint8_t*)&state->registers.af + 1;

    switch (flag) {
        case ZERO: return *reg_f >> 7;
        case SUB: return (*reg_f & MASK6) >> 6;
        case HALF: return (*reg_f & MASK5) >> 5;
        case CARRY: return (*reg_f & MASK4) >> 4;
        default: break;
    }
}