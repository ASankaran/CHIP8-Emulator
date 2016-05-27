#ifndef GFX_X
#define GFX_X 32
#endif

#ifndef GFX_Y
#define GFX_Y 64
#endif

class Chip8 {
public:
	void init();
	void runCycle();
	void loadProgram(const char*);
	//Keypad states
	unsigned char key[16];
	bool drawFlag;
private:
	//4KB memory
	unsigned char memory[4096];
	//Registers
	unsigned char V[16];
	//Index register
	unsigned short I;
	//Program counter
	unsigned short pc;
	//Graphic Pixels
	unsigned char gfx[GFX_X * GFX_Y];
	//Timer registers
	unsigned char delayTimer;
	unsigned char soundTimer;
	//Stack memory
	unsigned short stack[16];
	//Stack Pointer
	unsigned short sp;
	//current opcode
	unsigned short opcode;
};