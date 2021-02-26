#include "utils.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

int Utils::IsKeyDown(char keypressed)
{
	const uint8_t* sdl_keys = SDL_GetKeyboardState(NULL);
	uint8_t real_key = key[(int)keypressed];
	return sdl_keys[real_key];
}

void Utils::ExpandTexture(Chip8& chip8)
{
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	chip8.Expansion(chip8.gfx, (uint32_t*)pixels);
	SDL_UnlockTexture(texture);
}

void Utils::Render()
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Utils::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
}

void Utils::Init()
{
	SDL_memset(key, 0x0, sizeof(key));

	key[0] =  SDL_SCANCODE_X;	//0
	key[1] =  SDL_SCANCODE_1;	//1
	key[2] =  SDL_SCANCODE_2;	//2
	key[3] =  SDL_SCANCODE_3;	//3
	key[4] =  SDL_SCANCODE_Q;	//4
	key[5] =  SDL_SCANCODE_W;	//5
	key[6] =  SDL_SCANCODE_E;	//6
	key[7] =  SDL_SCANCODE_A;	//7
	key[8] =  SDL_SCANCODE_S;	//8
	key[9] =  SDL_SCANCODE_D;	//9
	key[10] = SDL_SCANCODE_Z;	//A
	key[11] = SDL_SCANCODE_C;	//B
	key[12] = SDL_SCANCODE_4;	//C
	key[13] = SDL_SCANCODE_R;	//D
	key[14] = SDL_SCANCODE_F;	//E
	key[15] = SDL_SCANCODE_V;	//F


	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	surface = SDL_CreateRGBSurface(0, 64, 32, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}
