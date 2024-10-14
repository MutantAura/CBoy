#ifndef OPTABLE_H_
#define OPTABLE_H_

#include <stdint.h>
#include "../models/cpu.h"

cpu_t* state;
uint8_t* ram;
registers_t* regs;
int cycle_cost;

int tick_cpu(cpu_t*, uint8_t*);

#endif