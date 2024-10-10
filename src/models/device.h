#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <stdint.h>
#include "cpu.h"

typedef struct {
    cpu_t cpu_state;

    // Total memory all in a single allocated pool. 
    uint8_t memory_pool[0xFFFF];

    // Pointers below will point to the start of specific blocks of `memory_pool`.
    uint8_t* rom_bank_0;
    uint8_t* rom_bank_1;
    uint8_t* vram;
    uint8_t* cart_ram;
    uint8_t* wram;
    uint8_t* echo_ram;
    uint8_t* oa_ram;
    uint8_t* reserved;
    uint8_t* io_regs;
    uint8_t* high_ram;
    uint8_t* intrpt_reg;

    uint8_t power_state;
} device_t;

#endif