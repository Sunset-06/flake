# flake

This is a lightweight Chip 8 interpreter/emulator written in C. It supports basic Chip-8 operations and uses SDL2 to draw the graphics.

I've worked on this purely to understand architecture, and while I try my hardest, it might not be the most readable code. This is the first C project and there might be a few deviations from standard practices, so I apologize for any hard-to-read code. 

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

  ./flake  #To start without preloading a rom
  ./flake path/to/your/rom.ch8 #Alternatively, start with a rom
```

The CPU runs at 950Hz by default. Adjust this based on your game using the slider, until the game runs well.

### Controls

The Chip8 keypad is mapped to the keyboard like this:  

<table>
<tr><th>Chip8 Keypad </th><th>Keyboard Mapping</th></tr>
<tr><td>

| 1 | 2 | 3 | C |
|--|--|--|--|
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

</td><td>

| 1 | 2 | 3 | 4 |
|--|--|--|--|
| Q | W | D | R |
| A | S | D | F |
| Z | X | C | V |

</td></tr> </table>

## Credits

This project was done mostly to understand CPU architecture and lower level programming better, and some of the resources i used were very useful. 

However, The random old reddit threads that explained to me most things are too varied and fragmented to list. So here are the sources I used for specific Chip8 knowledge. I recommend you check those out if you want to explore further too.

[Cowgod's Chip8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

[Tobias V Langhoff's guide to making a Chip8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

[Leonardo Folgoni's Implementation of the Emulator](https://github.com/f0lg0/CHIP-8)

The roms I used to test games were obtained from [The Chip8 Archive](https://johnearnest.github.io/chip8Archive/) and [Kripod's repository](https://github.com/kripod/chip8-roms) where there are lots more!

The roms for testing specific modules were obtained from [chip8-test-suite](https://github.com/Timendus/chip8-test-suite). These helped me out a lot, please check the original repository out!

Feel free to use the contents of this repository as you wish. [MIT LICENSE](https://github.com/Sunset-06/flake/blob/main/LICENSE)
