# flake

Flake is a lightweight Chip 8 interpreter/emulator written in C. It supports all basic Chip-8 operations and uses SDL2 to draw the graphics.

I've worked on this purely to understand architecture, and while I try my hardest, it might not be the most readable code. This is the first C project and there might be a few deviations from standard practices, so I apologize for any hard-to-read code. Of course, feedback is welcome!

## How to get it to work

You don't need to compile it on Debian-based linux distros anymore! Check the releases tab for the .deb file. Just download and install!

For manual compilation, read on.

## Compiling:

### Prerequisites:
1. GCC
2. SDL2

(This was done in a linux environment originally, but these instructions should still work for windows as long as you have all the prerequisites.)

First, Compile the four files in the src directory. Once done, You can either call without arguments, or pass the path to the rom as an argument.

You can compile manually, or use the provided makefile.

**Note: the -lSDL2 flag needs to be present when compiling for it to work.**

Manual Compilation
```bash
  gcc -o flake src/main.c src/operations.c src/tasks.c src/view.c src/theme.c -lSDL2 -lm #To compile
```
**Alternatively, use the MakeFile provided to compile the project.**

```bash
  make # To compile
```
This will create the binary in the root directory. 
To run the application:
```bash
  ./flake  # To start without preloading a rom
  ./flake path/to/your/rom.ch8 # Alternatively, start with a rom
```

## Packaging

To automatically package, just run
```bash
  make install
```
This will build the deb file in the `package/flake-amd64` directory. You can install from there.

### For manual packaging
The packaging directory is already setup and included in the `package/flake-amd64` directory. The important files are:

```bash
  /DEBIAN/control
  /usr/bin/
  /usr/share/applications/flake.desktop
```
The control and flake.desktop files contain information required for packaging. 

**The /usr/bin directory should contain the output binary to build from.**

All the icons are in the `hicolor/SIZExSIZE` directory, make a new folder for any icons with different sizes.

Before packaging, ensure you have the compiled binary along with the files listed above. If you do not, use the instructions above to compile. Once done, run the following:

```bash
  dpkg-deb --build package/flake-amd64 # Change the path to your packaging dir
```

This will create a `.deb` file in the directory `package/flake-amd64`

## Using the Emulator

**Note: The CPU runs at 950Hz by default. Adjust this based on your game using the slider, until the game runs how you want it to.**

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

## Testing

The roms for testing specific modules were obtained from [The Chip8 Test Suite](https://github.com/Timendus/chip8-test-suite). These helped me out a lot, please check the original repository out!

The roms I used to test games were obtained from [The Chip8 Archive](https://johnearnest.github.io/chip8Archive/) and [Kripod's repository](https://github.com/kripod/chip8-roms) where there are lots more!


## Credits

This project was done mostly to understand CPU architecture and lower level programming better, and some of the resources I used were very helpful for understanding the CHIP8. 

Here are some sources I used for specific Chip8 knowledge. I recommend you check those out if you want to explore further too.

[Cowgod's Chip8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

[Tobias V Langhoff's guide to making a Chip8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

[Leonardo Folgoni's Implementation of the Emulator](https://github.com/f0lg0/CHIP-8)

Feel free to use the contents of this repository as you wish. [MIT LICENSE](https://github.com/Sunset-06/flake/blob/main/LICENSE)
