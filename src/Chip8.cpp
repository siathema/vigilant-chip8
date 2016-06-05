#include "Chip8.hpp"

Chip8::Chip8(char* chip8_program, int program_size): screen(640, 320), input() {

  init(chip8_program, program_size);

}

void Chip8::init(char* chip8_program, int program_size) {

  srand(time(NULL));
  chip_timer = 0.0;
  memset(mem, 0x00, MEMORY_SIZE*sizeof(char));
  memset(DATA_reg, 0x00, 16*sizeof(uint8_t));
  I_reg = 0x0000;
  SOUND_reg = TIMER_reg = 0x00;
  SP_reg = 0x0100;
  PC_reg = 0x0200;
  
  memcpy(mem, hex_sprites, 16*5*sizeof(uint8_t));
  memcpy(mem+0x0200, chip8_program, program_size*sizeof(char));
  
}

Chip8::~Chip8() {

}

void Chip8::run_instruction() {
  uint16_t opcode = mem[PC_reg];
  opcode = opcode<<8 | mem[PC_reg+1];
  uint16_t PC_dest = 0;
  if((opcode&0xF000)==0x0000 && (opcode!=0x00E0) && (opcode!=0x00EE)) { // SYS addr - jump to subroutine at addr

    SP_reg++;
    mem[SP_reg] = PC_reg;
    uint16_t addr = opcode&0x0FFF;
    PC_dest = addr;


  }
  else if(opcode == 0x00E0) { // CLS -  clear the display
    screen.clear_screen();
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
    bool is_collision = false;
    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_value_x = DATA_reg[reg_index];
    reg_index = (opcode&0x00F0)>>4;
    uint8_t reg_value_y = DATA_reg[reg_index];
    uint8_t sprite_index = (opcode&0x000F);

    for(int i=0; i<sprite_index; i++) {

     
      if(reg_value_y + i >= SCREEN_HEIGHT) {
	if(screen.draw_byte(mem[I_reg+i], reg_value_x, i)) {

	  is_collision = true;
	  
	}
      }
      
      if(screen.draw_byte(mem[I_reg+i], reg_value_x, reg_value_y+i)) {
	is_collision = true;
      }
      
    }
  }
  else if((opcode & 0xF0FF) == 0xE09E) { //SKP Vx - skip if key-input = vX

    // Get value stored in Vx
    uint16_t reg_index = (opcode&0x0F00)>>8;
    if(input.is_key_pressed(DATA_reg[reg_index])) {
	PC_reg += 2;
      }
  }
  else if((opcode & 0xF0FF) == 0xE0A1) { //SKNP Vx - skip if key-input != vX

    //Get value stored in Vx
    uint16_t reg_index = (opcode&0x0F00)>>8;
    if(!input.is_key_pressed(DATA_reg[reg_index])) {
      PC_reg += 2;
    }
    
  }
  else if((opcode & 0xF0FF) == 0xF007) { // LD Vx, DT - Set vx = delay timer

    uint16_t reg_index = (opcode&0x0F00)>>8;
    DATA_reg[reg_index] = TIMER_reg;
    
  }
  else if((opcode & 0xF0FF) == 0xF00A) { // LD Vx, K - wait for key, set in Vx
    bool cont = false;

    // Get value stored in Vx
    uint16_t reg_index = (opcode&0x0F00)>>8;

    while(cont) {
      cont = input.is_key_pressed(DATA_reg[reg_index]);
    }
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

    uint16_t reg_index = (opcode&0x0F00)>>8;
    uint8_t reg_value = DATA_reg[reg_index];
    I_reg = mem[reg_value *4];

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

// updates state of Chip8 core
void Chip8::update() {

  sf::Event event;

  while(screen.get_window().pollEvent(event)) {

    if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {

      input.update_keys();
      
    }
    if(event.type == sf::Event::Closed) {

      screen.get_window().close();

    }
  }

  // Time instruction
  time_t before = time(NULL);
  
  run_instruction();
  
  time_t after = time(NULL);
  
  chip_timer += difftime(after, before);

  // if timer is 1/60 of a second then update the sound and timer registers
  if(chip_timer > (1.0 / 60.0)) {
    if(TIMER_reg > 0) {
      TIMER_reg--;
    }
    if(SOUND_reg > 0) {
      SOUND_reg--;
    }
    chip_timer = 0.0;
  }

    screen.show_screen();
    
}
