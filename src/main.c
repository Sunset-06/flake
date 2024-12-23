#include"Chip8.h"

uint8_t memory[4096]={0};
uint16_t pc = START_ADDRESS;
uint16_t I = 0;
uint8_t registers[16] = {0};
uint16_t stack[16] = {0};
uint8_t sp=0    ;
unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
uint8_t delayTimer;
uint8_t soundTimer;  
uint16_t opcode;

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Wrong command! Use it as:\n flake rom_file.ch8");
        return 1;
    }
    initializeScreen();
    char* inputRom = argv[1];
    LoadRom(inputRom);
    int quit_flag=0;
    unsigned char curr_key_state[16] = {0};
    SDL_Event event;

    //main loop
    while(!quit_flag){
        //handling keypresses
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit_flag = 1; 
            
            const Uint8* state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_BACKSPACE]) {
                quit_flag = 1;
            }

            for (int keycode = 0; keycode < 16; keycode++) {
                curr_key_state[keycode] = state[keymappings[keycode]] ? 1 : 0;  // 1 if pressed, 0 if not
            }
        }

        //handling the timers
        if (delayTimer > 0) {
            --delayTimer;
        }
        if (soundTimer > 0) {
            --soundTimer;
        }
        //finally, executing the instructions
        execute();
        drawScreen();
        SDL_Delay(16); 
    }

    endScreen();
    return 0;
}