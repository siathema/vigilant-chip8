#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>
#include "Screen.hpp"
#include "Chip8_Input.hpp"

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
  uint8_t SP_reg;

  // Program counter(PC)
  uint16_t PC_reg;

  // Pointer to main memory
  char *mem;

  // Timer
  double chip_timer;

  // Screen
  Screen screen;

  // Input
  Chip8_Input input;

public:
  Chip8(char * memory, Screen& r_screen, Chip8_Input& r_input);
  ~Chip8();
  void run_instruction();
  void update();
};

#endif







