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

	if (argc > 1) 
		components.getrom(argv[1]);
	else
		components.getrom("");

	bool running = true;
	while (running) {

		components.gameLoop(300);


		//quit logic
		SDL_Event event{};
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT: {
				running = false;
				break;
			}
			case SDL_EVENT_KEY_DOWN: {
				switch (event.key.key) {
					case SDLK_X: {
						components.keys[0] = 1;
						break;
					}
					case SDLK_1: {
						components.keys[1] = 1;
						break;
					}
					case SDLK_2: {
						components.keys[2] = 1;
						break;
					}
					case SDLK_3: {
						components.keys[3] = 1;
						break;
					}
					case SDLK_Q: {
						components.keys[4] = 1;
						break;
					}
					case SDLK_W: {
						components.keys[5] = 1;
						break;
					}
					case SDLK_E: {
						components.keys[6] = 1;
						break;
					}
					case SDLK_A: {
						components.keys[7] = 1;
						break;
					}
					case SDLK_S: {
						components.keys[8] = 1;
						break;
					}
					case SDLK_D: {
						components.keys[9] = 1;
						break;
					}
					case SDLK_C: {
						components.keys[0xA] = 1;
						break;
					}
					case SDLK_Z: {
						components.keys[0xB] = 1;
						break;
					}
					case SDLK_4: {
						components.keys[0xC] = 1;
						break;
					}
					case SDLK_R: {
						components.keys[0xD] = 1;
						break;
					}
					case SDLK_F: {
						components.keys[0xE] = 1;
						break;
					}
					case SDLK_V: {
						components.keys[0xF] = 1;
						break;
					}
				}
				break;
			}
			case SDL_EVENT_KEY_UP: {
				switch (event.key.key) {
				case SDLK_X: {
					components.keys[0] = 0;
					break;
				}
				case SDLK_1: {
					components.keys[1] = 0;
					break;
				}
				case SDLK_2: {
					components.keys[2] = 0;
					break;
				}
				case SDLK_3: {
					components.keys[3] = 0;
					break;
				}
				case SDLK_Q: {
					components.keys[4] = 0;
					break;
				}
				case SDLK_W: {
					components.keys[5] = 0;
					break;
				}
				case SDLK_E: {
					components.keys[6] = 0;
					break;
				}
				case SDLK_A: {
					components.keys[7] = 0;
					break;
				}
				case SDLK_S: {
					components.keys[8] = 0;
					break;
				}
				case SDLK_D: {
					components.keys[9] = 0;
					break;
				}
				case SDLK_C: {
					components.keys[0xA] = 0;
					break;
				}
				case SDLK_Z: {
					components.keys[0xB] = 0;
					break;
				}
				case SDLK_4: {
					components.keys[0xC] = 0;
					break;
				}
				case SDLK_R: {
					components.keys[0xD] = 0;
					break;
				}
				case SDLK_F: {
					components.keys[0xE] = 0;
					break;
				}
				case SDLK_V: {
					components.keys[0xF] = 0;
					break;
				}
				}
				break;
			}
			}

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