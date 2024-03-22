#include <stdint.h>

#define CPU_FREQUENCY 4'190'000

typedef struct {

} ControlUnit;

typedef struct {
    uint8_t data;
    uint16_t address;
} Transport;

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
} Registers;

typedef struct {
    uint8_t i1;
    uint8_t i2;
} ArithUnit;

typedef struct {
    ControlUnit control;
    Registers registers;
    ArithUnit arith;
    Transport buses;
    uint16_t id;

    uint8_t* exec_op;
    uint8_t* fetch_op;
} Cpu;
