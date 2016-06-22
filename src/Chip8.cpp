#include "Chip8.hpp"
////////////////////////////////////////////////////////////////////////////////
Chip8::Chip8(char* chip8_program, int program_size)
    : screen(640, 320), input() {
  init(chip8_program, program_size);
}
////////////////////////////////////////////////////////////////////////////////
void Chip8::init(char* chip8_program, int program_size) {
  // Initialize RNG
  srand(time(NULL));
  // Initialize timer.
  timer = sf::milliseconds(0);
  // Initialize chip8 memory and registers.
  memset(mem, 0x00, MEMORY_SIZE * sizeof(char));
  memset(DATA_reg, 0x00, 16 * sizeof(uint8_t));
  I_reg = 0x0000;
  SOUND_reg = TIMER_reg = 0x00;
  SP_reg = 0x0100;
  PC_reg = 0x0200;

  // Load hex_sprites and Chip8 program into Chip8 memory.
  memcpy(mem, hex_sprites, 16 * 5 * sizeof(uint8_t));
  memcpy(mem + 0x0200, chip8_program, program_size * sizeof(char));
  // Initialize sound
  if(!buffer.loadFromFile("assets/213795__austin1234575__beep-1-sec.wav")) {
    std::cerr<< "Could not open sound file!\n";
    std::exit(-1);
  }
  sound.setBuffer(buffer);
  sound.setLoop(true);
}
////////////////////////////////////////////////////////////////////////////////
Chip8::~Chip8() {}
////////////////////////////////////////////////////////////////////////////////
void Chip8::run_instruction(uint16_t opcode, bool debug) {
  // Temp var for PC.
  uint16_t PC_dest = 0;

  if ((opcode & 0xF000) == 0x0000 && (opcode != 0x00E0) &&
      (opcode != 0x00EE)) {  // SYS addr - jump to subroutine at addr

    SP_reg++;
    mem[SP_reg] = PC_reg;
    uint16_t addr = opcode & 0x0FFF;
    PC_dest = addr;

    if (debug == true) {
      std::cout << std::setbase(16) << ": SYS " << addr << std::endl;
    }

  } else if (opcode == 0x00E0) {  // CLS -  clear the display
    screen.clear_screen();

    if (debug == true) {
      std::cout << ": CLS" << std::endl;
    }

  } else if (opcode == 0x00EE) {  // RET - Return from subroutine
    // zero PC_reg
    PC_reg = 0x0000;
    // load address off the stack.
    PC_reg |= (mem[SP_reg]) << 8;
    SP_reg--;
    PC_reg |= (mem[SP_reg]);
    SP_reg--;

    if (debug == true) {
      std::cout << ": RET" << std::endl;
    }
  } else if ((opcode & 0xF000) == 0x1000) {  // JP addr - Jump to lower 14-bit

    uint16_t addr = opcode & 0x0FFF;
    PC_dest = addr;

    if (debug == true) {
      std::cout << std::setbase(16) << ": JP " << addr << std::endl;
    }
  } else if ((opcode & 0xF000) == 0x2000) {  // CALL addr - Call subroutine
    // Load the address in PC_reg onto the stack.
    SP_reg++;
    mem[SP_reg] = PC_reg;
    SP_reg++;
    mem[SP_reg] = PC_reg >> 8;
    uint16_t addr = opcode & 0x0FFF;
    PC_dest = addr;

    if (debug == true) {
      std::cout << std::setbase(16) << ": CALL " << addr << std::endl;
    }

  } else if ((opcode & 0xF000) == 0x3000) {  // SE Vx, byte - Skip net if Vx=b
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content = DATA_reg[reg_index];
    uint8_t op_byte = (opcode & 0x00FF);
    if (reg_content == op_byte) {
      PC_reg += 2;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": SE " << reg_index << ", "
                << (int)op_byte << std::endl;
    }
  } else if ((opcode & 0xF000) ==
             0x4000) {  // SNE Vx, byte - Skip next if Vx!=b
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content = DATA_reg[reg_index];
    uint8_t op_byte = (opcode & 0x00FF);
    if (reg_content != op_byte) {
      PC_reg += 2;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": SNE " << reg_index << ", "
                << (int)op_byte << std::endl;
    }

  } else if ((opcode & 0xF000) == 0x5000) {  // SE Vx, Vy - Skip next if Vx=Vy
    // find register index in opcode.
    uint16_t reg_index_x = (opcode & 0x0F00) >> 8;
    uint8_t reg_content_x = DATA_reg[reg_index_x];
    uint16_t reg_index_y = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index_y];

    if (reg_content_x == reg_content_y) {
      PC_reg += 2;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": SE " << reg_index_x << ", "
                << reg_index_y << std::endl;
    }

  } else if ((opcode & 0xF000) == 0x6000) {  // LD Vx, byte - Set Vx = byte
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    DATA_reg[reg_index] = byte;

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD " << reg_index << ", " << (int)byte
                << std::endl;
    }

  } else if ((opcode & 0xF000) == 0x7000) {  // ADD Vx, byte - Vx = Vx+byte
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    DATA_reg[reg_index] = DATA_reg[reg_index] + byte;

    if (debug == true) {
      std::cout << std::setbase(16) << ": ADD " << reg_index << ", "
                << (int)byte << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x8000) {  // LD Vx, Vy - Set Vx=Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    DATA_reg[reg_index] = reg_content_y;

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }

  } else if ((opcode & 0xF00F) == 0x8001) {  // OR Vx, Vy - Set Vx= Vx OR Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    DATA_reg[reg_index] = DATA_reg[reg_index] | reg_content_y;

    if (debug == true) {
      std::cout << std::setbase(16) << ": OR " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x8002) {  // AND Vx, Vy - Set Vx= Vx AND Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    DATA_reg[reg_index] = DATA_reg[reg_index] & reg_content_y;

    if (debug == true) {
      std::cout << std::setbase(16) << ": AND " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x8003) {  // XOR Vx, Vy - Set Vx= Vx XOR Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    DATA_reg[reg_index] = DATA_reg[reg_index] ^ reg_content_y;

    if (debug == true) {
      std::cout << std::setbase(16) << ": XOR " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }

  } else if ((opcode & 0xF00F) == 0x8004) {  // ADD Vx, Vy - Set Vx= Vx + Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    uint16_t sum = DATA_reg[reg_index] + reg_content_y;
    if (sum > 255) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }
    DATA_reg[reg_index] = sum;

    if (debug == true) {
      std::cout << std::setbase(16) << ": ADD " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x8005) {  // SUB Vx, Vy - Set Vx= Vx - Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_x > reg_content_y) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_x - reg_content_y;

    if (debug == true) {
      std::cout << std::setbase(16) << ": SUB " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x8006) {  // SHR Vx, Vy - Vx>>1
    // find register index in opcode.
    uint8_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_x & 0x01) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_x >> 1;

    if (debug == true) {
      std::cout << std::setbase(16) << ": SHR " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x8007) {  // SUBN Vx, Vy - Vx = Vx - Vy
    // find register index in opcode.
    uint8_t reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];
    reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_y > reg_content_x) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_y - reg_content_x;

    if (debug == true) {
      std::cout << std::setbase(16) << ": SUBN " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF00F) == 0x800E) {  // SHR Vx, Vy - Vx = Vx<<1
    // find register index in opcode.
    uint8_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    if (reg_content_x & 0x01) {
      DATA_reg[15] = 1;
    } else {
      DATA_reg[15] = 0;
    }

    DATA_reg[reg_index] = reg_content_x << 1;

    if (debug == true) {
      std::cout << std::setbase(16) << ": SHR " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF000) == 0x9000) {  // SNE Vx, Vy - Skip next if Vx!=Vy
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_content_x = DATA_reg[reg_index];
    reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_content_y = DATA_reg[reg_index];

    if (reg_content_x != reg_content_y) {
      PC_reg += 2;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": SNE " << reg_index << ", "
                << ((opcode & 0x00F0) >> 4) << std::endl;
    }
  } else if ((opcode & 0xF000) == 0xA000) {  // LD I, addr - I = addr

    I_reg = opcode & 0x0FFF;

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD "
                << "I, " << (opcode & 0x0FFF) << std::endl;
    }
  } else if ((opcode & 0xF000) == 0xB000) {  // JP V0, addr

    uint16_t addr = opcode & 0x0FFF;
    PC_dest = addr + DATA_reg[0];

    if (debug == true) {
      std::cout << std::setbase(16) << ": JP "
                << "V0, " << addr << std::endl;
    }
  } else if ((opcode & 0xF000) ==
             0xC000) {  // RND Vx, byte - set Vx=random AND b
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    DATA_reg[reg_index] = (rand() % 256) & (opcode & 0x00FF);

    if (debug == true) {
      std::cout << std::setbase(16) << ": RND " << reg_index << ", "
                << (opcode & 0x00FF) << std::endl;
    }
  } else if ((opcode & 0xF000) == 0xD000) {  // DRW Vx, Vy, nibble
    bool is_collision = false;
    // find register index in opcode.
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_value_x = DATA_reg[reg_index];
    reg_index = (opcode & 0x00F0) >> 4;
    uint8_t reg_value_y = DATA_reg[reg_index];
    // find sprite index in opcode
    uint8_t sprite_index = (opcode & 0x000F);
    // temp index in case sprite is drawn off the screen in the y direction.
    int rollover_index = 0;
    // loop through each sprite byte and draw.
    for (int i = 0; i < sprite_index; i++) {
      // If the y index is off the screen draw it at the top of the screen.
      if (reg_value_y + i >= SCREEN_HEIGHT) {
        if (screen.draw_byte(mem[I_reg + i], reg_value_x, rollover_index)) {
          is_collision = true;
        }
        rollover_index++;
      } else if (screen.draw_byte(mem[I_reg + i], reg_value_x,
                                  reg_value_y + i)) {
        is_collision = true;
      }
    }
    // If the sprite collides with another sprite Vf is set to 1.
    if (is_collision) {
      DATA_reg[15] = 0x01;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": DRW " << ((opcode & 0x0F00) >> 8)
                << ", " << reg_index << ", " << (int)sprite_index << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xE09E) {  // SKP Vx - skip if key-input = vX

    // Get value stored in Vx
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    if (input.is_key_pressed(DATA_reg[reg_index])) {
      PC_reg += 2;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": SKP " << ((opcode & 0x0F00) >> 8)
                << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xE0A1) {  // SKNP Vx - skip if key-input !=
                                             // vX

    // Get value stored in Vx
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    if (!input.is_key_pressed(DATA_reg[reg_index])) {
      PC_reg += 2;
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": SKNP " << ((opcode & 0x0F00) >> 8)
                << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xF007) {  // LD Vx, DT - Set vx = delay timer

    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    DATA_reg[reg_index] = TIMER_reg;

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD " << reg_index << ", DT"
                << std::endl;
    }
  } else if ((opcode & 0xF0FF) ==
             0xF00A) {  // LD Vx, K - wait for key, set in Vx
    bool key_is_not_pressed = true;
    // Get value stored in Vx
    uint16_t reg_index = (opcode & 0x0F00) >> 8;

    while ( key_is_not_pressed ) {
      input.update_keys();
      for( uint8_t i=0; i<16; i++ ) {
        if(input.is_key_pressed(i)) {
          DATA_reg[reg_index] = i;
          key_is_not_pressed = false;
        }
      }
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD " << reg_index << ", K"
                << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xF015) {  // LD DT, Vx - set delay timer = Vx

    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    TIMER_reg = DATA_reg[reg_index];

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD "
                << "DT, " << reg_index << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xF018) {  // LD ST, VX - set sound timer = Vx

    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    SOUND_reg = DATA_reg[reg_index];

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD ST, " << reg_index << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xF01E) {  // ADD I, Vx - I = I + Vx

    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    I_reg += DATA_reg[reg_index];

    if (debug == true) {
      std::cout << std::setbase(16) << ": ADD I, " << reg_index << std::endl;
    }
  } else if ((opcode & 0xF0FF) ==
             0xF029) {  // LD F, Vx - set I = address of hex-sprite of Vx

    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_value = DATA_reg[reg_index];
    I_reg = reg_value * 5;

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD F, " << reg_index << std::endl;
    }
  } else if ((opcode & 0xF0FF) == 0xF033) {  // LD B, Vx store in I, I+1, and
                                             // I+2 the BCD representation of Vx

    // Get value stored in Vx
    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    uint8_t reg_value = DATA_reg[reg_index];

    // Calculate BCD value of Vx
    uint8_t value_hundreds = reg_value / 100;
    reg_value %= 100;
    uint8_t value_tens = reg_value / 10;

    // Store most significant byte in mem[I] and the least significant in
    // mem[I+2]
    mem[I_reg] = value_hundreds;
    mem[I_reg + 1] = value_tens;
    mem[I_reg + 2] = reg_value % 10;

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD B, " << reg_index << std::endl;
      }
  } else if ((opcode & 0xF0FF) == 0xF055) {  // LD Vx, [I] - store the memory
                                              // starting at I in regs[V0..Vx]

     uint16_t reg_index = (opcode & 0x0F00) >> 8;
     for (int i = 0; i <= reg_index; i++) {
       mem[I_reg + i] = DATA_reg[i];
     }

      if (debug == true) {
       std::cout << std::setbase(16) << ": LD [I], " << reg_index << std::endl;
      }
  } else if ((opcode & 0xF0FF) == 0xF065) {  // LD [I], Vx - store [V0..Vx] in
                                             // memory starting at mem[I]

    uint16_t reg_index = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= reg_index; i++) {
      DATA_reg[i] = mem[I_reg + i];
    }

    if (debug == true) {
      std::cout << std::setbase(16) << ": LD " << reg_index << ", [I]" << std::endl;
    }
  }

  // Setting up next instruction
  if (PC_dest != 0) {
    PC_reg = PC_dest;
  } else {
    PC_reg += 2;
  }
  return;
}
////////////////////////////////////////////////////////////////////////////////
// updates state of Chip8 core
void Chip8::update(bool debug) {
  sf::Event event;
  // Input checking.
  while (screen.get_window().pollEvent(event)) {
    if (event.type == sf::Event::KeyPressed ||
        event.type == sf::Event::KeyReleased) {
      input.update_keys();
    }
    if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      screen.get_window().close();
      exit(0);
    }
  }
  // Get the opcode from chip8 memory.
  uint16_t opcode = 0x00;
  opcode = mem[PC_reg];
  opcode = opcode << 8 | mem[PC_reg + 1];

  // debugger info
  if (debug) {
    std::cout << "Insruction@Address->0x" << std::setbase(16) << PC_reg << ": ";
  }

  // Initialize clock.
  sf::Clock clock;
  // Start timing cycle
  sf::Time begin = clock.getElapsedTime();

  run_instruction(opcode, debug);

  sf::Time end = clock.getElapsedTime();

  sf::Time elapsed = ( end - begin );
  timer = timer + elapsed;


  // if timer is 1/60 of a second then update the sound and timer registers
  // TODO: actually update at a 60hz.
  if (timer.asMicroseconds() > 80) {
    if (TIMER_reg > 0) {
      TIMER_reg--;
    }
    if (SOUND_reg > 0) {
      SOUND_reg--;
      if ( !is_playing ) {
        sound.play();
        is_playing = true;
      }
    } else {
      sound.pause();
      is_playing = false;
    }
    timer = sf::Time::Zero;
  }
  // Draw.
  screen.show_screen();
  }
