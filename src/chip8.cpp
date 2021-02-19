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
		std::cout << "[ERROR]: File is NULL\n";
		return false;
	}

	// Loads the file and puts in a vector
	std::ifstream file(filepath, std::ios::in | std::ios::binary);
	std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	// Checks if the vector has been filled
	if (content.empty())
	{
		std::cout << "[ERROR]: File is Empty or does not exist\n";
		return false;
	}

	file.close();

	// Copy file content into memory from 0x200 == 512
	for (int i = 0; i < content.size(); ++i)
	{
		memory[i + 512] = content[i];
	}
}

void Chip8::EmulateCycle()
{
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	// Decode opcode
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
			case 0x0000: // 0x00E0: Clears the screen        
			  // Execute opcode
			break;

			case 0x000E: // 0x00EE: Returns from subroutine          
			  // Execute opcode
			break;

			default:
				std::cout << "[ERROR]: Unknown opcode [0x0000]: 0x" << opcode << '\n';
		}
	break;

	case 0x2000: // 0x2NNN: calls the subroutine at address NNN
		stack[sp] = pc;
		++sp;
		pc = opcode & 0x0FFF;
	break;

	case 0x8000:
		switch (opcode & 0x000F)
		{
			case 0x004: // 0x8XY4: Adds the value of VY to VX. VF is 1 if there's a carry, else is 0
				if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
				{
					V[0xF] = 1; //carry
				}
				else
				{
					V[0xF] = 0;
				}
				V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
				pc += 2;
			break;

			default:
				std::cout << "[ERROR]: Unknown opcode [0x0000]: 0x" << opcode << '\n';
		}
	break;

	case 0xD000: /* Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels 
				 and a height of N pixels. Each row of 8 pixels is read as bit-coded 
				 starting from memory location I; I value doesn’t change after the 
				 execution of this instruction. As described above, VF is set to 1 
				 if any screen pixels are flipped from set to unset when the sprite is drawn, 
				 and to 0 if that doesn’t happen. */
	{
		unsigned short x = V[(opcode & 0x0F00) >> 8]; 
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F; 
		unsigned short pixel;

		V[0xF] = 0; // Reset register VF
		for (int yline = 0; yline < height; yline++) // Loop over rows
		{
			pixel = memory[I + yline]; // Get pixel starting at I
			for (int xline = 0; xline < 8; xline++) // Loop over 8 bytes of the row
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gfx[(x + xline + ((y + yline) * 64))] == 1)
					{
						V[0xF] = 1;
					}
					gfx[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}
		drawFlag = true;
		pc += 2;
	}
	break;


	case 0xF000:
		switch (opcode & 0x00FF)
		{
			case 0x0033: // 0xFX33: Stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2
				memory[I]		= V[(opcode & 0x0F00) >> 8] / 100;
				memory[I + 1]	= (V[(opcode & 0x0F00) >> 8] / 10) % 10;
				memory[I + 2]	= (V[(opcode & 0x0F00) >> 8] % 100) % 10;
				pc += 2;
			break;

			default:
				std::cout << "[ERROR]: Unknown opcode [0x0000]: 0x" << opcode << '\n';
		}
	break;


	case 0xA000: // ANNN: Sets I to the address NNN
		I = opcode & 0x0FFF;
		pc += 2;
	break;


	default:
		std::cout << "[ERROR]: Unknown opcode: 0x" << opcode << '\n';
	break;
	}

	if (delay_timer > 0)
	{
		--delay_timer;
	}

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
		{
			std::cout << "[SOUND TIMER ALERT]\n";
		}
		--sound_timer;
	}
}

