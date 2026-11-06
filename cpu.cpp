#include <iostream>
#include <fstream>
#include "cpu.hpp"

void Components::getrom() {

	std::ifstream Rom("IBM Logo.ch8", std::ios::binary | std::ios::ate);

	if (Rom.is_open()) {
		std::streampos size = Rom.tellg();
		char* contents = new char[size];

		Rom.seekg(0, std::ios::beg);
		Rom.read(contents, size);
		Rom.close();

		for (auto i = 0; i < sizeof(characters_fonts); ++i)
			memory[i] = characters_fonts[i];

		for (auto i = 0; i < size; ++i) {
			memory[0x200 + i] = contents[i];
			printf("\\x%.2x", memory[0x200 + i]);
			
		}

		delete[] contents;
	}
	else
		std::cout << "File not open";
}