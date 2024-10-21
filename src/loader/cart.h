#ifndef CART_H_
#define CART_H_

#include <stdint.h>
#include "../models/constants.h"

typedef enum cart_type {
    ROM_ONLY = 0,
    MBC1 = 1,
    MBC1_RAM = 2,
    MBC1_RAM_BATTERY = 3,
    MBC2 = 5,
    MBC2_BATTERY = 6,
    ROM_RAM = 8,
    ROM_RAM_BATTERY = 9,
    MMM01 = 11,
    MMM01_RAM = 12,
    MMM01_RAM_BATTERY = 13,
    MBC3_TIMER_BATTERY = 15,
    MBC3_TIMER_RAM_BATTERY = 16,
    MBC3 = 17,
    MBC3_RAM = 18,
    MBC_RAM_BATTERY = 19,
    MBC5 = 25,
    MBC5_RAM = 26,
    MBC5_RAM_BATTERY = 27,
    MBC5_RUMBLE = 28,
    MBC5_RUMBLE_RAM = 29,
    MBC5_RUMBLE_RAM_BATTERY = 30,
    MBC6 = 32,
    MBC7_SENSOR_RUMBLE_RAM_BATTERY = 34,
    POCKET_CAM = 252,
    BANDAI_TAMA5 = 253,
    HUC3 = 254,
    HUC1_RAM_BATTERY = 255
} cart_type;

typedef struct cart_header {
    uint8_t entry_op1;
    uint8_t entry_op2;

    uint8_t boot_logo[47];

    union {
        struct {
            char title[15];
            char dev_code[3];
            uint8_t cgb_flag;
        };
        char old_title[19];
    };
    char new_license[2]; // Only use if old_license == 0x33
    uint8_t sgb_flag;

    cart_type type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t dest_code;
    uint8_t old_license;
    uint8_t game_version;

    uint8_t local_checksum;
    uint16_t global_checksum;
} cart_header;

typedef enum loader_result {
    FAILURE,
    SUCCESS,
    UNKNOWN
} loader_result;

loader_result load_rom(char*, uint8_t*);
cart_header* parse_header(uint8_t*);

#endif