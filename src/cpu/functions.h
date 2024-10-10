#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdint.h>

#define MASK0 0x1
#define MASK1 0x2
#define MASK3 0x4
#define MASK4 0x8
#define MASK5 0x10
#define MASK6 0x20
#define MASK7 0x40
#define MASK8 0x80
#define MASK_LOW_4 0x0F
#define MASK_HIGH_8 0xF0

void ld_r16_d16(uint16_t*);
void ld_ra16_r8(uint16_t, uint8_t);
void inc_r16(uint16_t*);
void inc_r8(uint8_t*);
void inc_ra16(uint16_t);
void dec_r16(uint16_t*);
void dec_r8(uint16_t*);
void adc_A_r8(uint16_t*);
void set_flags(flag_t, uint8_t);

#endif