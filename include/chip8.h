#pragma once

#include <string>
#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "SDL.h"
#include "utils.h"

#define MEMORYSIZE 4096

class Utils;

class Chip8
{
public:
	Chip8();
	~Chip8();

	void Initialize();
	bool LoadGame(const std::string& filepath);
	void EmulateCycle(Utils utils);
	void Expansion(unsigned char* from, uint32_t* to);

public:
	unsigned short opcode; // 35 opcodes in total
	unsigned char memory[MEMORYSIZE]; // 4K of memory
	unsigned char V[16]; // 15 8-bit general purpose registers + 1 carry flag

	unsigned short I; // index register
	unsigned short pc; // Program Counter

	unsigned char gfx[64 * 32]; // 2048 pixels in the screen

	// Timer registers instead of Interrupt and HW registers
	// They count at 60Hz
	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16]; // Used to remember the current location before a jump is performed
	unsigned short sp; // Stack pointer to the level of the stack

	char wait_key;

	unsigned char chip8_fontset[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
};