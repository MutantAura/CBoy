#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "config/user_config.h"
#include "config/device_config.h"
#include "cpu/optable.h"
#include "loader/cart.h"
#include "models/constants.h"
#include "models/device.h"

SDL_Window* main_window;
SDL_Renderer* renderer;
SDL_Event queue;

// Prototypes
int init_sdl(config_t*);
void process_queue(SDL_Event*, device_t*);
device_t* init_gb_device(char*);

int main(int argc, char** argv) {
    
    // 1. Handle CLI args
    config_t* config = create_config(argc, argv);

    // 2. Init GB device
    device_t* device = init_gb_device(config);

    // 3. Init SDL
    if (init_sdl(config) == 0) {
        puts("SDL failed to init. Closing...");
        goto cleanup;
    }
    
    // 4. Verify and Load ROM
    loader_result result = load_rom(argv[1], &device->memory_pool);
    if (result != SUCCESS) {
        puts("ERROR: Fatal loader error.");
        goto cleanup;
    }

    cart_header* rom_header = parse_header(device->rom_bank_0);
    device_ctx context = create_device_context(device, rom_header);

    // 5. Enter SDL loop
    while (device->power_state == POWER_ON) {
        // 5a. Poll input
        process_queue(&queue, device);

        // 5b. Tick CPU
        int cycles = 0;
        while (cycles < CPU_FREQUENCY) {
            cycles += tick_cpu(&device->cpu_state, device->memory_pool);
        }
        
        // 5c. Render

        // 5d. Update misc items.
        // TODO

        // 5e. Update render timer.
        // TODO

        SDL_Delay(1000/VERT_SYNC);
    }

cleanup:
    // 6. Cleanup
    free(device);
    free(config);
    free(rom_header);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}

int init_sdl(config_t* config) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)) {
        return 0;
    }

    renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return 0;
    }

    return 1;
}

device_t* init_gb_device(char* rom_path) {
    device_t* device = calloc(1, sizeof(device_t));

    device->rom_bank_0 = &device->memory_pool[ROM0_START];
    device->rom_bank_1 = &device->memory_pool[ROM1_START];
    device->vram = &device->memory_pool[VRAM_START];
    device->cart_ram = &device->memory_pool[CART_START];
    device->wram = &device->memory_pool[WRAM0_START];
    device->echo_ram = &device->memory_pool[ECHO_START];
    device->oa_ram = &device->memory_pool[OBJ_START];
    device->reserved = &device->memory_pool[NA_START];
    device->io_regs = &device->memory_pool[IO_START];
    device->high_ram = &device->memory_pool[HRAM_START];
    device->intrpt_reg = &device->memory_pool[INTRPT_START];

    device->power_state = POWER_ON;

    return device;
}

void process_queue(SDL_Event* event, device_t* device) {
    while (SDL_PollEvent(event) != 0) {
        switch (event->type) {
            case SDL_QUIT: device->power_state = POWER_OFF; return;
            case SDL_KEYDOWN:
                switch(event->key.keysym.sym) {
                    case SDLK_ESCAPE: device->power_state = POWER_OFF; return;
                }
        }
    }
}
