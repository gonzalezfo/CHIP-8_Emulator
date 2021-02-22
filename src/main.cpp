#include "chip8.h"

#include <iostream>

Chip8 chip8;


int main(int argc, char** argv)
{
	chip8.Initialize();

	std::cout << "Welcome to Jesus Gonzalez's CHIP-8 Emulator\n";
	std::cout << "Write the name of the ROM you want to play\n";

	std::string filename;

	do {
		filename.clear();
		filename = "../data/";
		std::string aux;
		std::cin >> aux;
		filename = filename + aux + ".ch8";
	} while (!chip8.LoadGame(filename));


	
	// TODO Set graphics
	
	// TODO Set input

	// TODO Initialize CHIP8 (registers and memory once)

	// TODO Load Game

	// Emulation loop
		// Emulate one cycle (Fetch, Decode and Execute opcode)

		// If the draw flag is set, update the screen

		// Store key press state (Press and Release)

	return 0;
}