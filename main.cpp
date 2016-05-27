#include "Chip8.h"

Chip8 chip8;

int main(int argc, char const *argv[])
{
	chip8.init();
	chip8.loadProgram("C:\\Users\\Arnav\\Desktop\\pong2.c8");
	while(1) {
		chip8.runCycle();
	}
	return 0;
}