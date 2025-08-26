#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cart.h"

uint8_t* load_rom(char* file_name) {
    FILE* rom = fopen(file_name, "rb");
    if (rom == NULL) {
        puts("Failed to load ROM file from specified location. Aborting...");
        return NULL;
    }

    fseek(rom, 0L, SEEK_END);
    int size = ftell(rom);
    fseek(rom, 0L, SEEK_SET);

    uint8_t* cart_buffer = malloc(size);

    int result = fread(cart_buffer, sizeof(uint8_t), size, rom);
    if (result != size) {
        puts("Failed to read the loaded ROM into emulated memory.");
        return NULL;
    }

    if (fclose(rom) != 0) {
        puts("Failed to close file handle!! Continuing...");
        return NULL;
    }

    return cart_buffer;
}

cart_header* parse_header(uint8_t* buffer) {
    cart_header* new_header = malloc(sizeof(cart_header));
    if (new_header == NULL) {
        puts("Failed to allocate ROM header.");
        return NULL;
    }

    memcpy(new_header, &buffer[CART_HEADER_START], sizeof(cart_header));

    return new_header;
}