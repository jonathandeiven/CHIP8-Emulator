#include <iostream>
#include <thread>
#include <SDL.h>
#include "cpu.h"
#include "keyboard.h"
#include "window.h"

//Screen dimensions
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 480;

//Classes
Chip8 chip8;
SDLWindow sdl;

//Forward declarations
void draw_pixels();

int main(int argc, char **argv) {

	if (argc < 2)
	{
		std::cout << "Missing ROM file. Usage: chip8-emulator <ROM>" 
			<< std::endl;
		exit (EXIT_FAILURE);
	}

	bool running = true;
	SDL_Event e;

	sdl.create_window("CHIP-8 Emulator", SCREEN_WIDTH, SCREEN_HEIGHT);

	chip8.initialize();
	chip8.load(argv[1]);

	//Emulation loop
	while(running)
	{
		// Emulate one cycle
		chip8.cycle();
		//std::this_thread::sleep_for(std::chrono::milliseconds(17)); // 60Hz CPU

		// Update screen
		if (chip8.drawFlag){
			draw_pixels();
			SDL_UpdateWindowSurface(sdl.window);
		}

		//Handle events on queue
		while(SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
				running = false;
			//Key down
			else if (e.type == SDL_KEYDOWN)
				chip8.key[key_map(e.key.keysym.sym)] = 1;
			//Key up
			else if (e.type == SDL_KEYUP)
				chip8.key[key_map(e.key.keysym.sym)] = 0;
		}
	}
}

//Draw the pixels based on data in CPU's GFX stack
void draw_pixels() {
	SDL_Rect rect;
	int scale = SCREEN_WIDTH / GFX_W;

	//Loop through and draw each pixel
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			rect.x = x * scale;
			rect.y = y * scale;
			rect.w = scale;
			rect.h = scale;

			if (chip8.gfx[x + (y * 64)] == 0) {
				//Draw black pixel
				SDL_FillRect(sdl.screenSurface, &rect, 
					SDL_MapRGB(sdl.screenSurface->format, 0, 0, 0));
			} else {
				//Draw white pixel
				SDL_FillRect(sdl.screenSurface, &rect, 
					SDL_MapRGB(sdl.screenSurface->format, 255, 255, 255));
			}
		}
	}
}

