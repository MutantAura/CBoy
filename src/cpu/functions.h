#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdint.h>

#define MASK0 0b0001
#define MASK1 0b0010
#define MASK3 0b0100
#define MASK4 0b1000
#define MASK5 0b00010000
#define MASK6 0b00100000
#define MASK7 0b01000000
#define MASK8 0b10000000
#define MASK_8_LOW4 0b00001111
#define MASK_8_HIGH4 0b11110000

void ld_r16_d16(uint16_t*);
void ld_ra16_r8(uint16_t, uint8_t);
void inc_r16(uint16_t*);
void inc_r8(uint8_t*);
void inc_ra16(uint16_t);
void dec_r16(uint16_t*);
void dec_r8(uint8_t*);
void dec_ra16(uint16_t);
void adc_A_r8(uint16_t*);
void set_flags(flag_t, uint8_t);

#endif