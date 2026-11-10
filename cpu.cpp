#include <iostream>
#include <fstream>
#include <vector>
#include "cpu.hpp"

void Components::getrom(const char* rom_path) {

	std::ifstream Rom(rom_path, std::ios::binary | std::ios::ate); // std::ios::binary reads the binary and std::ios::ate move ths file reading cursor to the end

	if (Rom.is_open()) {
		std::streampos size = Rom.tellg();
		char* contents = new char[size];

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
	int8_t byte1 = memory[PC];
	int8_t byte2 = memory[PC + 1];

	int16_t opcode = (byte1 << 8) | byte2;

	opchecker(opcode);

	//printf("\\x%.04x\n", opcode);
}

void Components::opchecker(int16_t opcode) {

	int8_t first_value = (opcode & 0xf000) >> 12;

	int8_t X = (opcode & 0x0f00) >> 8;

	int8_t Y = (opcode & 0x00f0) >> 4;

	int8_t N = opcode & 0x000f;

	int8_t NN = opcode & 0x00ff;

	int8_t NNN = opcode & 0x0fff;

	switch (first_value) {
		case 0xD:
			break;
	}

	PC += 2;
}