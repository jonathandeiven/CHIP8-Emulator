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

	unsigned short I;  //Index register
	unsigned short pc; //Program counter

	unsigned char gfx[GFX_SIZE]; //Graphics

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[STACK_SIZE];
	unsigned short sp; //Stack pointer

	unsigned char key[KEY_SIZE];

	long file_size; //Size of loaded RAM

public:
	bool drawFlag;

	void initialize();
	void cycle();
	void load(const char* rom);

	// Sprite draw
	void draw_sprite(unsigned short X, unsigned short Y, unsigned short height);

	// Debug methods
	void ram_dump();
	void cpu_dump();

	Chip8();  //Constructor
	~Chip8(); //Destructor
};