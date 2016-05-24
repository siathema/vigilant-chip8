#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>

class chip8 {

  // CHIP8 data registers V0..VF
  uint16_t v0_reg; 
  uint16_t v1_reg;
  uint16_t v2_reg;
  uint16_t v3_reg;
  uint16_t v4_reg;
  uint16_t v5_reg;
  uint16_t v6_reg;
  uint16_t v7_reg;
  uint16_t v8_reg;
  uint16_t v9_reg;
  uint16_t va_reg;
  uint16_t vb_reg;
  uint16_t vc_reg;
  uint16_t vd_reg;
  uint16_t ve_reg;
  uint16_t vf_reg;

  // Stack pointer (SP)
  uint8_t SP_reg;

  // Program counter(PC)
  uint16_t PC_reg;

  // Pointer to main memory
  uint16_t *mem;

public:
  chip8(uint8_t* memory);
  ~chip8();
  void run_instruction(uint16_t opcode);
  void update();
}

#endif








