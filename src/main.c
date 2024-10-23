#include"Chip8.h"
#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>
#include<time.h>
#include"operations.c"
#include"screen.c"

//seeds the random byte generator
void initRand() {
    srand(time(NULL));
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Please pass the path to the rom as an argument.\n");
        return 1;
    }
    //initializing everything
    char *rom=argv[1];
    pc = START_ADDRESS;
    opcode = 0;
    I = 0;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;
    memset(memory, 0, sizeof(memory));           
    memset(registers, 0, sizeof(registers));     
    memset(stack, 0, sizeof(stack));               
    memset(keypad, 0, sizeof(keypad));             
    memset(screen, 0, sizeof(screen));             

    // Optional: Load fontset into memory if you have one
    // memcpy(&memory[FONTSET_START_ADDRESS], chip8_fontset, FONTSET_SIZE);

    LoadRom(rom);
    initializeSDL();
    bool run= true;
    // Main emulation loop
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {  //executes events in the in order .
            if (event.type == SDL_QUIT) {
                run = false;
            }
        }
        CPUCycle();
        drawScreen();
        SDL_Delay(16);  // almost 60fps
    }
    //shuts everything down once loop is done
    endSDL();
    return 0;
}


//These are declared in the header


void fetch(){
    opcode=(memory[pc] << 8u) | memory[pc+1]; //Combining next 2 bytes to build the instruction
    pc+=2;
}

void CPUCycle(){
    fetch();
    operate();
}

void LoadRom(char filename[]){
    FILE* file = fopen(filename, "rb");  

    if (file == NULL) {
        fprintf(stderr, "Failed to open ROM: %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    // Check if the ROM fits into memory which has 4096 indexes
    if ((filesize + START_ADDRESS) > sizeof(memory)) {
        fprintf(stderr, "ROM is too large to fit in memory. Check if you chose the correct file.\n");
        fclose(file);
        exit(1);
    }

    // Read the file into memory starting at 0x200
    size_t result = fread(&memory[START_ADDRESS], sizeof(uint8_t), filesize, file);
    if (result != filesize) {
        fprintf(stderr, "Failed to read ROM file.\n");
        fclose(file);
        exit(1);
    }

    fclose(file);
}
