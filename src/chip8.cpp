#include "chip8.h"

#include <fstream>
#include <sstream>
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
	SDL_memcpy(memory + 0x50, chip8_fontset, 80);

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	srand(time(NULL));

}

bool Chip8::LoadGame(const std::string& filepath)
{
	// Check parameter errors
	if (filepath.empty())
	{
		std::cout << "[ERROR]: Could not load the ROM\n";
		return false;
	}

	// Loads the file and puts in a vector
	std::ifstream file(filepath, std::ios::in | std::ios::binary);
	std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	// Checks if the vector has been filled
	if (content.empty())
	{
		std::cout << "[ERROR]: Could not load the ROM or the content is corrupted\n";
		return false;
	}

	// Copy file content into memory from 0x200 == 512
	for (int i = 0; i < content.size(); ++i)
	{
		memory[i + 0x200] = content[i];
	}
	file.close();
	return true;
}

void Chip8::EmulateCycle()
{
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
	pc = (pc + 2) & 0xFFF;


	uint16_t NNN	= opcode & 0x0FFF;		// address
	uint8_t NN		= opcode & 0xFF;		// 8-bit constant
	uint8_t N		= opcode & 0xF;			// 4-bit constant
	uint8_t X		= (opcode >> 8) & 0xF;  // 4-bit register identifier
	uint8_t Y		= (opcode >> 4) & 0xF;	// 4-bit register identifier
	uint8_t P		= (opcode >> 12);		// most significative 4 bits


	switch (P)
	{
	case 0:
		if (opcode == 0x00E0) // 00E0: Clears the screen.
		{
			SDL_memset(gfx, 0x0, 2048);
		}
		else if (opcode == 0x00EE) // 00EE: Returns from a subroutine
		{
			if (sp > 0)
			{
				pc = stack[--sp];
			}
		}
		break;
	case 1: // 1NNN: Jumps to address NNN
		pc = NNN;
		break;
	case 2: // 0x2NNN: Calls subroutine at NNN.
		if (sp < 16)
		{
			stack[sp++] = pc;
		}
		pc = NNN;
		break;
	case 3: // 3XNN: Skips the next instruction if VX equals NN.
		if (V[X] == NN)
		{
			pc = (pc + 2) & 0xFFF;
		}
		break;
	case 4: // 4XNN: Skips the next instruction if VX doesn't equal NN
		if (V[X] != NN)
		{
			pc = (pc + 2) & 0xFFF;
		}
		break;
	case 5: // 5XY0: Skips the next instruction if VX equals VY
		if (V[X] == V[Y])
		{
			pc = (pc + 2) & 0xFFF;
		}
		break;
	case 6: // 6XNN: Sets VX to NN
		V[X] = NN;
		break;
	case 7: // 7XNN: Adds NN to VX. (Carry flag is not changed)
		V[X] = (V[X] + NN) & 0xFF;
		break;
	case 8:
		switch (N)
		{
		case 0: // 8XY0: Sets VX to the value of VY.
			V[X] = V[Y];
			break;
		case 1: // 8XY1: Sets VX to VX or VY. (Bitwise OR operation)
			V[X] |= V[Y];
			break;
		case 2: // 8XY2: Sets VX to VX and VY. (Bitwise AND operation)
			V[X] &= V[Y];
			break;
		case 3: // 8XY3: Sets VX to VX xor VY.
			V[X] ^= V[Y];
			break;
		case 4: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			V[0xF] = (V[X] > V[X] + V[Y]);
			V[X] += V[Y];
			break;
		case 5: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			V[0xF] = (V[X] > V[Y]);
			V[X] -= V[Y];
			break;
		case 6: // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
			V[0xF] = (V[X] & 1);
			V[X] >>= 1;
			break;
		case 7: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			V[0xF] = V[Y] > V[X];
			V[X] = V[Y] - V[X];
			break;
		case 0xE: // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
			V[0xF] = ((V[X] & 0x80) != 0);
			V[X] <<= 1;
			break;
		default:
			std::cout << "[ERROR]: Unknown opcode: 0x" << opcode << '\n';
			break;
		}
		break;
	case 9: // 9XY0: Skips the next instruction if VX doesn't equal VY
		if (V[X] != V[Y])
		{
			pc = (pc + 2) & 0xFFF;
		}
		break;
	case 0xA: // ANNN: Sets I to the address NNN
		I = NNN;
		break;
	case 0xB: // BNNN: Jumps to the address NNN plus V0.
		pc = (NNN + V[0]) & 0xFFF;
		break;
	case 0xC: // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		V[X] = rand() & NN;
		break;
	case 0xD: /* DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels 
					   and a height of N pixels.Each row of 8 pixels is read as bit - coded
					   starting from memory location I; I value doesn’t change after the
					   execution of this instruction.As described above, VF is set to 1
					   if any screen pixels are flipped from set to unset when the sprite is drawn,
					   and to 0 if that doesn’t happen.*/
		V[15] = 0;
		for (int j = 0; j < N; j++)
		{
			uint8_t sprite = memory[I + j];
			for (int i = 0; i < 8; ++i)
			{
				int px = (V[X] + i) & 63;
				int py = (V[Y] + j) & 31;
				int pos = 64 * py + px;
				int pixel = (sprite & (1 << (7 - i))) != 0;

				V[15] |= (gfx[pos] & pixel);
				gfx[pos] ^= pixel;
			}
		}
		break; 
	case 0xE:
		if (NN == 0x9E) // EX9E: Skips the next instruction if the key stored in VX is pressed. 
		{
			if (key[V[X]] != 0)
			{
				pc += 4;
			}
		}
		else if (NN == 0xA1) // EXA1: Skips the next instruction if the key stored in VX isn't pressed.
		{
			if (key[V[X]] == 0)
			{
				pc += 4;
			}
		}
		break;
	case 0xF:
		switch (NN)
		{
		case 0x07: // FX07: Sets VX to the value of the delay timer.
			V[X] = delay_timer;
			break;
		case 0x0A: // FX0A: A key press is awaited, and then stored in VX.
		{
			bool keyPress = false;

			for (int i = 0; i < 16; ++i)
			{
				if (key[i] != 0)
				{
					V[X] = i;
					keyPress = true;
				}
			}
			if (!keyPress)
			{
				return;
			}
		}
			break;
		case 0x15: // FX15: Sets the delay timer to VX.
			delay_timer = V[X];
			break;
		case 0x18: // FX18: Sets the sound timer to VX.
			sound_timer = V[X];
			break;
		case 0x1E: // FX1E: Adds VX to I.
			// TO DO: Spacefight 2091! needs VF to be affected
			//		  Animal Race needs it NOT to be affected	
			I += V[X];
			break;
		case 0x29: /* FX29: Sets I to the location of the sprite for the character in VX. 
							Characters 0-F (in hexadecimal) are represented by a 4x5 font. */
			I = 0x50 + (V[X] & 0xF) * 5;
			break;
		case 0x33: /* FX33: Stores the binary-coded decimal representation of VX, 
							with the most significant of three digits at the address in I,
							the middle digit at I plus 1, and the least significant 
							digit at I plus 2. */
			memory[I] = V[X] / 100;
			memory[I + 1] = (V[X] / 10) % 10;
			memory[I + 2] = V[X] % 10;
			break;
		case 0x55: /* FX55: Stores V0 to VX (including VX) in memory starting at address I.
							The offset from I is increased by 1 for each value written, 
							but I itself is left unmodified.  */
			for (int i = 0; i <= X; ++i)
			{
				memory[I + i] = V[i];
			}

			I += X + 1;
			break;
		case 0x65: /* FX65: Fills V0 to VX (including VX) with values from memory 
							starting at address I. The offset from I is increased by 1 
							for each value written, but I itself is left unmodified.*/
			for (int i = 0; i <= X; ++i)
			{
				V[i] = memory[I + i];
			}

			I += X + 1;
			break;
		default:
			std::cout << "[ERROR]: Unknown opcode: 0x" << opcode << '\n';
			break;
		}
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

void Chip8::Expansion(unsigned char* from, uint32_t* to)
{
	for (int i = 0; i < 2048; ++i)
	{
		to[i] = (from[i]) ? -1 : 0;
	}
}

