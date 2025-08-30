#ifndef OPTABLE_H_
#define OPTABLE_H_

#include <stdint.h>
#include "../models/cpu.h"

extern cpu_t* state;
extern uint8_t* ram;
extern registers_t* regs;
extern int cycle_cost;

int tick_cpu(cpu_t*, uint8_t*, uint8_t*);
void unimplemented_exception(char* name, int cost, int pc);

#endif