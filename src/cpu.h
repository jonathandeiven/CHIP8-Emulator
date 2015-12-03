#define MEMORY_SIZE 4096 //4k memory
#define REGISTER_SIZE 16
#define STACK_SIZE 16
#define KEY_SIZE 16

unsigned short opcode;
unsigned char memory[MEMORY_SIZE]; 
unsigned char V[REGISTER_SIZE]; //Register

unsigned short I; //Index register
unsigned short pc; //Program counter

unsigned char gfx[64 * 32]; //Graphics

unsigned char delay_timer;
unsigned char sound_timer;

unsigned short stack[STACK_SIZE];
unsigned short sp;

unsigned char key[KEY_SIZE];
