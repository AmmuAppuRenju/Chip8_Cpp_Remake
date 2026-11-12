#include <iostream>
#include <fstream>
#include <vector>
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

		for (auto i = 0; i < sizeof(characters_fonts); ++i)
			memory[i] = characters_fonts[i];

		for (auto i = 0; i < size; ++i)
			memory[0x200 + i] = contents[i];
		

		delete[] contents;
	}
	else
		std::cout << "No file found";
}

void Components::gameLoop(){
	uint8_t *byte1 = &memory[PC];
	uint8_t *byte2 = &memory[PC + 1];

	uint16_t opcode = (*byte1 << 8) | *byte2;

	opchecker(opcode);

	/*printf("\\x%.04x\n", opcode);*/
}

void Components::opchecker(int16_t opcode) {

	int8_t first_value = (opcode & 0xf000) >> 12;

	int8_t X = (opcode & 0x0f00) >> 8;

	int8_t Y = (opcode & 0x00f0) >> 4;

	int8_t N = opcode & 0x000f;

	int8_t NN = opcode & 0x00ff;

	int8_t NNN = opcode & 0x0fff;

	if (opcode == 0x00E0) {
		std::cout << "Clear Screen\n";
	}
	if (opcode == 0x00EE) {
		std::cout << "Subroutine\n";
		PC = stack->top();
		stack->pop();
	}

	switch (first_value) {
		case 0x1:
			std::cout << "Jump\n";
			PC = NNN - 2;
			break;
		case 0x6:
			std::cout << "set register VX\n";
			V[X] = NN;
			break;
		case 0x7:
			std::cout << "add value to register VX\n";
			V[X] = (V[X] + NN) & 0xFF;
			break;
		case 0xA:
			std::cout << "add value to register VX\n";
			I = NNN;
			break;
		case 0xD:
			/*int8_t x = V[X] % display[0];
			int8_t y = V[Y] % display[1];

			V[0xF] = 0;

			for (auto row = 0; row < N; row++) {
				int8_t spriteByte = memory[I + row];
				auto currenty = y + row;
				if (currenty >= display[1]) 
					continue;

				for (auto col = 0; col < 8; col++) {
					auto currentx = x + col;
					if (currentx >= display[0])
						continue;
					auto spriteBit = (spriteByte & (0x80 >> col));

					if (spriteBit) {
						SDL_Rect pixel = { currentx, currenty, 1, 1 };
						SDL_Point point = { currentx , currenty };
						if (SDL_PointInRect(&point, &pixel)) {
							std::cout << "Black - ";
							SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
							V[0xF] = 1;
						}
						else {
							std::cout << "White - ";
							SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						}
						std::cout << "Drew pixel at (" << currentx << ", " << currenty << ")\n";
						SDL_FRect pixel_f = { currentx, currenty, 1, 1 };
						SDL_RenderFillRect(renderer, &pixel_f);

					}
				}
			}*/


			break;
	}

	PC += 2;
}