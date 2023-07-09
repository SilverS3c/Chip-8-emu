#pragma once
#define PROG_START_ADDRESS 0x200
#define FONT_START_ADDRESS 0x50
#include <cstdint>
typedef unsigned char byte;

class Emulator;
class Processor
{
public:
	Processor(Emulator* emu);
	~Processor();
	void loop();
	void setup();
	bool waitKey = false;
	void setDataRegister(byte _register, byte value);
	byte waitKeyCode = 0;
	void tickTimers();
private:
	byte mDelayTimer = 0;
	byte mSoundTimer = 0;
	byte mDataRegisters[16] = { 0 };
	uint16_t I = 0;
	uint16_t PC = PROG_START_ADDRESS;

	Emulator* mEmu;
	uint16_t fetched = 0;
	struct OpCode {
		byte OP = 0;
		byte X = 0;
		byte Y = 0;
		byte N = 0;
		byte NN = 0;
		uint16_t NNN = 0;
	} mOpCode;

	uint16_t fetch();
	void decode();
};
