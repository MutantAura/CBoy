#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "../device/objects.h"

#define CPU_FREQUENCY 4'190'000

typedef struct {

} control_t;

typedef struct {
    uint8_t data;
    uint16_t address;
} transport_t;

typedef struct {
    // 16-bit special registers
    uint16_t pc;
    uint16_t sp;

    // 8 bit special registers
    uint8_t ir;
    uint8_t ie;

    // a = accumulator register
    // f = flag register
    // b-e = generic registers
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
} registers_t;

typedef struct {
    uint8_t i1;
    uint8_t i2;
} arith_t;

typedef struct {
    control_t control;
    registers_t registers;
    arith_t arith;
    transport_t buses;
    uint16_t id;

    uint8_t* exec_op;
    uint8_t* fetch_op;
} cpu_t;

#endif