#include "Emulator.h"
#include <chrono>
#include <iostream>
#include <fstream>

void Emulator::start()
{
	auto previousTimeIntruction = std::chrono::high_resolution_clock::now();
	auto previousTimeDisplay = previousTimeIntruction;
	while (true)
	{
		auto time = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(time - previousTimeIntruction).count() >= (1000000000 / INSTRUCTIONS_PER_SEC))
		{
			previousTimeIntruction = time;
			mpProcessor->loop();
		}

		if (std::chrono::duration_cast<std::chrono::nanoseconds>(time - previousTimeDisplay).count() >= (1000000000 / FPS))
		{
			previousTimeDisplay = time;
			mpDisplay->events();
			mpProcessor->tickTimers();
		}
	}
}

void Emulator::loadRom(std::string path)
{
	byte buffer[4096] = { 0 };
	size_t size = 0;
	std::ifstream rom(path, std::ifstream::binary);
	if (!rom)
	{
		printf("An error occured while opening the ROM file.");
		return;
	}
	rom.seekg(0, rom.end);
	size = rom.tellg();
	rom.seekg(0, rom.beg);
	rom.read((char*)buffer, size);
	rom.close();

	mpMemory->copyToMemory(PROG_START_ADDRESS, buffer, size);
}

Emulator::Emulator(std::string romPath)
{
	mpMemory = new Memory(MEM_SIZE);
	mpProcessor = new Processor(this);
	mpDisplay = new Display(mpProcessor);


	mpMemory->copyToMemory(FONT_START_ADDRESS, mFonts, sizeof(mFonts));
	loadRom(romPath);
}

Emulator::~Emulator()
{
}