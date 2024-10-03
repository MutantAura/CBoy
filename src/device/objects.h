#ifndef OBJECTS_H
#define OBJECTS_H

#include "../cpu/cpu.h"
#include "constants.h"

typedef struct {
    cpu_t cpu_state;

    uint8_t ram[DMG_RAM];
    uint8_t framebuffer[DMG_VRAM];

    uint8_t power_state;
} device_t;

#endif