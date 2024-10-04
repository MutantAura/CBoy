#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include "../device/constants.h"

typedef struct control_t {

} control_t;

typedef struct transport_t {
    uint8_t data;
    uint16_t address;
} transport_t;

typedef struct registers_t {
    // 16-bit special registers
    uint16_t pc;
    uint16_t sp;

    // 8 bit special registers
    uint8_t ir;
    uint8_t ie;

    // a = accumulator register
    // f = flag register
    // b-e = generic registers
    uint16_t af;
    uint16_t bc;
    uint16_t de;
    uint16_t hl;

} registers_t;

typedef struct arith_t {
    uint8_t i1;
    uint8_t i2;
} arith_t;

typedef struct cpu_t {
    control_t control;
    registers_t registers;
    arith_t arith;
    transport_t buses;
    uint16_t id;

    uint8_t* exec_op;
    uint8_t* fetch_op;
} cpu_t;

int tick_cpu(cpu_t*, uint8_t*);

#endif