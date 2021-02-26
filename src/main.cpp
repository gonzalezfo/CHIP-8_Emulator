#include "chip8.h"
#include "utils.h"


Chip8 chip8;
Utils utils;


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


	utils.Init();
	utils.ExpandTexture(chip8);

	SDL_Event event;

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
			if (chip8.wait_key == -1)
			{
				chip8.EmulateCycle(utils);
			}
			else
			{
				for (int key = 0; key <= 0xF; ++key)
				{
					if (utils.IsKeyDown(key))
					{
						chip8.V[(int)chip8.wait_key] = key;
						chip8.wait_key = -1;
						break;
					}
				}
			}
			cycles = SDL_GetTicks();
		}

		if (SDL_GetTicks() - last_tick > (1000 / 60))
		{
			utils.ExpandTexture(chip8);
			utils.Render();
			last_tick = SDL_GetTicks();
		}
	}
	
	utils.Destroy();

	return 0;
}