#include"../include/Chip8.h"

//All globals are here
uint8_t memory[4096]={0};
uint16_t pc = START_ADDRESS;
uint16_t I = 0;
uint8_t registers[16] = {0};
uint16_t stack[16] = {0};
uint8_t sp=0;
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
int CPU_HZ = 950; // !!! Change later - need to make this variable from UI


uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
unsigned char beepBuffer[735];
uint8_t delayTimer;
uint8_t soundTimer;
float cpuAccumulated = 0.0f;
float timerAccumulated = 0.0f;
uint16_t opcode;
SDL_Scancode keymappings[16] = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};
uint8_t curr_key_state[16]= {0};  
uint8_t quit_flag=0;
uint8_t pause_flag=0;


/* ----------------> TODO: Fix the ROM Loading Timing <--------------*/
int main(int argc, char** argv) {
    /* if (argc != 2) {
        printf("Wrong command! Use it as:\n flake rom_file.ch8\n");
        return 1;
    } */
    initializeScreen();
    initializeSound();
    //char* inputRom = argv[1];
    //LoadRom(inputRom);
    // counters for cpu and timers  
    uint32_t lastCpuTicks = SDL_GetTicks();
    const float timerInterval = 1000.0f / 60.0f; // Timers locked to 60

    //main loop
    while(!quit_flag){
        // handling inputs
        handle_keypress();
        drawScreen();

        //Checks if the emu is paused every loop
        if(!pause_flag){
            // handling timers
            uint32_t currentTicks = SDL_GetTicks();
            uint32_t deltaTicks = currentTicks - lastCpuTicks;
            lastCpuTicks = currentTicks;

            cpuAccumulated += deltaTicks;
            timerAccumulated += deltaTicks;

            while(timerAccumulated >= timerInterval) { 
                if (delayTimer > 0) {
                    --delayTimer;
                }
                if (soundTimer > 0) {
                    --soundTimer;
                }

                if (soundTimer == 0) {
                    stopBeep();
                }
                else {
                    beep();
                }
        
                timerAccumulated -= timerInterval;
            }

            /*I'm keeping this inside the loop to allow 
            the player to change it dynamically whenever they want
            Maybe a slider when get around to adding a UI */
            float intervalPerCycle = 1000.0f / CPU_HZ; 
            
            //executing the instructions
            while(cpuAccumulated >= intervalPerCycle) {    
                execute();
                cpuAccumulated -= intervalPerCycle;
            }
        }
        else {
            // Pause for 16ms to prevent infinite loop 
            SDL_Delay(16);  
        }
    }

    endScreen();
    return 0;
}