#include "chip8.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "SDL_stdinc.h"


Chip8::Chip8()
{
}

Chip8::~Chip8()
{
}

void Chip8::Initialize()
{
	pc		= 0x200;		// Program Counter Start Address
	opcode	= 0x0;		
	I		= 0x0;
	sp		= 0x0;

	// Clear display
	SDL_memset(gfx, 0x0, sizeof(gfx));

	// Clear stack
	SDL_memset(stack, 0x0, sizeof(stack));

	// Clear registers V0-VF
	SDL_memset(V, 0x0, sizeof(V));
	SDL_memset(key, 0x0, sizeof(key));

	// Clear memory
	SDL_memset(memory, 0x0, sizeof(memory));

	// Load fontset
	for (int i = 0; i < 80; ++i)
	{
		memory[i] = chip8_fontset[i];
	}

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	drawFlag = true;
}

bool Chip8::LoadGame(const std::string& filepath)
{
	// Check parameter errors
	if (filepath.empty())
	{
		std::cout << ("[ERROR]: File is NULL") << std::endl;
		return false;
	}

	// Loads the file and puts in a vector
	std::ifstream file(filepath, std::ios::in | std::ios::binary);
	std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	// Checks if the vector has been filled
	if (content.empty())
	{
		std::cout << ("[ERROR]: File is Empty or does not exist");
		return false;
	}

	file.close();

	// Copy file content into memory from 0x200 == 512
	for (int i = 0; i < content.size(); ++i)
	{
		memory[i + 512] = content[i];
	}
}

