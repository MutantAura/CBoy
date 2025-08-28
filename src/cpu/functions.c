#include <stdio.h>

#include "functions.h"
#include "optable.h"
#include "../models/cpu.h"

// INFO: All functions cycle_cost = their cycle cost.
// TODO: Check if variable cost instructions are true/false coded e.g. jr_nz_s8 which path is 3/2?

// LOAD INSTRUCTIONS
void ld_r16_d16(uint16_t* reg) {
    *reg = (state->exec_op[2] << 8) | state->exec_op[1];

    regs->pc.reg16 += 3;
    cycle_cost = 3;
}

void ld_ra16_r8(uint16_t adr, uint8_t value) {
    ram[adr] = value;

    regs->pc.reg16++;
    cycle_cost = 2;
}

void ld_r8_d8(uint8_t* reg) {
    *reg = state->exec_op[1];

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void ld_r8_r8(uint8_t* reg, uint8_t value) {
    *reg = value;

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void ld_a16_sp16() {
    uint16_t adr = *((uint16_t*)state->exec_op);

    ram[adr] = regs->sp.low;
    ram[adr++] = regs->sp.high;

    regs->pc.reg16 += 3;
    cycle_cost = 5;
}

void ld_r8_ra16(uint8_t* reg, uint16_t adr) {
    *reg = ram[adr];

    regs->pc.reg16++;
    cycle_cost = 2;
}

void ld_ra16_d8(uint16_t adr) {
    ram[adr] = state->exec_op[1];

    regs->pc.reg16 += 3;
    cycle_cost = 2;
}

void ld_a16_r8(uint8_t value) {
    ram[(state->exec_op[2] << 8) | state->exec_op[1]] = value;

    regs->pc.reg16 += 3;
    cycle_cost = 4;
}

void ld_r16_sps8(uint16_t* reg) {
    uint16_t new_sp = regs->sp.reg16 + (int8_t)state->exec_op[1];

    set_flag(ZERO, 0);
    set_flag(HALF, 0);
    set_flag(SUB, (int8_t)state->exec_op[1] < 0);
    // TODO: figure out signed 16-bit carry?

    *reg = new_sp;

    regs->pc.reg16 += 2;
    cycle_cost = 3;
}

void ld_r16_r16(uint16_t value, uint16_t* reg) {
    *reg = value;

    regs->pc.reg16 += 1;
    cycle_cost = 2;
}

void ld_r8_a16(uint8_t* reg) {
    *reg = ram[(state->exec_op[2] << 8) | state->exec_op[1]];

    regs->pc.reg16 += 3;
    cycle_cost = 4;
}

// INCREMENT/DECREMENT INSTRUCTIONS
void inc_r16(uint16_t* reg) {
    (*reg)++;

    regs->pc.reg16++;
    cycle_cost = 2;
}

void inc_r8(uint8_t* reg) {
    (*reg)++;

    set_flag(ZERO, *reg == 0);
    set_flag(HALF, (*reg & MASK8_LOW4) == 0);
    set_flag(SUB, 0);

    regs->pc.reg16++;
    cycle_cost = 1;
}

void inc_ra16(uint16_t adr) {
    ram[adr]++;

    set_flag(ZERO, ram[adr] == 0);
    set_flag(HALF, (ram[adr] & MASK8_LOW4) == 0);
    set_flag(SUB, 0);

    regs->pc.reg16++;
    cycle_cost = 3;
}

void dec_r16(uint16_t* reg) {
    (*reg)--;

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void dec_r8(uint8_t* reg) {
    (*reg)--;

    set_flag(ZERO, *reg == 0);
    set_flag(HALF, *reg == 0xFF); // Only possible if underflow (0-1 -> 0xFF)
    set_flag(SUB, 1);

    regs->pc.reg16++;
    cycle_cost = 1;
}

void dec_ra16(uint16_t adr) {
    ram[adr]--;

    set_flag(ZERO, ram[adr] == 0);
    set_flag(HALF, ram[adr] == 0xFF); // Only possible if underflow (0-1 -> 0xFF)
    set_flag(SUB, 1);

    regs->pc.reg16++;
    cycle_cost = 3;
}

// GENERAL ARITHMETIC INSTRUCTIONS
void add_r16_r16(uint16_t* reg, uint16_t value) {
    uint32_t overflow = *reg + value;

    set_flag(CARRY, overflow > UINT16_MAX);
    set_flag(HALF, ((*reg & MASK16_LOW12) + (value & MASK16_LOW12)) >> 12);
    set_flag(SUB, 0);

    *reg += value;

    regs->pc.reg16++;
    cycle_cost = 2;
}

void add_r8_r8(uint8_t* reg, uint8_t value) {
    uint16_t overflow = *reg + value;

    set_flag(ZERO, *reg + value == 0);
    set_flag(CARRY, overflow > UINT8_MAX);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 0);

    *reg += value;

    regs->pc.reg16++;
    cycle_cost = 1;
}

 void add_r8_d8(uint8_t* reg) {
    uint16_t overflow = *reg + state->exec_op[1];

    set_flag(ZERO, *reg + state->exec_op[1] == 0);
    set_flag(CARRY, overflow > UINT8_MAX);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (state->exec_op[1] & MASK8_LOW4)) >> 4);
    set_flag(SUB, 0);

    *reg += state->exec_op[1];

    regs->pc.reg16 += 2;
    cycle_cost = 2;
 }

