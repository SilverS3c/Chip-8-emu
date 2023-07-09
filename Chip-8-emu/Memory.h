#pragma once
#include <list>

typedef unsigned char byte;

#include <stdio.h>
#include <string.h>

class Memory
{
public:
	Memory(size_t size);
	~Memory();
	void copyToMemory(size_t dst, void* src, size_t size);
	void copyFromMemory(void* dst, size_t src, size_t size);
	byte readMemory(size_t address);
	void writeByte(size_t address, byte data);
private:
	byte* mpRam;
};

