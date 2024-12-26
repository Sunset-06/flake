# flake

This is a Chip 8 interpreter/emulator written in C. It supports basic Chip-8 operations and uses SDL2 to draw the graphics.

I've worked on this purely to understand architecture, and while I try my hardest, it might not be the most readable code  as it is the first emulator I have written and also the first time I have used SDL2. The file structure might be a bit messy as of now, but all the necessary components are declared in one header file, so they should be easy to keep track of.

## How to get it to work

### There are two prerequisites:
1. GCC
2. SDL2
 
Once you have these installed in your system, you should be good to go!

### Compiling:
In its current state the interpreter is rough, and does require you to compile it (This is definitely something that will be updated later)
For now, follow the instructions below:

(This was done in a linux environment originally, but these instructions should still work for windows as long as you have all the prerequisites.)

First, Compile the four files in the src directory. Once done, pass the path to the rom as an argument when running the program. Thats it!

**Note: the -lSDL2 flag needs to be present when compiling for it to work.**

For Example:
```bash
  gcc -g src/main.c src/operations.c src/tasks.c src/screen.c -lSDL2 -o flake #To compile

  ./flake path/to/your/rom.ch8 #To execute
```

There is also a preconfigured task for VSCode  in the `.vscode/tasks.json` file to make things easier if you use VSCode!

## Work left

There are still some things left to do for this,
1. Add keypad support (You cannot input anything in its current state!)
2. Implement the instructions that require the keypad
3. Implement the timers!
4. Fix the remaining instructions!

## Credits

This project was done mostly to understand CPU architecture and lower level programming better, and some of the resources i used were very useful. 

However, The random old reddit threads that explained to me what terminals originally were, and the NAND2Tetris course that got me comfortable with circuits and bitwise operations, are too varied and fragmented to list. So here are the sources I used for specific Chip8 knowledge. I recommend you check those out if you want to explore further too.

[Cowgod's Chip8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

[Tobias V Langhoff's guide to making a Chip8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

[Leonardo Folgoni's Implementation of the Emulator](https://github.com/f0lg0/CHIP-8)

The roms provided in the directory in this repo were obtained from [Kripod's repository](https://github.com/kripod/chip8-roms) where there are lots more!

The test roms were obtained from [chip8-test-suite](https://github.com/Timendus/chip8-test-suite). These helped me out a lot, so i won't be including them in my roms folder, please check the original repository out!

Feel free to use the contents of the repository as you wish.
