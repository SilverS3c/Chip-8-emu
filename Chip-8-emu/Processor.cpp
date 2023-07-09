#include "Processor.h"
#include "Emulator.h"
#include <cstdlib>
#include <Windows.h>

void Processor::loop()
{
	if (waitKey) return;
	fetch();
	decode();
}

void Processor::setup()
{

}
uint16_t Processor::fetch()
{
	fetched = mEmu->mpMemory->readMemory(PC);
	fetched <<= 8;
	PC++;
	fetched |= mEmu->mpMemory->readMemory(PC);
	PC ++;
	return fetched;
}
void Processor::decode() 
{
	mOpCode.OP = (fetched >> 12) & 0x0F;
	mOpCode.X = (fetched >> 8) & 0x0F;
	mOpCode.Y = (fetched >> 4) & 0x0F;
	mOpCode.N = fetched & 0x0F;
	mOpCode.NN = fetched & 0xFF;
	mOpCode.NNN = fetched & 0x0FFF;

	byte flip = 0;
	switch (mOpCode.OP)
	{
	case 0x00:
		if (fetched == 0x00e0)
		{
			// Clear Screen
			mEmu->mpDisplay->clearScreen();
		}
		else if (fetched == 0x00ee)
		{
			// Return
			PC = mEmu->mStack.top();
			mEmu->mStack.pop();
		}
		else
		{

		}
		break;
	case 0x01:
		// Jump
		PC = mOpCode.NNN;
		break;
	case 0x02:
		// Call
		mEmu->mStack.push(PC);
		PC = mOpCode.NNN;
		break;
	case 0x03:
		// if (VX == NN)
		if (mDataRegisters[mOpCode.X] == mOpCode.NN) fetch();
		break;
	case 0x04:
		// if (VX != NN)
		if (mDataRegisters[mOpCode.X] != mOpCode.NN) fetch();
		break;
	case 0x05:
		if (mDataRegisters[mOpCode.X] == mDataRegisters[mOpCode.Y]) fetch();
		break;
	case 0x06:
		// VX = NN
		mDataRegisters[mOpCode.X] = mOpCode.NN;
		break;
	case 0x07:
		// VX += NN
		mDataRegisters[mOpCode.X] += mOpCode.NN;
		break;
	case 0x08:
		switch (mOpCode.N)
		{
		case 0x00:
			mDataRegisters[mOpCode.X] = mDataRegisters[mOpCode.Y];
			break;
		case 0x01:
			mDataRegisters[mOpCode.X] |= mDataRegisters[mOpCode.Y];
			break;
		case 0x02:
			mDataRegisters[mOpCode.X] &= mDataRegisters[mOpCode.Y];
			break;
		case 0x03:
			mDataRegisters[mOpCode.X] ^= mDataRegisters[mOpCode.Y];
			break;
		case 0x04:
		{
			byte vf = 0;
			if ((int)mDataRegisters[mOpCode.X] + (int)mDataRegisters[mOpCode.Y] > 255)
				vf = 1;
			else
				vf = 0;
			mDataRegisters[mOpCode.X] += mDataRegisters[mOpCode.Y];
			mDataRegisters[0xf] = vf;
			break;
		}
		case 0x05:
		{
			byte vf = 0;
			if (mDataRegisters[mOpCode.X] <= mDataRegisters[mOpCode.Y])
				vf = 0;
			else
				vf = 1;
			mDataRegisters[mOpCode.X] -= mDataRegisters[mOpCode.Y];
			mDataRegisters[0xf] = vf;
			break;
		}
		case 0x06:
		{
			byte vf = 0;
			vf = mDataRegisters[mOpCode.X] & 0x01;
			mDataRegisters[mOpCode.X] >>= 1;
			mDataRegisters[0xf] = vf;
			break;
		}
		case 0x07:
		{
			byte vf = 0;
			if (mDataRegisters[mOpCode.Y] <= mDataRegisters[mOpCode.X])
				vf = 0;
			else
				vf = 1;
			mDataRegisters[mOpCode.X] = mDataRegisters[mOpCode.Y] - mDataRegisters[mOpCode.X];
			mDataRegisters[0xf] = vf;
			break;
		}
		case 0x0E: 
		{
			byte vf = 0;
			vf = (mDataRegisters[mOpCode.X] >> 7) & 0x01;
			mDataRegisters[mOpCode.X] <<= 1;
			mDataRegisters[0xf] = vf;
			break;
		}
		default:
			break;
		}
		break;
	case 0x09:
		if (mDataRegisters[mOpCode.X] != mDataRegisters[mOpCode.Y]) fetch();
		break;
	case 0x0A:
		// I = NN
		I = mOpCode.NNN;
		break;
	case 0x0B:
		PC = mOpCode.NNN + mDataRegisters[0];
		break;
	case 0x0C:
		mDataRegisters[mOpCode.X] = (rand() & 0xFF) & mOpCode.NN;
		break;
	case 0x0D:
	{
		// Draw on screen
		mDataRegisters[0xf] = 0;
		byte pixelValue = 0;
		for (byte n = 0; n < mOpCode.N; n++)
		{
			pixelValue = mEmu->mpMemory->readMemory(I + n);
			for (byte bit = 0; bit < 8; bit++)
			{
				if (pixelValue)
				{
					uint16_t x = mDataRegisters[mOpCode.X] + bit;
					uint16_t y = mDataRegisters[mOpCode.Y] + n;

					flip += mEmu->mpDisplay->setPixel(x, y, pixelValue & 0x80);
				}
				
				pixelValue <<= 1;
			}
		}
		mDataRegisters[0x0F] = (flip > 0 ? 1 : 0);
		break;
	}
	case 0x0E:
		if (mOpCode.NN == 0x9E)
		{
			if (mEmu->mpDisplay->isKeyPressed(mDataRegisters[mOpCode.X])) fetch();
		}
		else if (mOpCode.NN == 0xA1)
		{
			if (!mEmu->mpDisplay->isKeyPressed(mDataRegisters[mOpCode.X])) fetch();
		}
		break;
	case 0x0F:
		switch (mOpCode.NN)
		{
		case 0x07:
			mDataRegisters[mOpCode.X] = mDelayTimer;
			break;
		case 0x0A:
			waitKeyCode = mOpCode.X;
			break;
		case 0x15:
			mDelayTimer = mDataRegisters[mOpCode.X];
			break;
		case 0x18:
			mSoundTimer = mDataRegisters[mOpCode.X];
			break;
		case 0x1E:
			I += mDataRegisters[mOpCode.X];
			break;
		case 0x29:
			I = FONT_START_ADDRESS + mDataRegisters[mOpCode.X] * 5;
			break;
		case 0x33:
			mEmu->mpMemory->writeByte(I, mDataRegisters[mOpCode.X] / 100);
			mEmu->mpMemory->writeByte(I + 1, (mDataRegisters[mOpCode.X] % 100) / 10);
			mEmu->mpMemory->writeByte(I + 2, (mDataRegisters[mOpCode.X] % 100) % 10);
			break;
		case 0x55:
			for (byte i = 0; i <= mOpCode.X; i++)
			{
				mEmu->mpMemory->writeByte(I + i, mDataRegisters[i]);
			}
			break;
		case 0x65:
			for (byte i = 0; i <= mOpCode.X; i++)
			{
				mDataRegisters[i] = mEmu->mpMemory->readMemory(I + i);
			}
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
}

Processor::Processor(Emulator* emu)
{
	this->mEmu = emu;
}

Processor::~Processor()
{
}

void Processor::setDataRegister(byte _register, byte value)
{
	mDataRegisters[_register] = value;
}

void Processor::tickTimers()
{
	if (mDelayTimer > 0) mDelayTimer--;
	if (mSoundTimer > 0) mSoundTimer--;
}