////////////////////////////////////////////////////////////////////////////////
uint16_t Chip8::get_PC() { return PC_reg; }
////////////////////////////////////////////////////////////////////////////////
uint8_t Chip8::get_mem(uint16_t address) { return mem[address]; }
////////////////////////////////////////////////////////////////////////////////
uint8_t Chip8::get_V_regs(int index) { return DATA_reg[index]; }
////////////////////////////////////////////////////////////////////////////////
uint16_t Chip8::get_I() { return I_reg; }
////////////////////////////////////////////////////////////////////////////////
uint16_t Chip8::get_SP() { return SP_reg; }
////////////////////////////////////////////////////////////////////////////////
uint8_t Chip8::get_timer() { return TIMER_reg; }
////////////////////////////////////////////////////////////////////////////////
uint8_t Chip8::get_sound() { return SOUND_reg; }
////////////////////////////////////////////////////////////////////////////////
void Chip8::print_state() {
  std::cout << "PC: " << std::setbase(16) << PC_reg << std::endl;
  std::cout << "SP: " << std::setbase(16) << SP_reg << std::endl;
  std::cout << "I: " << std::setbase(16) << I_reg << std::endl;
  std::cout << "TIMER: " << std::setbase(16) << (int)TIMER_reg << std::endl;
  std::cout << "SOUND: " << std::setbase(16) << (int)SOUND_reg << std::endl;

  for (int i = 0; i < 16; i++) {
    std::cout << "V" << i << ": " << std::setbase(16) << (int)DATA_reg[i]
              << std::endl;
  }
}
////////////////////////////////////////////////////////////////////////////////
