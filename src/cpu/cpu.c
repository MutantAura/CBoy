#include <stdio.h>

#include "../device/objects.h"

void tick_cpu(device_t* device) {
    // If an instruction was fetched last cycle, execute it this cycle and fetch the next instruction.
    // TODO: check if program counter is placed at fetch op or excecute op? 
    if (device->cpu_state.fetch_op != NULL) {
        device->cpu_state.exec_op = device->cpu_state.fetch_op;
    }
    device->cpu_state.fetch_op = &device->ram[device->cpu_state.registers.pc];

    // Opcode switch tree - best way to group common opcodes?
    // TODO: Generic functions for similar opcodes?
    switch(device->cpu_state.exec_op[0]) {
        
    }
}