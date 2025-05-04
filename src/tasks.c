#include "../include/Chip8.h"
#define TINYFD_NOLIB
#include "../include/tinyfiledialogs.c"

//load the rom file
void LoadRom(const char* filename){
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
    opcode=0;
    memset(memory + START_ADDRESS, 0, sizeof(memory) - START_ADDRESS);
    //reloading fontset
    memcpy(memory, fontset, sizeof(fontset));

    memset(screen, 0, sizeof(screen));
    delayTimer = 0;
    soundTimer = 0;
    cpuAccumulated = 0.0f;
    timerAccumulated = 0.0f;
    stopBeep();
    memset(curr_key_state, 0, sizeof(curr_key_state));
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