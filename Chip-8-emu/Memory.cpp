#include "Memory.h"
#include <stdio.h>
#include <string.h>

void Memory::copyToMemory(size_t dst, void* src, size_t size)
{
	memcpy(mpRam + dst, src, size);
}

void Memory::copyFromMemory(void* dst, size_t src, size_t size)
{
	memcpy(dst, mpRam + src, size);
}

byte Memory::readMemory(size_t address)
{
	return mpRam[address];
}

void Memory::writeByte(size_t address, byte data)
{
	mpRam[address] = data;
}

Memory::Memory(size_t size)
{
	mpRam = (byte*)malloc(size);
}

Memory::~Memory()
{
	free(mpRam);
}