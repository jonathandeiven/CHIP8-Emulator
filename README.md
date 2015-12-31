# CHIP-8 Emulator System
This is an interpreter for the CHIP-8 system, implemented using C++ and the SDL library for graphics.
![Space Invaders Screenshot](http://jonathandeiven.com/images/scrnsht-chip8.png)

## To compile:

1. Make sure you have installed the SDL2 library on your machine. [Download link here](https://www.libsdl.org/download-2.0.php).
2. Execute the following code in the root of this project.
```
mkdir build && cd build
cmake ..
make
```
You will find the program in `bin`. Execute it as `./emulator <path_to_ROM>`


## Keyboard

Our standard keyboards don't have the same layout as the keypad from a CHIP-8 system, so the keys are mapped as:
```
| 1 | 2 | 3 | C |             | 1 | 2 | 3 | 4 |
| 4 | 5 | 6 | D |      =>     | Q | W | E | R |
| 7 | 8 | 9 | E |             | A | S | D | F |
| A | 0 | B | F |             | Z | X | C | V |
 (CHIP-8 Keypad)             (Emulator Keyboard)
```

## Games

Three well-known public domain games are included as ROM files in this repo: Space Invaders, PONG and Tetris. To control PONG, Player 1 uses `1` to move up and `Q` to move down while Player 2 uses `4` to move up and `R` to move down.

## Notes

- The timer exists for sound in the CPU cycling for future additions, but the actual sound was not programmed.