void add_r8_ra16(uint8_t* reg, uint16_t adr) {
    uint16_t overflow = *reg + ram[adr];

    set_flag(ZERO, *reg + ram[adr] == 0);
    set_flag(CARRY, overflow > UINT8_MAX);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (ram[adr] & MASK8_LOW4)) >> 4);
    set_flag(SUB, 0);

    *reg += ram[adr];

    regs->pc.reg16++;
    cycle_cost = 2;
}

void add_r16_s8(uint16_t* reg) {
    uint16_t value = *reg + state->exec_op[1];

    set_flag(ZERO, 0);
    set_flag(CARRY, 0);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (state->exec_op[1] & MASK8_LOW4) >> 4));
    set_flag(SUB, state->exec_op < 0);

    *reg += state->exec_op[1];

}

void adc_r8_r8(uint8_t* reg, uint8_t value) {
    value += get_flag(CARRY);
    uint16_t overflow = *reg + value;

    set_flag(ZERO, *reg + value == 0);
    set_flag(CARRY, overflow > UINT8_MAX);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 0);

    *reg += value;

    regs->pc.reg16++;
    cycle_cost = 1;
}

void adc_r8_d8(uint8_t* reg) {
    uint8_t value = state->exec_op[1] + get_flag(CARRY);
    uint16_t overflow = *reg + value;

    set_flag(ZERO, *reg + value == 0);
    set_flag(CARRY, overflow > UINT8_MAX);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 0);

    *reg += value;

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void adc_r8_ra16(uint8_t* reg, uint16_t adr) {
    uint8_t value = ram[adr] + get_flag(CARRY);
    uint16_t overflow = *reg + value;

    set_flag(ZERO, *reg + value == 0);
    set_flag(CARRY, overflow > UINT8_MAX);
    set_flag(HALF, ((*reg & MASK8_LOW4) + (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 0);

    *reg += value;

    regs->pc.reg16++;
    cycle_cost = 2;
}

void sub_r16_r16(uint16_t* reg, uint16_t value) {
    uint16_t underflow = *reg - value;

    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK16_LOW12) - (value & MASK16_LOW12)) >> 12);
    set_flag(SUB, 1);

    *reg -= value;

    regs->pc.reg16++;
    cycle_cost = 2;
}

void sub_r8_r8(uint8_t* reg, uint8_t value) {
    uint8_t underflow = *reg - value;

    set_flag(ZERO, underflow == 0);
    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK8_LOW4) - (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 1);

    *reg -= value;

    regs->pc.reg16++;
    cycle_cost = 1;
}

void sub_r8_ra16(uint8_t* reg, uint16_t adr) {
    uint8_t underflow = *reg - ram[adr];

    set_flag(ZERO, underflow == 0);
    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK8_LOW4) - (ram[adr] & MASK8_LOW4)) >> 4);
    set_flag(SUB, 1);

    *reg -= ram[adr];

    regs->pc.reg16++;
    cycle_cost = 2;
}

void sub_r8_d8(uint8_t* reg) {
    uint8_t underflow = *reg - state->exec_op[1];

    set_flag(ZERO, underflow == 0);
    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK8_LOW4) - (state->exec_op[1] & MASK8_LOW4)) >> 4);
    set_flag(SUB, 1);

    *reg -= state->exec_op[1];

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void sbc_r8_r8(uint8_t* reg, uint8_t value) {
    value -= get_flag(CARRY);
    uint8_t underflow = *reg - value;

    set_flag(ZERO, *reg + value == 0);
    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK8_LOW4) - (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 1);

    *reg -= value;

    regs->pc.reg16++;
    cycle_cost = 1;
}

void sbc_r8_ra16(uint8_t* reg, uint16_t adr) {
    uint8_t value = ram[adr] - get_flag(CARRY);
    uint8_t underflow = *reg - value;

    set_flag(ZERO, underflow == 0);
    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK8_LOW4) - (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 1);

    *reg -= value;

    regs->pc.reg16++;
    cycle_cost = 2;
}

