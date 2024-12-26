#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define START_ADDRESS 0x200
#define FONTSET_START_ADDRESS 0x50
#define FONTSET_SIZE 80

extern uint8_t memory[4096];
extern uint16_t pc;
extern uint16_t I;
extern uint8_t registers[16];
extern uint16_t stack[16];
extern uint8_t sp;
extern uint8_t fontset[FONTSET_SIZE];

extern uint32_t screen[64 * 32];
extern uint8_t delayTimer;
extern uint8_t soundTimer;  
extern uint16_t opcode;
extern SDL_Scancode keymappings[16];
extern uint8_t curr_key_state[16];
extern uint8_t quit_flag;

//SDL functions
void initializeScreen();
void drawScreen();
void endScreen();
void handle_keypress();
//emulator functions
void LoadRom(char filename[]);
void execute();
void run_operation();
#endif 