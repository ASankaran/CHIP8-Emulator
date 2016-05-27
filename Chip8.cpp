#include "Chip8.h"

void Chip8::init() {
	for (int i = 0; i < sizeof(memory)/sizeof(memory[0]); ++i) {
		memory[i] = 0;
	}
	for (int i = 0; i < sizeof(V)/sizeof(V[0]); ++i) {
		V[i] = 0;
	}
	I = 0;
	pc = 0x200;
	for (int i = 0; i < sizeof(gfx)/sizeof(gfx[0]); ++i) {
		gfx[i] = 0;
	}
	delayTimer = 0;
	soundTimer = 0;
	for (int i = 0; i < sizeof(stack)/sizeof(stack[0]); ++i) {
		stack[i] = 0;
	}
	sp = 0;
	for (int i = 0; i < sizeof(key)/sizeof(key[0]); ++i) {
		key[i] = 0;
	}
}

void Chip8::runCycle() {
	//Concatenate two bytes into opcode 
	opcode = memory[pc] << 8 | memory[pc + 1];
}