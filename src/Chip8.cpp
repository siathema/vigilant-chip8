#include "Chip8.hpp"
#include <stdlib.h>
#include <time.h>

chip8::chip8(char * memory) {
  mem = memory;
  srand(time(NULL));
}

chip8::~chip8() {

}

void chip8::run_instruction() {
  uint16_t opcode = mem[PC_reg];
  opcode = opcode<<8 | mem[PC_reg+1];
  uint16_t PC_dest = 0;
  
  if(opcode == 0x00E0) { // CLS -  clear the display
    //TODO: implement
  }
  else if(opcode == 0x00EE) { // RET - Return from subroutine

    PC_dest = mem[SP_reg];
    SP_reg--;

  }
  else if((opcode & 0xF000) == 0x1000) { // JP addr - Jump to lower 14-bit

    uint16_t addr = opcode&0x0FFF;
    PC_dest = addr;

  }
  else if((opcode & 0xF000) == 0x2000) { // CALL addr - Call subroutine

    SP_reg++;
    mem[SP_reg] = PC_reg;
    uint16_t addr = opcode&0x0FFF;
    PC_dest = addr;

  }
  else if((opcode & 0xF000) == 0x3000) { // SE Vx, byte - Skip net if Vx=b

    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content = DATA_reg[reg_index];
    uint8_t op_byte = (opcode&0x00FF);
    if(reg_content == op_byte) {
      PC_reg += 2;

    }
  }
  else if((opcode & 0xF000) == 0x4000) { // SNE Vx, byte - Skip next if Vx!=b

    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content = DATA_reg[reg_index];
    uint8_t op_byte = (opcode&0x00FF);
    if(reg_content != op_byte) {
      PC_reg += 2;
    }

  }
  else if((opcode & 0xF000) == 0x5000) { // SE Vx, Vy - Skip next if Vx=Vy

    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];

    if(reg_content_x == reg_content_y) {
      PC_reg += 2;
    }
    
  }
  else if((opcode & 0xF000) == 0x6000) { // LD Vx, byte - Set Vx = byte

    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t byte = opcode&0x00FF;
    DATA_reg[reg_index] = byte;
    
  }
  else if((opcode & 0xF000) == 0x7000) { // ADD Vx, byte - Vx = Vx+byte

    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t byte = opcode&0x00FF;
    DATA_reg[reg_index] = DATA_reg[reg_index] + byte;
    
  }
  else if((opcode & 0xF00F) == 0x8000) { // LD Vx, Vy - Set Vx=Vy

    uint16_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = reg_content_y;
    
  }
  else if((opcode & 0xF00F) == 0x8001) { // OR Vx, Vy - Set Vx= Vx OR Vy
    
    uint16_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = DATA_reg[reg_index] | reg_content_y;
    
  }
  else if((opcode & 0xF00F) == 0x8002) { // AND Vx, Vy - Set Vx= Vx AND Vy
    
    uint16_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = DATA_reg[reg_index] & reg_content_y;
    
  }
  else if((opcode & 0xF00F) == 0x8003) { // XOR Vx, Vy - Set Vx= Vx XOR Vy
    
    uint16_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = DATA_reg[reg_index] ^ reg_content_y;
    
  }
  else if((opcode & 0xF00F) == 0x8004) { // ADD Vx, Vy - Set Vx= Vx + Vy
    
    uint16_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    uint16_t sum = DATA_reg[reg_index] + reg_content_y;
    if(sum > 255) {
      DATA_reg[15] = 1;
    }
    DATA_reg[reg_index] = sum;
    
  }
  else if((opcode & 0xF00F) == 0x8005) { // SUB Vx, Vy - Set Vx= Vx - Vy
    
    uint16_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_x > reg_content_y) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_x - reg_content_y;
    
  }
  else if((opcode & 0xF00F) == 0x8006) { // SHR Vx, Vy - Vx>>1
    
    uint8_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_x & 0x01) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_x>>1;
    
  }
  else if((opcode & 0xF00F) == 0x8007) { // SUBN Vx, Vy - Vx = Vx - Vy
    
    uint8_t reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_y > reg_content_x) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_y - reg_content_x;
    
  }
  else if((opcode & 0xF00F) == 0x800E) { // SHR Vx, Vy - Vx = Vx<<1
    
    uint8_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_x & 0x01) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_x<<1;
    
  }
  else if((opcode & 0xF000) == 0x9000) { // SNE Vx, Vy - Skip next if Vx!=Vy
    
    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_content_y = DATA_reg[reg_index];

    if(reg_content_x != reg_content_y) {
      PC_reg += 2;
    }
    
  }
  else if((opcode & 0xF000) == 0xA000) { // LD I, addr - I = addr
    
    I_reg = opcode & 0x0FFF;
    
  }
  else if((opcode & 0xF000) == 0xB000) { // JP V0, addr
    
    uint16_t addr = opcode & 0x0FFF;
    PC_dest = addr + DATA_reg[0];
    
  }
  else if((opcode & 0xF000) == 0xC000) { //RND Vx, byte - set Vx=random AND b
    
    uint16_t reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = (rand() % 256) & (opcode&0x00FF);
    
  }
  else if((opcode & 0xF000) == 0xD000) { //DRW Vx, Vy, nibble
    //TODO: implement Graphics
  }
  else if((opcode & 0xF0FF) == 0xE09E) { //SKP Vx - skip if key-input = vX
    //TODO: implement Input
  }
  else if((opcode & 0xF0FF) == 0xE0A1) { //SKNP Vx - skip if key-input != vX
    //TODO: implement Input
  }
  else if((opcode & 0xF0FF) == 0xF007) { // LD Vx, DT - Set vx = delay timer

    uint16_t reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = TIMER_reg;
    
  }
  else if((opcode & 0xF0FF) == 0xF00A) { // LD Vx, K - wait for key, set in Vx
    //TODO: implement Input
  }
  else if((opcode & 0xF0FF) == 0xF015) { // LD DT, Vx - set delay timer = Vx

    uint16_t reg_index = (opcode&0x0F00)>>8;
    TIMER_reg = DATA_reg[reg_index];
    
  }
  else if((opcode & 0xF0FF) == 0xF018) { // LD ST, VX - set sound timer = Vx

    uint16_t reg_index = (opcode&0x0F00)>>8;
    SOUND_reg = DATA_reg[reg_index];

  }
  else if((opcode & 0xF0FF) == 0xF01E) { // ADD I, Vx - I = I + Vx

    uint16_t reg_index = (opcode&0x0F00)>>8;
    I_reg += DATA_reg[reg_index];

  }
  else if((opcode & 0xF0FF) == 0xF029) { // LD F, Vx - set I = address of hex-sprit of Vx

    //TODO: implement hex-sprites

  }
  else if((opcode & 0xF0FF) == 0xF033) { // LD B, Vx store in I, I+1, and I+2 the BCD representation of Vx

    // Get value stored in Vx
    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_value = DATA_reg[reg_index];

    // Calculate BCD value of Vx
    uint8_t value_hundreds = reg_value/100;
    reg_value %= 100;
    uint8_t value_tens = reg_value/10;

    // Store most significant byte in mem[I] and the least significant in mem[I+2] 
    mem[I_reg] = value_hundreds;
    mem[I_reg + 1] = value_tens;
    mem[I_reg + 2] = reg_value%10;

  }
  else if((opcode & 0xF0FF) == 0xF055) { // LD [I], Vx - store [V0..Vx] in memory starting at mem[I]

    uint16_t reg_index = (opcode&0x0F00)>>8;
    for(int i=0; i<=reg_index; i++) {

      mem[I_reg+i] = DATA_reg[i];

    }

  }
  else if((opcode & 0xFF0F) == 0xF065) { // LD Vx, [i] - store the memory starting at I in regs[V0..Vx]

    uint16_t reg_index = (opcode&0x0F00)>>8;
    for(int i=0; i<=reg_index; i++) {

      DATA_reg[i] = mem[I_reg+i]; 
    
    }
  }


  // Setting up next instruction
  if(PC_dest != 0) {
    PC_reg = PC_dest;
  } else {
    PC_reg += 2;
  }
  return;

  
}


void chip8::update() {

}
