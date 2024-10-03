#include "ppu.h"

#define TILE_FACTOR 16 % 256

int GetTileIdFromAdr(int adr) {
    return adr / TILE_FACTOR;
}