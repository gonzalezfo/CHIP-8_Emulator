#pragma once

#include "SDL.h"
#include "chip8.h"

class Chip8;

class Utils
{
public:
	Utils();
	~Utils();

	void Init();
	int IsKeyDown(char keypressed);
	void ExpandTexture(Chip8& chip8);
	void Render();
	void Destroy();

	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	SDL_Texture* GetTexture() { return texture; }
	SDL_Surface* GetSurface() { return surface; }
	int GetPitch() { return pitch; }
	Uint32* GetPixel() { return pixels; }

	unsigned char key[16]; // HEX based keypad

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Surface* surface;
	int pitch;
	Uint32* pixels;

};