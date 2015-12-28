#include <iostream>
#include <thread>
#include "cpu.h"

Chip8 chip8;

int main(int argc, char **argv) {
	
	if (argc < 2)
	{
		std::cout << "Missing ROM file. Usage: chip8-emulator <ROM>" << std::endl;
		exit (EXIT_FAILURE);
	}

	chip8.initialize();
	chip8.load(argv[1]);

	while(true)
	{
		// Emulate one cycle
		chip8.cycle();
		std::this_thread::sleep_for(std::chrono::milliseconds(17)); // 60Hz CPU

		// Update screen
		//if (chip8.drawFlag)
			//drawGraphics();

		// Store key presses
		// setKeys();
	}

	return 0;
}