#define MEMORY_SIZE 4096 //4k memory
#define REGISTER_SIZE 16
#define STACK_SIZE 16
#define KEY_SIZE 16
#define GFX_W 64 //Graphics width
#define GFX_H 32 //Graphics height
#define GFX_SIZE GFX_W*GFX_H

class Chip8 {
	unsigned short opcode;
	unsigned char memory[MEMORY_SIZE]; 
	unsigned char V[REGISTER_SIZE]; //Register

	unsigned short I; //Index register
	unsigned short pc; //Program counter

	unsigned char gfx[GFX_SIZE]; //Graphics

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[STACK_SIZE];
	unsigned short sp; //Stack pointer

	unsigned char key[KEY_SIZE];

public:
	void initialize();
	void cycle();
	void load(char* rom);

	Chip8(); //Constructor
	~Chip8(); //Destructor
};