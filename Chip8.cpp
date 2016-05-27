#include <stdio.h>
#include <cstdlib>
#include <ctime>

#include "Chip8.h"
#include "Fontset.h"

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
	for (int i = 0; i < sizeof(fontset)/sizeof(fontset[0]); ++i) {
		memory[i] = fontset[i];
	}
	drawFlag = true;
	srand(time(NULL));
}

void Chip8::loadProgram(const char* programFile) {
	FILE* file = fopen(programFile, "rb");
	unsigned char buf[3584];

	for (int i = 0; i < sizeof(buf)/sizeof(buf[0]); ++i) {
		buf[i] = 0;
	}

	fread(buf, 3584, 1, file);

	fclose(file);

	for (int i = 0; i < sizeof(buf)/sizeof(buf[0]); ++i) {
		memory[i + 0x200] = buf[i];
	}

	/*
	for (int i = 0; i < sizeof(memory)/sizeof(memory[0]); ++i) {
		printf ("0x%02X\n", memory[i]);
	*/
}

void Chip8::runCycle() {
	//Concatenate two bytes into opcode 
	opcode = memory[pc] << 8 | memory[pc + 1];

	//Match opcode to instruction
	switch(opcode & 0xF000) {
		case 0x0000:
			switch(opcode & 0x00FF) {
				//0x00E0
				case 0x00E0:
					for (int i = 0; i < sizeof(gfx)/sizeof(gfx[0]); ++i) {
						gfx[i] = 0;
					}
					drawFlag = true;
					pc += 2;
					break;
				//0x00EE
				case 0x00EE:
					pc = stack[--sp];
					pc += 2;
					break;
				//0x0NNN
				default:
					printf("Opcode: 0x%X not implemented.\n", opcode);
					exit(1);
			}
			break;
		//0x1NNN
		case 0x1000:
			pc = opcode & 0x0FFF;
			break;
		//0x2NNN
		case 0x2000:
			stack[sp++] = pc;
			pc = opcode & 0x0FFF;
			break;
		//0x3XNN
		case 0x3000:
			if(V[(opcode & 0x0F00) >> 8] == opcode & 0x00FF) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		//0x4XNN
		case 0x4000:
			if(V[(opcode & 0x0F00) >> 8] != opcode & 0x00FF) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		//0x5XY0
		case 0x5000:
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		//0x6XNN
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;
		//0x7XNN
		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
			break;
		case 0x8000:
			switch(opcode & 0x000F) {
				//0x8XY0
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				//0x8XY1
				case 0x0001:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				//0x8XY2
				case 0x0002:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				//0x8XY3
				case 0x0003:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				//0x8XY4
				case 0x0004:
					if(V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 0xFF) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				//0x8XY5
				case 0x0005:
					if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				//0x8XY6
				case 0x0006:
					V[0xF] = (V[(opcode & 0x0F00) >> 8] << 7) >> 7;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
					pc += 2;
					break;
				//0x8XY7
				case 0x0007:
					if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8] ;
					pc += 2;
					break;
				//0x8XYE
				case 0x000E:
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
					pc += 2;
					break;
				default:
					printf("Opcode: 0x%X not implemented.\n", opcode);
					exit(1);
			}
			break;
		//0x9XY0
		case 0x9000:
			if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		//0xANNN
		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		//0xBNNN
		case 0xB000:
			pc = (opcode & 0x0FFF) + V[0x0];
			break;
		//0xCXNN
		case 0xC000:
			V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
			pc += 2;
			break;
		//0xDXYN
		case 0xD000: {
			unsigned char xLoc = V[(opcode & 0x0F00) >> 8];
			unsigned char yLoc = V[(opcode & 0x00F0) >> 4];
			unsigned char height = opcode & 0x000F;
			unsigned char pixelSet = memory[I];
			V[0xF] = 0;
			for (int y = 0; y < height; ++y) {
				pixelSet = memory[I + y];
				for (int x = 0; x < 8; ++x) {
					//Isolate a single pixel, a bit, from pixelSet
					//Update gfx value if pixel is a 1
					if(pixelSet & (0x80 >> x) != 0) {
						//Check if current pixel value is 1
						if(gfx[xLoc + x + ((yLoc + y) * GFX_Y)] == 1) {
							V[0xF] = 1;
						}
						gfx[xLoc + x + ((yLoc + y) * GFX_Y)] ^= 1;
					}
				}
			}
			drawFlag = true;
			pc += 2;
			}
			break;
		case 0xE000:
			switch(opcode & 0x00FF) {
				//0xEX9E
				case 0x009E:
					if(key[V[(opcode & 0x0F00) >> 8]] != 0) {
						pc += 4;
					} else {
						pc += 2;
					}
					break;
				//0xEXA1
				case 0x00A1:
					if(key[V[(opcode & 0x0F00) >> 8]] == 0) {
						pc += 4;
					} else {
						pc += 2;
					}
					break;
				default:
					printf("Opcode: 0x%X not implemented.\n", opcode);
					exit(1);
			}
			break;
		case 0xF000:
			switch(opcode & 0x00FF) {
				//0xFX07
				case 0x0007:
					V[(opcode & 0x0F00) >> 8] = delayTimer;
					pc += 2;
					break;
				//0xFX0A
				case 0x000A: {
					bool haveKeyPress = false;
					for (int i = 0; i < sizeof(key)/sizeof(key[0]); ++i) {
						if(key[i] != 0) {
							haveKeyPress = true;
							V[(opcode & 0x0F00) >> 8] = i;
						}
					}
					//Don't increment pc until a key is pressed
					if(!haveKeyPress) {
						return;
					}
					pc += 2;
					}
					break;
				//0xFX15
				case 0x0015:
					delayTimer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				//0xFX18
				case 0x0018:
					soundTimer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				//0xFX1E
				case 0x001E:
					if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					I += V[(opcode & 0x0F00) >> 8];
					pc + 2;
					break;
				//0xFX29
				case 0x0029:
					//Multiply by 5, since character are 5 rows tall
					I =  V[(opcode & 0x0F00) >> 8] * 5;
					pc += 2;
					break;
				//0xFX33
				case 0x0033:
					memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I] = V[(opcode & 0x0F00) >> 8] / 10 % 10;
					memory[I] = V[(opcode & 0x0F00) >> 8] % 10;
					break;
				//0xFX55
				case 0x0055:
					for (int i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
						memory[I + i] = V[i];
					}
					pc += 2;
					break;
				//0xFX66
				case 0x0065:
					for (int i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
						V[i] = memory[I + i];
					}
					pc += 2;
					break;
				default:
					printf("Opcode: 0x%X not implemented.\n", opcode);
					exit(1);
			}
			break;
		default:
			printf("Opcode: 0x%X not implemented.\n", opcode);
			exit(1);
	}
}