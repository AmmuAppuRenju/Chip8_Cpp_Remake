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

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	std::vector<uint32_t> pixels(64 * 32, 0x394dfe80);
	SDL_UpdateTexture(texture, nullptr, pixels.data(), 64 * sizeof(uint32_t));


	SDL_RenderTexture(renderer, texture, nullptr, nullptr);
	
	SDL_RenderPresent(renderer);

	SDL_Delay(10000);

	components.getrom("IBM Logo.ch8");

	//minus 2 because we need to iterate to the amount the PC does for now
	for (auto i = 0; i < std::size(components.memory) - 2; i++) {
		components.gameLoop();
		
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