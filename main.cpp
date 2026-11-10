#include <iostream>
#include "cpu.hpp" 
//#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>

int main() {
	Components components;

	components.getrom("IBM Logo.ch8");

	for (auto i = 0; i < std::size(components.memory); i++) {
		components.gameLoop();
	}

	return 0;
};