#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "cpu.hpp" 


int main(int argc, char *argv[]) {
	Components components;
	SDL_Window* window = SDL_CreateWindow("Chip 8", components.display[0], components.display[0], 0);

	components.getrom("IBM Logo.ch8");

	for (auto i = 0; i < std::size(components.memory) - 2; i++) {
		components.gameLoop();
	}

    //sdl stuff
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
		return 1;
	}

	SDL_Quit();

	return 0;
};