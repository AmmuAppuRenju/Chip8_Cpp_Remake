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

		for (auto i = 0; i < 80; ++i)
			memory[i] = characters_fonts[i];

		for (auto i = 0; i < size; ++i)
			memory[0x200 + i] = contents[i];
		

		delete[] contents;
	}
	else
		std::cout << "No file found";
}

void Components::gameLoop(){
	uint8_t* byte1 = &memory[PC];
	uint8_t* byte2 = &memory[PC + 1];

	uint16_t opcode = (*byte1 << 8) | *byte2;

	opchecker(opcode);

	/*printf("\\x%.04x\n", opcode);*/
}

void Components::opchecker(int16_t opcode) {

	uint8_t first_value = (opcode & 0xf000) >> 12;

	uint8_t X = (opcode & 0x0f00) >> 8;

	uint8_t Y = (opcode & 0x00f0) >> 4;

	uint8_t N = opcode & 0x000f;

	//these are larger than one byte so the need int16_t
	uint16_t NN = opcode & 0x00ff;

	uint16_t NNN = opcode & 0x0fff;

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
			//std::cout << "Jump\n";
			PC = NNN - 2;
			break;
		case 0x2:
			PC = stack->top();
			PC = NNN - 2;
			break;
		case 0x3:
			if (V[X] == NN)
				PC += 2;
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
			uint16_t x = V[X] % display[0];
			uint16_t y = V[Y] % display[1];

			V[0xF] = 0;

			for (uint8_t row = 0; row < N; ++row) {
				uint8_t spriteByte = memory[I + row];
				uint8_t currenty = (y + row);
				if (currenty >= display[1]) break;

				for (uint8_t col = 0; col < 8; ++col) {
					uint8_t currentx = (x + col);
					if (currentx >= display[0]) break;

					uint8_t spriteBit = (spriteByte & (0x80 >> col));
					uint32_t* screenPixel = &pixels[currenty * display[0] + currentx];

					if (spriteBit) {
						if (*screenPixel == 0xFFFFFFFF) {
							V[0xF] = 1;
						}

						*screenPixel = 0xFFFFFFFF;
						std::cout << "Drew pixel at (" << x << ", " << y << "). \n";
					}
				}
			}

			SDL_UpdateTexture(texture, nullptr, pixels, display[0] * sizeof(pixels[0]));
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, texture, nullptr, nullptr);
			SDL_RenderPresent(renderer);

			break;
	}

	PC += 2;
}