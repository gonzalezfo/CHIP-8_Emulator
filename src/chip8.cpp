#include "chip8.h"
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

