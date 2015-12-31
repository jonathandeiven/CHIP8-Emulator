#include "window.h"
#include <iostream>

//SDL initialization
SDLWindow::SDLWindow() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL cannot initialize. Error: " << SDL_GetError() 
			<< std::endl;
		exit (EXIT_FAILURE);
	}
}

//SDL cleanup
SDLWindow::~SDLWindow() {
	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

//Creates a window using SDL
void SDLWindow::create_window(const char *scr_title, int scr_width, 
	int scr_height) {
	//Create window
	window = SDL_CreateWindow(scr_title, SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, scr_width, scr_height, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		std::cout << "Window cannot be created. Error: " << SDL_GetError() 
			<< std::endl;
		SDL_Quit();
		exit (EXIT_FAILURE);
	}

	//Get window surface
    screenSurface = SDL_GetWindowSurface( window );
}