void sbc_r8_d8(uint8_t* reg) {
    uint8_t value = state->exec_op[1] - get_flag(CARRY);
    uint8_t underflow = *reg - value;

    set_flag(ZERO, underflow == 0);
    set_flag(CARRY, underflow > *reg);
    set_flag(HALF, ((*reg & MASK8_LOW4) - (value & MASK8_LOW4)) >> 4);
    set_flag(SUB, 1);

    *reg -= value;

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

// BITWISE INSTRUCTIONS
void and_r8_r8(uint8_t* reg, uint8_t value) {
    *reg &= value;

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 1);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 1;
}

void and_r8_ra16(uint8_t* reg, uint16_t adr) {
    *reg &= ram[adr];

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 1);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 2;
}

void and_d8(uint8_t* reg) {
    *reg &= state->exec_op[1];

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 1);
    set_flag(CARRY, 0);

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void xor_r8_r8(uint8_t* reg, uint8_t value) {
    *reg ^= value;

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 0);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 1;
}

void xor_r8_ra16(uint8_t* reg, uint16_t adr) {
    *reg ^= ram[adr];

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 0);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 2;
}

void xor_r8_d8(uint8_t* reg) {
    *reg ^= state->exec_op[1];

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void or_r8_r8(uint8_t* reg, uint8_t value) {
    *reg |= value;

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 0);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 1;
}

void or_r8_r16(uint8_t* reg, uint16_t adr) {
    *reg |= ram[adr];

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 0);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 2;
}

void or_r8_ra16(uint8_t* reg, uint16_t adr) {
    *reg |= ram[adr];

    set_flag(ZERO, *reg == 0);
    set_flag(SUB, 0);
    set_flag(HALF, 0);
    set_flag(CARRY, 0);

    regs->pc.reg16++;
    cycle_cost = 2;
}

void or_r8_d8(uint8_t* reg) {
    *reg |= state->exec_op[1];

    regs->pc.reg16 += 2;
    cycle_cost = 2;
}

void cmp_r8_r8(uint8_t val1, uint8_t val2) {
    uint16_t underflow = val1 - val2;

    set_flag(ZERO, underflow == 0);
    set_flag(SUB, 1);
    set_flag(HALF, ((val1 & MASK8_LOW4) - (val2 & MASK8_LOW4)) >> 4);
    set_flag(CARRY, underflow > UINT8_MAX);

    regs->pc.reg16++;
    cycle_cost = 1;
}

void cmp_r8_ra16(uint8_t val1, uint16_t adr) {
    uint16_t underflow = val1 - ram[adr];

    set_flag(ZERO, underflow == 0);
    set_flag(SUB, 1);
    set_flag(HALF, ((val1 & MASK8_LOW4) - (ram[adr] & MASK8_LOW4)) >> 4);
    set_flag(CARRY, underflow > UINT8_MAX);

    regs->pc.reg16++;
    cycle_cost = 2;
}

// FLOW INSTRUCTIONS
void jr_s8(int8_t steps) {
    regs->pc.reg16 += steps;
    cycle_cost = 3;
}

void jr_nz_s8(int8_t steps) {
    if (get_flag(ZERO) == 0) {
        regs->pc.reg16 += steps;
        cycle_cost = 2;

        return;
    }

    regs->pc.reg16 += 2;
    cycle_cost = 3;
}

void jr_z_s8(int8_t steps) {
    if (get_flag(ZERO) == 1) {
        regs->pc.reg16 += steps;
        cycle_cost = 2;

        return;
    }

    regs->pc.reg16 += 2;
    cycle_cost = 3;
}

void jr_nc_s8(int8_t steps) {
    if (get_flag(CARRY) == 0) {
        regs->pc.reg16 += steps;
        cycle_cost = 2;

        return;
    }

    regs->pc.reg16 += 2;
    cycle_cost = 3;
}

void jr_c_s8(int8_t steps) {
    if (get_flag(CARRY == 1)) {
        regs->pc.reg16 += steps;
        cycle_cost = 2;

        return;
    }

    regs->pc.reg16 += 2;
    cycle_cost = 3;
}

void jmp() {
    uint16_t adr = regs->pc.reg16;
    regs->pc.high = ram[adr + 1];
    regs->pc.low = ram[adr + 2];

    cycle_cost = 4;
}

void jmp_nf(flag_t flag) {
    if (get_flag(flag) == 1) {
        uint16_t adr = regs->pc.reg16;
        regs->pc.high = ram[adr + 1];
        regs->pc.low = ram[adr + 2];

        cycle_cost = 4;

        return;
    }

    cycle_cost = 3;
}

void jmp_f(flag_t flag) {
    if (get_flag(flag) == 0) {
        uint16_t adr = regs->pc.reg16;
        regs->pc.high = ram[adr + 1];
        regs->pc.low = ram[adr + 2];

        cycle_cost = 4;

        return;
    }

    cycle_cost = 3;
}

