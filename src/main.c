#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "device/constants.h"
#include "device/objects.h"
#include "cpu/cpu.h"

SDL_Window* main_window;
SDL_Renderer* renderer;
SDL_Event event;

// Prototypes
int init_sdl(void);
void input_helper(SDL_Event*, device_t*);

int main(void) {
    
    // 1. Handle CLI args
    // TODO

    // 2. Init SDL
    if (init_sdl() == 0) {
        puts("SDL failed to init. Closing...");
        return 0;
    }

    // 3. Init GB device
    device_t* device = malloc(sizeof(device_t));
    memset(device, 0, sizeof(device_t));
    device->power_state = POWER_ON;
    
    // 4. Verify and Load ROM

    // 5. Enter SDL loop
    while (device->power_state == POWER_ON) {
        // 5a. Poll input
        while (SDL_PollEvent(&event) != 0) {
            input_helper(&event, device);
        }   

        // 5b. Tick CPU
        
        // 5c. Render

        // 5d. Update misc items.
        // TODO

        // 5e. Update render timer.
        // TODO
    }

    // 6. Cleanup
    free(device);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}

int init_sdl(void) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)) {
        return 0;
    }

    main_window = SDL_CreateWindow("CBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
    if (main_window == NULL) {
        return 0;
    }

    renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return 0;
    }

    return 1;
}

void input_helper(SDL_Event* event, device_t* device) {
    switch (event->type) {
        case SDL_QUIT: device->power_state = POWER_OFF; break;
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
                case SDLK_ESCAPE: device->power_state = POWER_OFF; break;
            }
    }
}
