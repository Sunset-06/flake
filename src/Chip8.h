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
extern uint32_t screen[64 * 32];
extern uint16_t pc;
extern uint16_t I;
extern uint16_t stack[16];
extern uint8_t delayTimer;
extern uint8_t soundTimer;
extern unsigned int registers[16];

extern uint8_t sp;
extern uint16_t opcode;
extern uint8_t keypad[16];
extern uint8_t fontset[FONTSET_SIZE];

/* 
// Function prototypes
void fetch();
void operate();
void CPUCycle();
void LoadRom(char filename[]);
void drawScreen();
void initializeSDL();
void endSDL();
 */
#endif 