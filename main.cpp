#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include "cpu.hpp" 


int main(int argc, char *argv[]) {
	SDL_Window *window = SDL_CreateWindow("Chip 8", 1280, 720, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	Components components(renderer, texture);

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	components.getrom("IBM Logo.ch8");

	//minus 2 because we need to iterate to the amount the PC does for now
	while (true) {
		components.gameLoop(60);
	}

    //sdl stuff
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
		return 1;
	}

	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
};