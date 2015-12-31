#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class SDLWindow {
public:
	//SDL window objects
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	//SDL helper method
	void create_window(const char *scr_title, int scr_height, int scr_width);

	//Class constructor, destructor, rule-of-three
	SDLWindow();
	~SDLWindow();
	SDLWindow(const SDLWindow&) = delete;
	SDLWindow& operator=(const SDLWindow&) = delete;
};

#endif