#include <iostream>
#include <thread>
#include <SDL.h>
#include "window.h"
#include "cpu.h"

//Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Chip8 chip8;

int main(int argc, char **argv) {

	if (argc < 2)
	{
		std::cout << "Missing ROM file. Usage: chip8-emulator <ROM>" 
			<< std::endl;
		exit (EXIT_FAILURE);
	}

	SDLWindow sdl;
	bool quit = false;
	SDL_Event e;

	sdl.create_window("CHIP-8 Emulator", SCREEN_WIDTH, SCREEN_HEIGHT);

	sdl.gHelloWorld = SDL_LoadBMP( "/Users/jonathan/Documents/Projects/CHIP8-Emulator/src/hello_world.bmp" );
	if( sdl.gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
		return 1;
	}

	while (!quit)
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		//Apply the image
		SDL_BlitSurface( sdl.gHelloWorld, NULL, sdl.screenSurface, NULL );
		SDL_UpdateWindowSurface( sdl.window );
	}

	/*chip8.initialize();
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
	}*/

	return 0;
}