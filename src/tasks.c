//For the UI
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION   
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "../include/nuklear.h"
#include "../include/nuklear_sdl_renderer.h"   
#include"../include/Chip8.h"
#define TINYFD_NOLIB
#include "../include/tinyfiledialogs.c"

struct nk_context *ctx;

uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
unsigned char beepBuffer[735];


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* emu_texture = NULL;

 SDL_Scancode keymappings[16] = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

SDL_AudioDeviceID audioDevice;
SDL_AudioSpec want, have;


//Handles keypresses
void handle_keypress() {
    SDL_Event event;
    nk_input_begin(ctx);
    
    // Separate input handling for the UI
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit_flag = 1;
        }
        nk_sdl_handle_event(&event);
    }
    
    // Separated keyboard handling for the emulator 
    if (!pause_flag) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        for (int i = 0; i < 16; i++) {
            curr_key_state[i] = state[keymappings[i]];
        }
    }
    
    nk_input_end(ctx);
}

//initializes the screen
void initializeScreen() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("flake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    emu_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        SCREEN_WIDTH, SCREEN_HEIGHT);

    ctx = nk_sdl_init(window, renderer);
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();

    SDL_SetTextureBlendMode(emu_texture, SDL_BLENDMODE_NONE);
    memset(screen, 0, sizeof(screen));
}

//updates the emulator output as a texture
void update_chip8_texture() {
    SDL_SetRenderTarget(renderer, emu_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (screen[y * SCREEN_WIDTH + x]) {
                SDL_Rect pixel = {x, y, 1, 1};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
}

//draws the UI  <--- This is the main function for the UI
void drawScreen() {
    //updates the SDL2 texture whenever called 
    update_chip8_texture();
    
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); //I want this to be changeable later
    SDL_RenderClear(renderer);
    
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    
    int emu_texture_width = 64 * 8;  // 64 pixels * 8 scale
    int emu_texture_height = 32 * 8; // 32 pixels * 8 scale
    int chip8X = (windowWidth - emu_texture_width) / 2;
    int chip8Y = (windowHeight - emu_texture_height) / 2;

    if (nk_begin(ctx, "CHIP-8", nk_rect(chip8X, chip8Y, emu_texture_width + 20, emu_texture_height + 40),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(ctx, emu_texture_height, 1);
        nk_image(ctx, nk_image_ptr(emu_texture));
    }

    nk_end(ctx);
    
    if (nk_begin(ctx, "Controls", nk_rect(20, 20, windowWidth - 40, 60),
    NK_WINDOW_BORDER|NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR)) {
        
        // Horizontal layout with 3 equal buttons
        nk_layout_row_static(ctx, 30, (windowWidth - 80) / 3, 3);
        
        // Pause button
        if (nk_button_label(ctx, pause_flag ? "Resume" : "Pause")) {
            pause_flag = !pause_flag;
        }
        
        // Load button
        if (nk_button_label(ctx, "Load ROM")) {
            openFilePicker();
        }
        
        // Quit button
        if (nk_button_label(ctx, "Quit")) {
            quit_flag = 1;
        }
    }
    nk_end(ctx);
    
    nk_sdl_render(NK_ANTI_ALIASING_ON);
    SDL_RenderPresent(renderer);
}

//init for the sounds
void initializeSound() {
    SDL_Init(SDL_INIT_AUDIO);
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 44100;
    want.format = AUDIO_U8;
    want.channels = 1;
    want.samples = 4096;
    want.callback = NULL;

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (audioDevice == 0) {
        fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
        return;
    }

    // square wave - goes beep
    for (int i = 0; i < 735; ++i) {
        beepBuffer[i] = (i / 100) % 2 ? 255 : 0;
    }
}

// starts the beep wave
void beep() {
    if (soundTimer > 0) {
        SDL_QueueAudio(audioDevice, beepBuffer, sizeof(beepBuffer));
        SDL_PauseAudioDevice(audioDevice, 0);
    }
}
//stops the beep wave
void stopBeep() {
    SDL_ClearQueuedAudio(audioDevice);
    SDL_PauseAudioDevice(audioDevice, 1);
}


//stop the emu
void endScreen(){
    if (emu_texture) SDL_DestroyTexture(emu_texture);
    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_CloseAudioDevice(audioDevice);
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

//resets emulator - for loading a new rom
void resetEmulator() {
    pc = START_ADDRESS;
    I = 0;
    memset(registers, 0, sizeof(registers));
    memset(stack, 0, sizeof(stack));
    sp = 0;
    memset(memory + START_ADDRESS, 0, sizeof(memory) - START_ADDRESS);
    memset(screen, 0, sizeof(screen));
    delayTimer = 0;
    soundTimer = 0;
    cpuAccumulated = 0.0f;
    timerAccumulated = 0.0f;
    stopBeep();
}

// Opens the file picker dialog
void openFilePicker() {
    pause_flag=1;
    const char *filterPatterns[] = {"*.ch8", "*.c8"};
    const char *selectedFile = tinyfd_openFileDialog(
        "Open a ROM",    
        "",                   
        2,                    
        filterPatterns,       
        "CHIP-8 ROMs",        
        0                    
    );
    //reset the emu
    if (selectedFile){
        resetEmulator();
        LoadRom(selectedFile);
        pause_flag=0;
    }
}

//get opcode and increment pc
void execute(){
    opcode=(memory[pc] << 8u) | memory[pc+1]; 
    pc+=2;
    run_operation();
}