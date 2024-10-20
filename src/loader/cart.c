#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cart.h"

loader_result load_rom(char* file_name, uint8_t* ram) {
    FILE* rom = fopen(file_name, "rb");
    if (rom == NULL) {
        puts("Failed to load ROM file from specified location");
        return FAILURE;
    }

    fseek(rom, 0L, SEEK_END);
    int size = ftell(rom);
    fseek(rom, 0L, SEEK_SET);

    int result = fread(ram, size, 1, rom);
    if (result != 1) {
        puts("Failed to read the loaded ROM into emulated memory.");
        return FAILURE;
    }

    if (fclose(rom) != 0) {
        puts("Failed to close file handle!! Continuing...");
    }

    return SUCCESS;
}

cart_header parse_header(uint8_t* ram) {
    cart_header new_header;

    memcpy(&new_header, ram[ROM0_START], sizeof(cart_header));

    return new_header;
}