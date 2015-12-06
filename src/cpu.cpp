#include "cpu.h"
#include <stdio.h>

// Font Set
unsigned char fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

// Init registers and memory
void Chip8::initialize() {
	// Reset registers
	opcode = 0;
	I = 0;
	sp = 0;

	// Program counter reset
	pc = 0x200;

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	// Clear display
	for (int i = 0; i < GFX_SIZE; i++) {
		gfx[i] = 0;
	}

	// Clear stack
	for (int i = 0; i < STACK_SIZE; i++) {
		stack[i] = 0;
	}

	// Clear registers
	for (int i = 0; i < REGISTER_SIZE; i++) {
		V[i] = 0;
	}

	// Clear keys
	for (int i = 0; i < KEY_SIZE; i++) {
		key[i] = 0;
	}	

	// Clear memory
	for (int i = 0; i < MEMORY_SIZE; i++) {
		memory[i] = 0;
	}

	// Load fontset into memory
	for (int i = 0; i < 80; i++) {
		memory[i] = fontset[i];
	}
}

// Load the ROM
void Chip8::load(char* rom) {

}

// Fetch, decode, execute opcode
void Chip8::cycle() {
	// Fetch two bytes and merge them to get opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
	// Move program counter two bytes
	pc += 2;

	// Decode opcode
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					for (int i = 0; i < GFX_SIZE; i++) {
						gfx[i] = 0;
					}
					break;

				default:
					printf("Opcode unknown: 0x%X\n", opcode);
					break;
			}
			break;

		case 0xA000: //
			I = opcode & 0x0FFF;
			pc += 2;
			break;

		default:
			printf("Opcode unknown: 0x%X\n", opcode);
			break;
	}

	// Update delay timer
	if (delay_timer > 0)
		--delay_timer;

	// Update sound timer
	if (sound_timer > 0)
	{
		if(sound_timer == 1)
		{
			printf("BEEP!\n");
			// Implement sound here
		}
		--sound_timer;
	}
}

// Default constructor
Chip8::Chip8() {}

// Default destructor
Chip8::~Chip8() {}