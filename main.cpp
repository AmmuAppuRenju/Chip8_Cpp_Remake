#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include "cpu.hpp" 

void cleanup(SDL_Window* win);

int main(int argc, char *argv[]) {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Chip 8", 1280, 720, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	if (!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing window", nullptr);
		cleanup(window);
		return 1;
	}
	//Chip 8 logic
	Components components(renderer, texture);

	components.getrom("IBM Logo.ch8");
	bool running = true;
	while (running) {

		components.gameLoop(60);

		//quit logic
		SDL_Event event{};
		while (SDL_PollEvent(&event)) {
			switch (event.type)
				case SDL_EVENT_QUIT:
					running = false;
					break;	
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	cleanup(window);
	return 0;
};

void cleanup(SDL_Window* win)
{
	SDL_DestroyWindow(win);
	SDL_Quit();
}