#include <iostream>
#include <fstream>
#include <stack>
#include <cstdlib>
#include <SDL3/SDL.h>
#include "cpu.hpp"

void Components::getrom(const char* rom_path) {

	std::ifstream Rom(rom_path, std::ios::binary | std::ios::ate); // std::ios::binary reads the binary and std::ios::ate move ths file reading cursor to the end

	if (Rom.is_open()) {
		std::streampos size = Rom.tellg();
		char *contents = new char[size];

		Rom.seekg(0, std::ios::beg); // std::ios::be starts the read at the beginning
		Rom.read(contents, size);
		Rom.close();

		for (auto i = 0; i < 80; ++i)
			memory[i] = characters_fonts[i];

		for (auto i = 0; i < size; ++i)
			memory[0x200 + i] = contents[i];

		//init_stack
		for (int i = 0; i < 16; ++i) {
			stack->push(0);
		}
		

		delete[] contents;
	}
	else
		std::cout << "No file found";
}

void Components::gameLoop(int fps){
	uint8_t* byte1 = &memory[PC];
	uint8_t* byte2 = &memory[PC + 1];

	uint16_t opcode = (*byte1 << 8) | *byte2;

	opchecker(&opcode);

	printf("\\x%.04x\n", opcode);

	SDL_Delay(1000/fps);
}

void Components::opchecker(uint16_t* opcode) {

	uint8_t first_value = (*opcode & 0xf000) >> 12;

	uint8_t X = (*opcode & 0x0f00) >> 8;

	uint8_t Y = (*opcode & 0x00f0) >> 4;

	uint8_t N = *opcode & 0x000f;

	//these are larger than one byte so the need int16_t
	uint16_t NN = *opcode & 0x00ff;

	uint16_t NNN = *opcode & 0x0fff;


	if (*opcode == 0x00E0) {
		/*std::cout << "Clear Screen\n";*/
		for (auto i : pixels) i = 0;
	}
	if (*opcode == 0x00EE) {
		/*std::cout << "Subroutine\n";*/
		PC = stack->top();
		stack->pop();
	}

	switch (first_value) {
		case 0x1: {
			/*std::cout << "Jump\n";*/
			PC = NNN - 2;
			break;
		}

		case 0x2: {
			stack->push(PC);
			PC = NNN - 2;
			break;
		}

		case 0x3: {
			if (V[X] == NN)
				PC += 2;
			break;
		}

		case 0x4: {
			if (V[X] != NN)
				PC += 2;
			break;
		}

		case 0x5: {
			if (V[X] == V[Y])
				PC += 2;
			break;
		}

		case 0x6: {
			/*std::cout << "0x6: set register VX\n";*/
			V[X] = NN;
			break;
		}

		case 0x7: {
			/*std::cout << "0x7: added value to register VX\n";*/
			V[X] = (V[X] + NN) & 0xFF;
			break;
		}

		case 0x8: {
			switch (N) {
				case 0: {
					V[X] = V[Y];
					break;
				}
				case 1: {
					V[X] = V[X] | V[Y];
					break;
				}
				case 2: {
					V[X] = V[X] & V[Y];
					break;
				}
				case 3: {
					V[X] = V[X] ^ V[Y];
					break;
				}
				case 4: {
					uint16_t sum = V[X] + V[Y];
					V[X] = sum & 0xFF;
					if (sum > 255)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					break;
				}
				case 5: {
					if (V[X] > V[Y])
						V[0xF] = 1;
					else {
						V[0xF] = 0;
						V[X] = (V[X] - V[Y]) & 0xFF;
					}
					break;
				}
				case 6: {
					V[0xF] = V[X] & 0x1;
					V[X] = V[X] >> 1;
					break;
				}
				case 7: {
					if (V[Y] > V[X])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[X] = (V[Y] - V[X]) & 0xFF;
					break;
				}
				case 0xE: {
					V[0xF] = (V[X] & 0x80) >> 7;
					V[X] = (V[X] << 1) & 0xFF;
					break;
				}
			}
			break;
		}

		case 0x9: {
			if (V[X] != V[Y])
				PC += 2;
			break;
		}

		case 0xA: {
			/*std::cout << "0xA: added value to register VX\n";*/
			I = NNN;
			break;
		}

		case 0xB: {
			PC = NNN + V[0];
			break;
		}

		case 0xC: {
			V[X] = rand() % 256 & NN;
			break;
		}

		case 0xD: {
			uint16_t x = V[X] % display[0];
			uint16_t y = V[Y] % display[1];

			V[0xF] = 0;

			for (uint8_t row = 0; row < N; ++row) {
				uint8_t spriteByte = memory[I + row];
				uint16_t currenty = (y + row);

				if (currenty >= display[1])
					continue;

				for (uint8_t col = 0; col < 8; ++col) {
					uint16_t currentx = (x + col);

					if (currentx >= display[0])
						continue;

					uint8_t spriteBit = (spriteByte & (0x80 >> col));
					uint32_t* pixel = &pixels[currenty * display[0] + currentx];

					if (spriteBit) {
						if (*pixel == 0xFFFFFFFF) {
							V[0xF] = 1;
						}
						*pixel = 0xFFFFFFFF;
						/*std::cout << "Drew pixel at (" << currentx << ", " << currenty << "). \n";*/
					}
				}
			}

			SDL_UpdateTexture(texture, nullptr, pixels, display[0] * sizeof(pixels[0]));
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, texture, nullptr, nullptr);
			SDL_RenderPresent(renderer);

			break;
		}

		case 0xE: {
			switch (NN) {
				case 0x9E:
					if (keys[V[X]])
						PC += 2;
					break;
				case 0xA1:
					if (!keys[V[X]])
						PC += 2;
					break;
				}
			break;
		}

		case 0xF: {
			switch (NN){
				case 0x07: {
					V[X] = delaytimer;
					break;
				}

				case 0x15: {
					delaytimer = V[X];
					break;
				}

				case 0x18: {
					soundtimer = V[X];
					break;
				}

				case 0x1E: {
					I += V[X];
					break;
				}

				case 0x29: {
					//characters_fonts starts at 0 in memory
					I = V[X] * 5;
					break;
				}

				case 0x33: {
					uint8_t val = V[X];
					memory[I + 2] = val % 10;
					val /= 10;
					memory[I + 1] = val % 10;
					val /= 10;
					memory[I] = val % 10;

					break;
				}

				case 0x55: {
					for (auto i = 0; i < X + 1; i++)
						memory[I + i] = V[i];
					break;
				}

				case 0x65:{
					for (auto i = 0; i < X + 1; i++)
						V[i] = memory[I + i];
					break;
				}

			}
			break;
		}
	}

	PC += 2;

}