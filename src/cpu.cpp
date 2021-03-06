#include "cpu.h"
#include "fontset.h"
#include <stdio.h>
#include <stdlib.h>
#include <random>

// Default constructor
Chip8::Chip8() {}

// Default destructor
Chip8::~Chip8() {}

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

	// Seed the RNG
	srand (time(NULL));
}

// Load the ROM
void Chip8::load(const char* rom) {
	FILE *program_file;
	size_t read_size;

	// Open ROM into file
	program_file = fopen(rom, "rb");
	if (program_file == NULL)
	{
		printf("ERROR: Cannot open ROM");
		exit (EXIT_FAILURE);
	}

	// Get ROM size
	fseek(program_file, 0, SEEK_END);
	file_size = ftell(program_file);

	// Rewind to ROM file start
	rewind(program_file);

	// Copy ROM to memory
	read_size = fread(&memory[I+0x200], sizeof(char), file_size, program_file);
	if ((read_size != file_size) || (file_size > (0x1000 - 0x200)))
	{
		printf("ERROR: Cannot load ROM to memory");
		exit (EXIT_FAILURE);
	}

	fclose(program_file);
}

// Fetch, decode, execute opcode
void Chip8::cycle() {

	// Flag for key press
	bool key_pressed = false;

	// Fetch two bytes and merge them to get opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	// 4-bit Register identifiers
	unsigned short X = (opcode&0x0F00) >> 8;
	unsigned short Y = (opcode&0x00F0) >> 4;

	pc += 2;

	// Decode opcode
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clear screen (CLS)
					for (int i = 0; i < GFX_SIZE; i++) {
						gfx[i] = 0;
					}
					drawFlag = true;
					break;

				case 0x000E: // 0x00EE: Return from subroutine (RTS)
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

		//Register operations
		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0: // 0x8XY0: Sets VX to value of VY (MOV)
					V[X] = V[Y];
					break;

				case 0x1: // 0x8XY2: Sets VX to VX or VY (OR)
					V[X] = (V[X] | V[Y]);
					break;

				case 0x2: // 0x8XY2: Sets VX to VX and VY (AND)
					V[X] = (V[X] & V[Y]);
					break;

				case 0x3: // 0x8XY3: Sets VX to VX xor VY (XOR)
					V[X] = (V[X] ^ V[Y]);
					break;

				case 0x4: // 0x8XY4: Adds VY to VX; VF is 1 when
						  // there's a carry and 0 if not (ADD)
					if ((V[Y] + V[X]) > 0xFF)
						V[0xF] = 1; // carry
					else
						V[0xF] = 0;
					V[X] += V[Y];
					break;

				case 0x5: // 0x8XY5: VY subtracted from VX. VF is 
						  // 0 if there's a borrow and 1 if not (SUB)
					if (V[X] > V[Y])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[X] -= V[Y];
					break;

				case 0x6: // 0x8XY6: Shifts VX right by 1. VF is
						  // least significant bit of VX before shift (SHR)
					V[0xF] = V[X] & 0x1;
					V[X] = V[X] >> 1;
					break;

				case 0x7: // 0x8XY7: Sets VX to VY minus VX. VF is 0
						  // if there's a borrow and 1 if not (SUBB)
					if (V[Y] > V[X])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[X] = V[Y] - V[X];
					break;

				case 0xE: // 0x8XY8: Shifts VX left by 1. VF is set to
						  // most significant bit of VX before shift (SHL)
					V[0xF] = V[X] >> 7;
					V[X] = V[X] << 1;
					break;

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
			V[X] = (rand() % 0xFF) & (opcode & 0xFF);
			break;

		case 0xD000: // 0xDXYN: Graphics sprite
			draw_sprite(X, Y, opcode & 0x000F);
			drawFlag = true;
			break;

		case 0xE000:
			switch(opcode & 0x00FF)
			{
				case 0x9E: // 0xEX9E: Skips next instruction if key
						   // stored in VX is pressed.
					if(key[V[X]] != 0)
						pc += 2;
					break;

				case 0xA1: // 0xEXA1: Skips next instruction if key
						   // stored in VX isn't pressed.
					if(key[V[X]] == 0)
						pc += 2;
					break;

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
					for (int i = 0; i < KEY_SIZE; i++)
					{
						if (key[i] != 0)
						{
							V[X] = i;
							key_pressed = true;
						}
					}

					// No key pressed
					if (key_pressed == 0)
					{
						pc -= 2;
						return;
					}
					break;

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
					I = V[X] * 5; // Font is 4x5 bits
					break;

				case 0x33: // 0xFX33: Stores binary-coded decimal of VX
					memory[I] = V[X] / 100;
					memory[I + 1] = (V[X] / 10) % 10;
					memory[I + 2] = (V[X] % 100) % 10;
					break;

				case 0x55: // 0xFX55: Stores V0 to VX in memory address
						   // starting at address I
					for (int i = 0; i < X; i++)
						memory[I + i] = V[i];
					I = I + X + 0x1;
					break;

				case 0x65: // 0xFX65: Fills V0 to VX with values from
						   // memory starting at address I
					for (int i = 0; i < X; i++)
						V[i] = memory[I + i];
					I = I + X + 0x1;
					break;

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
			//printf("BEEP!\n");
			// Implement sound here
		}
		--sound_timer;
	}
}

// Draws sprite at (V[X], V[Y]) of specified height
void Chip8::draw_sprite(unsigned short X, unsigned short Y, 
						unsigned short height) {
	unsigned short x = V[X]; // x-position of sprite
	unsigned short y = V[Y]; // y-position of sprite
	unsigned short pixel;

	V[0xF] = 0; // Reset V[F] register

	for (int y_line = 0; y_line < height; y_line++)
	{
		pixel = memory[I + y_line]; // Get pixel to draw

		// Each pixel is 8 bits long, so loop through
		for(int x_col = 0; x_col < 0x08; x_col++)
		{
			if((pixel & (0x80 >> x_col)) != 0)
			{
				if(gfx[x + x_col + ((y + y_line) * 64)] == 1)
					V[0xF] = 1; // Collision
				gfx[x + x_col + ((y + y_line) * 64)] ^= 1;
			}
		}
	}
}

// Dumps contents of memory
void Chip8::ram_dump() {
	printf("********************************\n");
	printf("            RAM DUMP\n\n");

	if (file_size == 0)
		printf("ROM not loaded...\n");
	else
	{
		for (int i = 0; i <= 0x200 + file_size; i++)
		{
			printf("%02X ", memory[i]);
			if ((i + 1) % 11 == 0)
				printf("\n");
		}
	}
	printf("\n\n********************************\n");
}

// Dumps contents of CPU register and stack
void Chip8::cpu_dump() {
	printf("********************************\n");
	printf("            CPU DUMP\n\n");

	if (file_size == 0)
		printf("ROM not loaded...\n");
	else
	{
		printf("Program Counter: 0x%hx\n", pc);
		printf("Index Register:  0x%hx\n", I);
		printf("Stack Pointer:   0x%hx\n", sp);
		printf("\nRegister Dump:\n");
		for (int i = 0; i < REGISTER_SIZE; i++)
		{
			printf("%02X ", V[i]);
			if ((i + 1) % 11 == 0)
				printf("\n");
		}
		printf("\nStack Dump:\n");
		for (int i = 0; i < sp; i++)
		{
			printf("%hx ", stack[i]);
			if ((i + 1) % 11 == 0)
				printf("\n");
		}
	}
	printf("\n\n********************************\n");
}