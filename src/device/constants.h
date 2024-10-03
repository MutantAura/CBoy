#ifndef CONSTANTS_H
#define CONSTANTS_H

// Display constants
#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144

#define HORI_SYNC 9198
#define VERT_SYNC 60 // 59.73 think about it later...

// Memory constants
#define DMG_RAM 8192
#define GBC_RAM 8192 * 4

#define DMG_VRAM 8192
#define GBC_VRAM 8192 * 2

// Memory map
#define ROM0_START 0x0000
#define ROM1_STAR 0x4000
#define VRAM_START 0x8000
#define CART_START 0xA000
#define WRAM0_START 0xC000
#define WRAM1_START 0xD000
#define ECHO_START 0xE000
#define OBJ_START 0xFE00
#define NA_START 0xFEA0
#define IO_START 0xFF00
#define HRAM_START 0xFF80
#define INTRPT_START 0xFFFF

// Audio constants
#define AUDIO_CHANNELS 4
#define AUDIO_OUTPUT 2

typedef enum power_state {
    POWER_OFF,
    POWER_ON
} power_state;

#endif
