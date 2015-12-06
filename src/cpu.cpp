#include "cpu.h"
#include "fontset.h"
#include <stdio.h>

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
	// 4-bit Register identifiers
	unsigned char X = (opcode&0x0F00) >> 8;
	unsigned char Y = (opcode&0x00F0) >> 4;

	// Fetch two bytes and merge them to get opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	pc += 2;

	// Decode opcode
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clear screen
					for (int i = 0; i < GFX_SIZE; i++) {
						gfx[i] = 0;
					}
					break;

				case 0x000E: // 0x00EE: Return from subroutine
					pc = stack[--sp];
					break; 

				default:
					printf("Opcode unknown: 0x%X\n", opcode);
					break;
			}
			break;

		case 0x1000: // 0x1NNN: Jump to address NNN
			pc = opcode & 0x0FFF;
			break;

		case 0x2000: // 0x2NNN: Calls subroutine at NNN
			stack[sp++] = pc;
			pc = opcode & 0x0FFF;
			break;

		case 0x3000: // 0x3XNN: Skips next instruction if VX = NN
			if (V[X] == (opcode & 0x00FF))
				pc += 2;
			break;

		case 0x4000: // 0x4XNN: Skips next instruction if VX != NN
			if (V[X] != (opcode & 0x00FF))
				pc += 2;
			break;

		case 0x5000: // 0x5XY0: Skips next instruction if VX = VY
			if (V[X] == V[Y])
				pc += 2;
			break;

		case 0x6000: // 0x6XNN: Sets VX = NN
			V[X] = (opcode & 0x00FF);
			break;

		case 0x7000: // 0x7XNN: Adds NN to VX
			V[X] += (opcode & 0x00FF);
			break;

		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0: // 0x8XY0: Sets VX to value of VY
					V[X] = V[Y];
					break;

				case 0x1: // 0x8XY2: Sets VX to VX or VY
					V[X] = (V[X] | V[Y]);
					break;

				case 0x2: // 0x8XY2: Sets VX to VX and VY
					V[X] = (V[X] & V[Y]);
					break;

				case 0x3: // 0x8XY3: Sets VX to VX xor VY
					V[X] = (V[X] ^ V[Y]);
					break;

				case 0x4: // 0x8XY4: Adds VY to VX; VF is 1 when
						  // there's a carry and 0 if not

				case 0x5: // 0x8XY5: VY subtracted from VX. VF is 
						  // 0 if there's a borrow and 1 if not

				case 0x6: // 0x8XY6: Shifts VX right by 1. VF is
						  // least significant bit of VX before shift

				case 0x7: // 0x8XY7: Sets VX to VY minus VX. VF is 0
						  // if there's a borrow and 1 if not

				case 0xE: // 0x8XY8: Shifts VX left by 1. VF is set to
						  // most significant bit of VX before shift

				default:
					printf("Opcode unknown: 0x%X\n", opcode);
					break;
			}
			break;

		case 0x9000: // 0x9XY0: Skips next instruction if VX != VY
			if (V[X] != V[Y])
				pc += 2;
			break;

		case 0xA000: // 0xANNN: Set I to address NNN
			I = opcode & 0x0FFF;
			break;

		case 0xB000: // 0xBNNN: Jumps to address of NNN + V0
			pc = (opcode & 0x0FFF) + V[0];
			break;

		case 0xC000: // 0xCXNN: Sets VX to result of bitwise operation
					 // on random number and NN

		case 0xD000: // 0xDXYN: Graphics sprite

		case 0xE000:
			switch(opcode & 0x00FF)
			{
				case 0x9E: // 0xEX9E: Skips next instruction if key
						   // stored in VX is pressed.

				case 0xA1: // 0xEXA1: Skips next instruction if key
						   // stored in VX isn't pressed.

				default:
					printf("Opcode unknown: 0x%X\n", opcode);
					break;
			}
			break;

		case 0xF000:
			switch(opcode & 0x00FF)
			{
				case 0x07: // 0xFX07: Sets VX to delay timer value
					V[X] = delay_timer;
					break;

				case 0x0A: // 0xFX0A: Key press awaited, and stored in VX

				case 0x15: // 0xFX15: Sets delay timer to VX
					delay_timer = V[X];
					break;

				case 0x18: // 0xFX18: Sets sound timer to VX
					sound_timer = V[X];
					break;

				case 0x1E: // 0xFX1E: Adds VX to I
					I += V[X];
					break;

				case 0x29: // 0xFX29: Sets I to location of sprite

				case 0x33: // 0xFX33: Stores binary-coded decimal of VX

				case 0x55: // 0xFX55: Stores V0 to VX in memory address
						   // starting at address I

				case 0x65: // 0xFX65: Fills V0 to VX with values from
						   // memory starting at address I

				default:
					printf("Opcode unknown: 0x%X\n", opcode);
					break;
			}
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