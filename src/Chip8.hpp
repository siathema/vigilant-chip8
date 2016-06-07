#ifndef CHIP8_H
#define CHIP8_H
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SFML/Window.hpp>
#include "Screen.hpp"
#include "Chip8_Input.hpp"

const int MEMORY_SIZE = 4096;

// hex sprites
const uint8_t hex_sprites[16*5] = { 0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
			          0x20, 0x60, 0x20, 0x20, 0x70,  // 1
		                  0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
			          0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
	                          0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
		                  0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
		                  0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
		                  0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
	                          0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
                                  0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
                                  0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
                                  0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
                                  0xF0, 0x80, 0x80, 0x80, 0x90,  // C
                                  0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
				  0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
				  0xF0, 0x80, 0xF0, 0x80, 0x80}; // F 

class Chip8 {

  // CHIP8 data registers (V0..VF)
  uint8_t DATA_reg[16];

  // Address register(I)
  uint16_t I_reg;

  // Timer register
  uint8_t TIMER_reg;

  // Sound timer register
  uint8_t SOUND_reg;
  
  // Stack pointer (SP)
  uint16_t SP_reg;

  // Program counter(PC)
  uint16_t PC_reg;

  // Pointer to main memory
  uint8_t mem[MEMORY_SIZE];

  // Timer
  double chip_timer;

  // Screen
  Screen screen;

  // Input
  Chip8_Input input;

  
public:
  Chip8(char * chip8_program, int program_size);
  ~Chip8();
  void init(char* chip8_program, int program_size);
  void run_instruction(uint16_t opcode, bool debug=false);
  void update(bool debug=false);
  uint16_t get_PC();
  uint8_t get_mem(uint16_t address);
  uint8_t get_V_regs(int index);
  uint16_t get_I();
  uint16_t get_SP();
  uint8_t get_timer();
  uint8_t get_sound();
  void print_state();
};

#endif
