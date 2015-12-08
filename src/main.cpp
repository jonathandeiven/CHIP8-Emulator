#include <iostream>
#include "cpu.h"

using namespace std;

Chip8 chip8;

int main(int argc, char **argv) {

	if (argc < 2)
	{
		cout << "Missing ROM file. Usage: chip8-emulator <ROM>" << endl;
		exit (EXIT_FAILURE);
	}

	chip8.initialize();
	chip8.load(argv[1]);

	while(true)
	{
		// Emulate one cycle
		chip8.cycle();

		// Update screen
		//if (chip8.drawFlag)
			//drawGraphics();

		// Store key presses
		// setKeys();
	}

	return 0;
}