#include "chip8.h"
#include "SDL.h"


Chip8 chip8;

int main(int argc, char* argv[])
{

	chip8.Initialize();

	std::cout << "***Welcome to Jesus Gonzalez's CHIP-8 Emulator***\n\n";
	std::cout << "Write the name of the ROM you want to play\n";

	std::string filename;

	do {
		filename.clear();
		filename = "../data/";
		std::string aux;
		std::cin >> aux;
		filename = filename + aux + ".ch8";
	} while (!chip8.LoadGame(filename));


	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, 64, 32, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_Event event;

	int pitch;
	Uint32* pixels;
	
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	chip8.Expansion(chip8.gfx, (uint32_t*)pixels);
	SDL_UnlockTexture(texture);

	int Quit = 0;
	int last_tick = 0;
	int cycles = 0;

	while (!Quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Quit = 1;
				break;
			}
		}

		if (SDL_GetTicks() - cycles > 1) {
			chip8.EmulateCycle();
			cycles = SDL_GetTicks();
		}

		if (SDL_GetTicks() - last_tick > (1000 / 60))
		{
			SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
			chip8.Expansion(chip8.gfx, (uint32_t*)pixels);
			SDL_UnlockTexture(texture);

			//SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			//SDL_WaitEvent(&event);
			last_tick = SDL_GetTicks();
		}
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();

	return 0;
}