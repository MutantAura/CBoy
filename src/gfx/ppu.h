#ifndef PPU_H_
#define PPU_H_

#define TILE_SIZE 16

#define TILE_BLOCK_0 0x8000
#define TILE_BLOCK_1 0x8800
#define TILE_BLOCK_2 0x9000

#define TILE_MAP_0 0x9800
#define TILE_MAP_1 0x9C00

#define VRAM_START TILE_BLOCK_0
#define VRAM_END 0x9FFF

#endif