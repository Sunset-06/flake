#include"Chip8.h"

uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

 SDL_Scancode keymappings[16] = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

//Handles keypresses
void handle_keypress() {
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        switch (event.type) {
            case SDL_QUIT:
                quit_flag = 1;
                break;
            default:
                if (state[SDL_SCANCODE_ESCAPE]) {
                    quit_flag = 1;
                }

                for (int i= 0;i < 16; i++) {
                    curr_key_state[i] = state[keymappings[i]];
                    //printf("%d is %d \n",i,curr_key_state[i]);
                }
                break;
        }
    }
}

//initializes the screen
void initializeScreen() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("flake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*8, 32*8, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    memset(screen, 0, sizeof(screen));
}

//draws it
void drawScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (screen[y * SCREEN_WIDTH + x]) {
                SDL_Rect pixel = { x * 8, y * 8, 8, 8 };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

//stop the emu
void endScreen(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//load the rom file
void LoadRom(char filename[]){
    FILE* file = fopen(filename, "rb");  
    if (file == NULL) {
        fprintf(stderr, "Failed to open ROM: %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    if ((filesize + START_ADDRESS) > sizeof(memory)) {
        fprintf(stderr, "Incorrect file. Too large to fit in memory.\n");
        fclose(file);
        exit(1);
    }

    size_t result = fread(&memory[START_ADDRESS], sizeof(uint8_t), filesize, file);
    if (result != filesize) {
        fprintf(stderr, "Failed to read ROM.\n");
        fclose(file);
        exit(1);
    }

    fclose(file);
}

//get opcode and increment pc
void execute(){
    opcode=(memory[pc] << 8u) | memory[pc+1]; 
    pc+=2;
    run_operation();
}