#include"Chip8.h"
#include<time.h>

//helper function to provide a random byte, specifically for the 0xCXNN instruction
void initRand() {
    srand(time(NULL));
}
uint8_t randByte() {
    return rand() % 256;
}

uint8_t carry;

// All codes that start with 0, 8, E, or F require nested statements.
void run_operation()
{
    uint8_t secondNibble = (opcode & 0x0F00u) >> 8u;
    uint8_t thirdNibble = (opcode & 0x00F0u) >> 4u;
    uint8_t fourthNibble = opcode & 0x000Fu;

    switch (opcode & 0xF000u)
    {
        case 0x0000:
            switch (opcode & 0x00FFu)
            {
                // 0x00E0: CLS
                case 0x00E0:
                    memset(screen, 0, sizeof(screen));
                    break;
                // 0x00EE: RET
                case 0x00EE:
                    pc = stack[--sp];
                    break;
            }   
            break;
        
        // 0x1nnn: Jump to address NNN
        case 0x1000:
            pc = opcode & 0x0FFFu;
            break;
        
        // 0x2nnn: Call subroutine at NNN
        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFFu;
            break;
        
        // 0x3xkk: Skip next instruction if Vx == kk
        case 0x3000:
            if (registers[secondNibble] == (opcode & 0x00FFu))
            {
                pc += 2;
            }
            break;
        
        // 0x4xkk: Skip next instruction if Vx != kk
        case 0x4000:
            if (registers[secondNibble] != (opcode & 0x00FFu))
            {
                pc += 2;
            }
            break;
        
        // 0x5xy0: Skip next instruction if Vx == Vy
        case 0x5000:
            if (registers[secondNibble] == registers[thirdNibble])
            {
                pc += 2;
            }
            break;
        
        // 0x6xkk: Set Vx = kk
        case 0x6000:
            registers[secondNibble] = opcode & 0x00FFu;
            break;
        
        // 0x7xkk: Set Vx += kk
        case 0x7000:
            registers[secondNibble] += (opcode & 0x00FFu);
            break;
        
        // 0x8xy_
        case 0x8000:
            switch (opcode & 0x000Fu)
            {
                // 0x8xy0: Set Vx = Vy
                case 0x0000:
                    registers[secondNibble] = registers[thirdNibble];
                    break;
                // 0x8xy1: Set Vx = Vx OR Vy
                case 0x0001:
                    registers[secondNibble] |= registers[thirdNibble];
                    break;
                // 0x8xy2: Set Vx = Vx AND Vy
                case 0x0002:
                    registers[secondNibble] &= registers[thirdNibble];
                    break;
                // 0x8xy3: Set Vx = Vx XOR Vy
                case 0x0003:
                    registers[secondNibble] ^= registers[thirdNibble];
                    break;
                // 0x8xy4: Set Vx += Vy, set VF = carry
                case 0x0004:
                {
                    uint16_t sum = registers[secondNibble] + registers[thirdNibble];
                    carry = (sum > 255U) ? 1 : 0;
                    registers[secondNibble] = sum & 0xFFu;
                    registers[0xF] =carry;  
                    break;
                }
                // 0x8xy5: Set Vx -= Vy, set VF = NOT borrow
                case 0x0005:
                    carry = (registers[secondNibble] >= registers[thirdNibble]) ? 1 : 0;
                    registers[secondNibble] -= registers[thirdNibble];
                    registers[0xF] = carry;
                    break;
                // 0x8xy6: Shift Vx right by 1
                case 0x0006:
                    carry = registers[secondNibble] & 0x1u;
                    registers[secondNibble] >>= 1;
                    registers[0xF] = carry;
                    break;
                // 0x8xy7: Set Vx = Vy - Vx, set VF = NOT borrow
                case 0x0007:
                    carry = (registers[thirdNibble] >= registers[secondNibble]) ? 1 : 0;
                    registers[secondNibble] = registers[thirdNibble] - registers[secondNibble];
                    registers[0xF] = carry;
                    break;
                // 0x8xyE: Shift Vx left by 1
                case 0x000E:
                    carry = (registers[secondNibble] & 0x80u) >> 7u;
                    registers[secondNibble] <<= 1;
                    registers[0xF] = carry;
                    break;
            }
            break;
        
        // 0x9xy0: Skip next instruction if Vx != Vy
        case 0x9000:
            if (registers[secondNibble] != registers[thirdNibble])
            {
                pc += 2;
            }
            break;
        
        // 0xANNN: Set I = NNN
        case 0xA000:
            I = opcode & 0x0FFFu;
            break;
        
        // 0xBNNN: Jump to V0 + NNN
        case 0xB000:
            pc = registers[0] + (opcode & 0x0FFFu);
            break;
        
        // 0xCXNN: Set Vx = random byte AND NN
        case 0xC000:
            registers[secondNibble] = randByte() & (opcode & 0x00FFu);
            break;
        
        // 0xDXYN: Draw sprite at (Vx, Vy), set VF = collision
        case 0xD000:
        {
            uint8_t Vx = registers[secondNibble];
            uint8_t Vy = registers[thirdNibble];
            uint8_t height = opcode & 0x000Fu;
            registers[0xF] = 0;

            for (uint8_t row = 0; row < height; ++row)
            {
                uint8_t spriteByte = memory[I + row];
                for (uint8_t col = 0; col < 8; ++col)
                {
                    uint8_t spritePixel = spriteByte & (0x80u >> col);
                    uint32_t *screenPixel = &screen[(Vy + row) * SCREEN_WIDTH + (Vx + col)];

                    if (spritePixel)
                    {
                        if (*screenPixel == 0xFFFFFFFF)
                        {
                            registers[0xF] = 1;
                        }
                        *screenPixel ^= 0xFFFFFFFF;
                    }
                }
            }
            break;
        }
        
        case 0xE000:
            switch (opcode & 0x00FFu)
            {
                // 0xEX9E: Skip next instruction if key with the value of Vx is pressed
                case 0x009E:
                    if (curr_key_state[registers[secondNibble]])
                    {
                        pc += 2;
                    }
                    break;
                // 0xEXA1: Skip next instruction if key with the value of Vx is not pressed
                case 0x00A1:
                    if (!curr_key_state[registers[secondNibble]])
                    {
                        pc += 2;
                    }
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FFu)
            {
                // 0xFX07: Set Vx = delay timer value
                case 0x0007:
                    registers[secondNibble] = delayTimer;
                    break;
                // 0xFX0A: Wait for a key press, store the value of the key in Vx
                case 0x000A:
                {
                    bool keyPressed = false;
                    for (uint8_t i = 0; i < 16; ++i)
                    {
                        if (curr_key_state[i])
                        {
                            registers[secondNibble] = i;
                            keyPressed = true;
                            break;
                        }
                    }
                    if (!keyPressed)
                    {
                        pc -= 2;
                    }
                    break;
                }
                // 0xFX15: Set delay timer = Vx
                case 0x0015:
                    delayTimer = registers[secondNibble];
                    break;
                // 0xFX18: Set sound timer = Vx
                case 0x0018:
                    soundTimer = registers[secondNibble];
                    break;
                // 0xFX1E: Set I += Vx
                case 0x001E:
                    I += registers[secondNibble];
                    break;
                // 0xFX29: Set I = location of sprite for digit Vx
                case 0x0029:
                    I = registers[secondNibble] * 5;
                    break;
                // 0xFX33: Store BCD representation of Vx in memory locations I, I+1, I+2
                case 0x0033:
                    memory[I] = registers[secondNibble] / 100;
                    memory[I + 1] = (registers[secondNibble] / 10) % 10;
                    memory[I + 2] = registers[secondNibble] % 10;
                    break;
                // 0xFX55: Store registers V0 to Vx in memory starting at address I
                case 0x0055:
                    for (uint8_t i = 0; i <= secondNibble; ++i)
                    {
                        memory[I + i] = registers[i];
                    }
                    break;
                // 0xFX65: Read registers V0 to Vx from memory starting at address I
                case 0x0065:
                    for (uint8_t i = 0; i <= secondNibble; ++i)
                    {
                        registers[i] = memory[I + i];
                    }
                    break;
            }
            break;
        default:
            printf("Error: Unknown opcode 0x%04X \n", opcode);
            exit(1); 
    }
}
