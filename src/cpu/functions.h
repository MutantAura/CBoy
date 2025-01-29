#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdint.h>
#include "../models/cpu.h"

#define MASK0 0b0001
#define MASK1 0b0010
#define MASK2 0b0100
#define MASK3 0b1000
#define MASK4 0b00010000
#define MASK5 0b00100000
#define MASK6 0b01000000
#define MASK7 0b10000000
#define MASK8_LOW4 0b00001111
#define MASK8_HIGH4 0b11110000
#define MASK16_LOW8 0b0000000011111111
#define MASK16_HIGH8 0b1111111100000000
#define MASK16_LOW12 0b0000111111111111

void ld_r16_d16(uint16_t*);
void ld_ra16_r8(uint16_t, uint8_t);
void ld_a16_sp16();
void ld_r8_d8(uint8_t*);
void ld_r8_ra16(uint8_t*, uint16_t);
void ld_r8_r8(uint8_t*, uint8_t);
void ld_ra16_d8(uint16_t);

void inc_r16(uint16_t*);
void inc_r8(uint8_t*);
void inc_ra16(uint16_t);
void dec_r16(uint16_t*);
void dec_r8(uint8_t*);
void dec_ra16(uint16_t);

void add_r16_r16(uint16_t*, uint16_t);
void add_r8_r8(uint8_t*, uint8_t);
void add_r8_d8(uint8_t*);
void add_r8_ra16(uint8_t*, uint16_t);
void adc_r8_r8(uint8_t*, uint8_t);
void adc_r8_ra16(uint8_t*, uint16_t);
void adc_r8_d8(uint8_t*);

void sub_r16_r16(uint16_t*, uint16_t);
void sub_r8_r8(uint8_t*, uint8_t);
void sub_r8_ra16(uint8_t*, uint16_t);
void sbc_r8_r8(uint8_t*, uint8_t);
void sbc_r8_ra16(uint8_t*, uint16_t);

void and_r8_r8(uint8_t*, uint8_t);
void and_r8_ra16(uint8_t*, uint16_t);
void xor_r8_r8(uint8_t*, uint8_t);
void xor_r8_ra16(uint8_t*, uint16_t);
void or_r8_r8(uint8_t*, uint8_t);
void or_r8_r16(uint8_t*, uint16_t);
void or_r8_ra16(uint8_t*, uint16_t);
void cmp_r8_r8(uint8_t, uint8_t);
void cmp_r8_ra16(uint8_t, uint16_t);

void jr_s8(int8_t);
void jr_nz_s8(int8_t);
void jr_z_s8(int8_t);
void jr_nc_s8(int8_t);
void jr_c_s8(int8_t);
void jmp();
void jmp_z();
void jmp_nz();

void ret();
void ret_nz();
void ret_z();
void pop_r16(reg16_t*);
void push_r16(reg16_t*);
void call_a16();
void call_z_a16();
void call_nz_a16();

void rst(int);

void set_flag(flag_t, int);
int get_flag(flag_t);
void flip_flag(flag_t);

#endif