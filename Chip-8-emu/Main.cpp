#include "Emulator.h"
#undef main
int main()
{
	Emulator* emulator = new Emulator("E:\\programok\\xampp8.1.5\\htdocs\\chip8-master\\roms\\BC_test.ch8");
	emulator->start();
	return 0;
}