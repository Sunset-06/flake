# flake

This is a lightweight Chip 8 interpreter/emulator written in C. It supports basic Chip-8 operations and uses SDL2 to draw the graphics.

I've worked on this purely to understand architecture, and while I try my hardest, it might not be the most readable code. This is the first C project and there might be a few deviations from standard practices, so I apologize f

## How to get it to work

### There are two prerequisites:
1. GCC
2. SDL2

### Compiling:
(This was done in a linux environment originally, but these instructions should still work for windows as long as you have all the prerequisites.)

First, Compile the four files in the src directory. Once done, pass the path to the rom as an argument when running the program.

**Note: the -lSDL2 flag needs to be present when compiling for it to work.**

For Example:
```bash
  gcc -g src/main.c src/operations.c src/tasks.c src/screen.c -lSDL2 -o flake #To compile

  ./flake path/to/your/rom.ch8 #To execute
```
**Alternatively, use the MakeFile provided to compile the project.**

```bash
  make #To compile

  ./flake path/to/your/rom.ch8 #To execute
```

Currently the CPU speed is fixed to 950 Hz. In case this does not behave well with the rom you want to run, You need to change this to your desired frequency and recompile.

```C
// in main.c at line 32
int CPU_HZ = 950;
```
950 works for most modern games, but older games might have trouble running this. For example, this was too fast for Pong.

## Work left

This project is fully functioning! Everything works now!
The previous commits had me adding a variable for CPU frequency and functions for sound. 

Now that the emulator is done, I want to make a UI to allow people to load roms, change colours and maybe even change what waveform plays when the sound timers are on (currently it plays a square wave).

I'm not sure when this will be done, or what I would be using to implement it, but it definitely is being worked on. Until then, you will have to use a cli to run roms.

## Credits

This project was done mostly to understand CPU architecture and lower level programming better, and some of the resources i used were very useful. 

However, The random old reddit threads that explained to me most things are too varied and fragmented to list. So here are the sources I used for specific Chip8 knowledge. I recommend you check those out if you want to explore further too.

[Cowgod's Chip8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

[Tobias V Langhoff's guide to making a Chip8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

[Leonardo Folgoni's Implementation of the Emulator](https://github.com/f0lg0/CHIP-8)

The roms I used to test games were obtained from [The Chip8 Archive](https://johnearnest.github.io/chip8Archive/) and [Kripod's repository](https://github.com/kripod/chip8-roms) where there are lots more!

The roms for testing specific modules were obtained from [chip8-test-suite](https://github.com/Timendus/chip8-test-suite). These helped me out a lot, please check the original repository out!

Feel free to use the contents of this repository as you wish. [MIT LICENSE](https://github.com/Sunset-06/flake/blob/main/LICENSE)
