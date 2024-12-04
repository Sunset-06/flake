#include"Chip8.h"
#define PIXEL_SIZE 10  

uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


void initializeSDL() {
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

    window = SDL_CreateWindow("flake",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    memset(screen, 0, sizeof(screen));
}


void drawScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (screen[y * SCREEN_WIDTH + x]) {
                SDL_Rect pixel = { x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
} 

//-------------------------------------generated test main---------------------------------------

int main() {
    initializeSDL();
    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Simple test: toggle a few pixels on the screen
        memset(screen, 0, sizeof(screen));  // Clear screen

        // Draw a simple pattern (for example, a diagonal line)
        for (int i = 0; i < SCREEN_WIDTH && i < SCREEN_HEIGHT; ++i) {
            screen[i * SCREEN_WIDTH + i] = 1; // Turn on diagonal pixels
        }


        drawScreen(); 

        // Delay for a bit to control the refresh rate
        SDL_Delay(100); 
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}