void jmp_r16(uint16_t adr) {
    regs->pc.reg16 = adr;

    cycle_cost = 1;
}

// Stack flow/control opc.reg16odes
void ret_nf(flag_t flag) {
    if (get_flag(flag) == 0) {
        regs->pc.low = ram[regs->sp.reg16];
        regs->pc.high = ram[regs->sp.reg16 + 1];

        regs->sp.reg16 += 2;

        cycle_cost = 5;
        
        return;
    }

    regs->pc.reg16++;
    cycle_cost = 2;
}

void ret_f(flag_t flag) {
 if (get_flag(flag) == 1) {
        regs->pc.low = ram[regs->sp.reg16];
        regs->pc.high = ram[regs->sp.reg16 + 1];

        regs->sp.reg16 += 2;

        cycle_cost = 5;

        return;
    }

    regs->pc.reg16++;
    cycle_cost = 2;
 }

 void ret() {
    regs->pc.low = ram[regs->sp.reg16];
    regs->pc.high = ram[regs->sp.reg16 + 1];

    regs->sp.reg16 += 2;

    cycle_cost = 4;
 }

 void pop_r16(reg16_t* reg) {
    reg->low = ram[regs->sp.reg16];
    reg->high = ram[regs->sp.reg16 + 1];

    regs->sp.reg16 += 2;

    regs->pc.reg16++;
    cycle_cost = 3;
 }

 void push_r16(reg16_t* reg) {
    ram[regs->sp.reg16 - 1] = reg->high;
    ram[regs->sp.reg16 - 2] = reg->low;
    regs->sp.reg16 -= 2;

    regs->pc.reg16++;
    cycle_cost = 4;
 }

 void call_a16() {
    ram[regs->sp.reg16 - 1] = regs->pc.reg16 + 3;
    regs->sp.reg16--;

    regs->pc.high = state->exec_op[2];
    regs->pc.low = state->exec_op[1];

    cycle_cost = 6;
 }

 void call_nf_a16(flag_t flag) {
    if (get_flag(flag) == 0) {
        // CALL instruction is 3 bytes long. Push next instruction to the stack.
        ram[regs->sp.reg16 - 1] = regs->pc.reg16 + 3;
        regs->sp.reg16--;

        regs->pc.high = state->exec_op[2];
        regs->pc.low = state->exec_op[1];

        cycle_cost = 6;
    }

    regs->pc.reg16 += 3;
    cycle_cost = 3;
 }

 void call_f_a16(flag_t flag) {
    if (get_flag(flag) == 1) {
        // CALL instruction is 3 bytes long. Push next instruction to the stack.
        ram[regs->sp.reg16 - 1] = regs->pc.reg16 + 3;
        regs->sp.reg16--;

        regs->pc.high = state->exec_op[2];
        regs->pc.low = state->exec_op[1];

        cycle_cost = 6;
    }

    regs->pc.reg16 += 3;
    cycle_cost = 3;
 }

 void rst(uint8_t type) {
    push_r16(&regs->pc);

    regs->pc.high = 0x00;
    regs->pc.low = type;

    cycle_cost = 4;
 }

void set_flag(flag_t target, int set) {
    // Zero - Set when something becomes 0, simples.
    // Carry - Set when 8 bit addition > 0xFF, 16 bit addition > 0xFFFF, subtraction < 0, when rotation shifts out a "1".
    // Sub - Is instruction a subtraction operation? Simples.
    // Half - Set when there is a carry out of bit 3 (u8 xxxx Xxxx) or bit 11 (u16 (xxxx Xxxx xxxx xxxx).
    if (set) {
        switch (target) {
            case ZERO: regs->af.low |= MASK7; return;
            case SUB: regs->af.low |= MASK6; return;
            case HALF: regs->af.low |= MASK5; return;
            case CARRY: regs->af.low |= MASK4; return;
            default: puts("Failed to set flag, type unknown."); break;
        }
    } else {
        switch (target) {
            case ZERO: regs->af.low &= ~MASK7; return;
            case SUB: regs->af.low &= ~MASK6; return;
            case HALF: regs->af.low &= ~MASK5; return;
            case CARRY: regs->af.low &= ~MASK4; return;
            default: puts("Failed to set flag, type unknown."); break;
        }
    }
}

int get_flag(flag_t flag) {
    switch (flag) {
        case ZERO: return regs->af.low >> 7; break;
        case SUB: return (regs->af.low & MASK6) >> 6; break;
        case HALF: return (regs->af.low & MASK5) >> 5; break;
        case CARRY: return (regs->af.low & MASK4) >> 4; break;
        default: return 2; // TODO: Nasty stuff, re-think later.
    }
}

void flip_flag(flag_t flag) {
    set_flag(flag, !get_flag(flag));
}