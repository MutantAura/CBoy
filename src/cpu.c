#include <stdio.h>

#include "device.h"

void TickCpu(Device* device) {
    // If an instruction was fetched last cycle, execute it this cycle and fetch the next instruction.
    // TODO: check if program counter is placed at fetch op or excecute op? 
    if (device->cpuState.fetch_op != NULL) {
        device->cpuState.exec_op = device->cpuState.fetch_op;
    }
    device->cpuState.fetch_op = &device->ram[device->cpuState.registers.pc];

    switch(device->cpuState.exec_op[0]) {

    }
}