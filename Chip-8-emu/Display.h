#pragma once
#define WIDTH 64
#define HEIGHT 32
#define SDL_MAIN_HANDLED
#include <SDL2-2.28.1/include/SDL.h>
#include <map>
#include "Processor.h"

typedef unsigned char byte;

struct Color
{
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
};

class Display
{
public:
	Display(Processor* _cpu);
	~Display();
	void events();
	void clearScreen();
	bool getPixel(uint16_t index);
	bool getPixel(uint16_t x, uint16_t y);
	bool setPixel(uint16_t index, bool value);
	bool setPixel(uint16_t x, uint16_t y, bool value);
	bool* getMemory();
	bool isKeyPressed(byte key);
private:
	bool mDisplayMemory[WIDTH * HEIGHT] = { 0 };
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Processor* mProcessor;
	std::map<byte, int> keyMap = {
		{0, SDLK_x},
		{1, SDLK_1},
		{2, SDLK_2},
		{3, SDLK_3},
		{4, SDLK_q},
		{5, SDLK_w},
		{6, SDLK_e},
		{7, SDLK_a},
		{8, SDLK_s},
		{9, SDLK_d},
		{0xa, SDLK_y},
		{0xb, SDLK_c},
		{0xc, SDLK_4},
		{0xd, SDLK_r},
		{0xe, SDLK_f},
		{0xf, SDLK_v},
	};

	struct Color mBackgroundColor = { 0,0,0 };
	struct Color mForegroundColor = { 255,255,255 };
	std::map<int, bool> mKeys = {
		{SDLK_1, false},
		{SDLK_2, false},
		{SDLK_3, false},
		{SDLK_4, false},
		{SDLK_q, false},
		{SDLK_w, false},
		{SDLK_e, false},
		{SDLK_r, false},
		{SDLK_a, false},
		{SDLK_s, false},
		{SDLK_d, false},
		{SDLK_f, false},
		{SDLK_y, false},
		{SDLK_x, false},
		{SDLK_c, false},
		{SDLK_v, false},
	};
};

