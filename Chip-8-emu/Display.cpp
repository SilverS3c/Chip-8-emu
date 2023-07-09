#include "Display.h"
#include <iostream>

Display::Display(Processor* _cpu)
{
	mProcessor = _cpu;
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	mWindow = SDL_CreateWindow("CHIP-8",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH*10, HEIGHT*10, 0);

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(mRenderer, WIDTH, HEIGHT);

	
}

Display::~Display()
{
}

void Display::events()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type)
	{
	case SDL_QUIT:
		SDL_DestroyWindow(mWindow);
		SDL_Quit();
		break;
	case SDL_KEYDOWN:
		if (mKeys.find(e.key.keysym.sym) != mKeys.end())
		{
			mKeys[e.key.keysym.sym] = true;
			if (mProcessor->waitKey)
			{
				mProcessor->waitKey = false;
				byte code = 0;
				for (const auto& key : keyMap)
				{
					if (key.second == e.key.keysym.sym)
					{
						code = key.first;
						break;
					}
				}
				mProcessor->setDataRegister(mProcessor->waitKeyCode, code);
			}
		}
		break;
	case SDL_KEYUP:
		if (mKeys.find(e.key.keysym.sym) != mKeys.end())
		{
			mKeys[e.key.keysym.sym] = false;
		}
		break;
	default:
		break;
	}
	SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 255);
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, mForegroundColor.r, mForegroundColor.g, mForegroundColor.b, 255);
	for (int i = 0; i < sizeof(mDisplayMemory); i++)
	{
		if (mDisplayMemory[i])
		{
			SDL_RenderDrawPoint(mRenderer, i % WIDTH, i / WIDTH);
		}
	}
	SDL_RenderPresent(mRenderer);
}

void Display::clearScreen()
{
	for (int i = 0; i < (WIDTH * HEIGHT); i++)
	{
		mDisplayMemory[i] = 0;
	}
}

bool Display::getPixel(uint16_t index)
{
	return mDisplayMemory[index];
}

bool Display::getPixel(uint16_t x, uint16_t y)
{
	return mDisplayMemory[y * WIDTH + x];
}

bool Display::setPixel(uint16_t index, bool value)
{
	bool prev = mDisplayMemory[index];
	mDisplayMemory[index] ^= value;
	return prev != value;
}

bool Display::setPixel(uint16_t x, uint16_t y, bool value)
{
	x = x % WIDTH;
	y = y % HEIGHT;
	return setPixel(y * WIDTH + x, value);
}

bool* Display::getMemory()
{
	return mDisplayMemory;
}

bool Display::isKeyPressed(byte key)
{
	return mKeys[keyMap[key]];
}