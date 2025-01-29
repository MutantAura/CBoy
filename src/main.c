#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "config/user_config.h"
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
device_t* init_gb_device(config_t*);

int main(int argc, char** argv) {
    
    // 1. Handle CLI args
    config_t* config = create_config(argc, argv);
    if (config == NULL) {
        puts("Failed to create user-configuration. Aborting...");
        goto cleanup4;
    }

    // 2. Init GB device
    device_t* device = init_gb_device(config);

    // 3. Init SDL
    if (init_sdl(config) == 0) {
        puts("Failed to init SDL. Aborting...");
        goto cleanup3;
    }
    
    // 4. Verify and Load ROM
    device->cart.buffer = load_rom(argv[1]);
    if (device->cart.buffer == NULL) {
        puts("Failed to load user-provided ROM file. Aborting...");
        goto cleanup2;
    }

    // 4b. Create ROM header
    device->cart.header = parse_header(device->cart.buffer);
    if (device->cart.header == NULL) {
        puts("Failed to create ROM header. Aborting...");
        goto cleanup;
    }

    // 4c. Setup initial device state based on header data.
    // TODO: Boot-ROM and don't hardcode this value, read from header.
    device->cpu_state.fetch_op = device->rom_bank_0 + 0x100;

    // 5. Enter SDL loop
    while (device->power_state == POWER_ON) {
        // 5a. Poll input
        process_queue(&queue, device);

        // 5b. Tick CPU
        int cycles = 0;
        while (cycles < CPU_FREQUENCY) {
            int cost = tick_cpu(&device->cpu_state, device->memory_pool);

            // An unimplemented instruction will return a cycle cost of -1, in this event, die.
            if (cost == -1) {
                device->power_state = POWER_OFF;
                break;
            }

            cycles += cost;
        }
        
        // 5c. Render

        // 5d. Update misc items.
        // TODO

        // 5e. Update render timer.
        // TODO

        SDL_Delay(1000/VERT_SYNC);
    }

    // 6. Cleanup
cleanup:
    free(device->cart.header);
cleanup2:
    free(device->cart.buffer);
cleanup3:
    free(device);
cleanup4:
    free(config);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}

int init_sdl(config_t* config) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)) {
        return 0;
    }

    main_window = SDL_CreateWindow("CBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                    DISPLAY_WIDTH * config->display_scale,
                                    DISPLAY_HEIGHT * config->display_scale,
                                    0);

    renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return 0;
    }

    return 1;
}

device_t* init_gb_device(config_t* config) {
    device_t* device = calloc(1, sizeof(device_t));
    
    device->variant = config->variant;

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

    if (config->frequency_override == 0) {
        device->cpu_state.frequency = CPU_FREQUENCY;
    } else device->cpu_state.frequency = config->frequency_override;

    device->power_state = POWER_ON;

    return device;
}

void process_queue(SDL_Event* event, device_t* device) {
    while (SDL_PollEvent(event) != 0) {
        switch (event->type) {
            case SDL_QUIT: device->power_state = POWER_OFF; return;

            case SDL_WINDOWEVENT:
                switch (event->window.type) {
                    case SDL_WINDOWEVENT_MAXIMIZED: SDL_MaximizeWindow(main_window); break;
                    case SDL_WINDOWEVENT_MINIMIZED: SDL_MinimizeWindow(main_window); break;
                    case SDL_WINDOWEVENT_CLOSE: device->power_state = POWER_OFF; break;
                } break;

            case SDL_KEYDOWN:
                switch(event->key.keysym.sym) {
                    case SDLK_ESCAPE: device->power_state = POWER_OFF; return;
                } break;
        }
    }
}
