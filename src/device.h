#include "cpu.h"

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144

typedef struct {
    Cpu cpuState;

    uint8_t ram[8192];
    uint8_t framebuffer[8192];
} Device;