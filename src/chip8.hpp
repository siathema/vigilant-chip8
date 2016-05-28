#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>

class chip8 {

  // CHIP8 data registers V0..VF
  uint8_t DATA_reg[16];

  // Stack pointer (SP)
  uint8_t SP_reg;

  // Program counter(PC)
  uint16_t PC_reg;

  // Pointer to main memory
  char *mem;

public:
  chip8(char * memory);
  ~chip8();
  void run_instruction();
  void update();
};







