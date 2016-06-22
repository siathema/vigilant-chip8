#ifndef CHIP8_H
#define CHIP8_H
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Screen.hpp"
#include "Chip8_Input.hpp"

const int MEMORY_SIZE = 4096;

// hex sprites
const uint8_t hex_sprites[16 * 5] = {0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
                                     0x20, 0x60, 0x20, 0x20, 0x70,   // 1
                                     0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
                                     0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
                                     0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
                                     0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
                                     0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
                                     0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
                                     0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
                                     0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
                                     0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
                                     0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
                                     0xF0, 0x80, 0x80, 0x80, 0x90,   // C
                                     0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
                                     0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
                                     0xF0, 0x80, 0xF0, 0x80, 0x80};  // F

//----------------------------------------------------------------------------//
// Chip8:
// Emulates the chip8 core.
//----------------------------------------------------------------------------//
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
  sf::Time timer;

  // Sound
  sf::SoundBuffer buffer;
  sf::Sound sound;
  bool is_playing = false;

  // Screen
  Screen screen;

  // Input
  Chip8_Input input;

 public:
   //-------------------------------------------------------------------------//
   // Chip8 ( char* chip8_program, int program_size)
   // Constructor for the chip8 core. chip8_program is a pointer to a
   // buffer containing the chip8 program to be run. program_size is the
   // size in bytes of the chip8 program.
   //-------------------------------------------------------------------------//
  Chip8(char* chip8_program, int program_size);
  //--------------------------------------------------------------------------//
  // ~chip8 ()
  // Chip8 core destructor. does nothing.
  //--------------------------------------------------------------------------//
  ~Chip8();
  //--------------------------------------------------------------------------//
  // void init ( char* chip8_program, int program_size )
  // initializes the chip8 core.chip8_program is a pointer to a
  // buffer containing the chip8 program to be run. program_size is the
  // size in bytes of the chip8 program.
  //--------------------------------------------------------------------------//
  void init(char* chip8_program, int program_size);
  //--------------------------------------------------------------------------//
  // void run_instruction ( uint16_t opcode, bool debug = false )
  // Takes a chip8 intruction and executes it. opcode is a 16 chip8 instruction.
  // If the debug flag is set to true, run_instruction will print the chip8
  // instruction to the standard output.
  //--------------------------------------------------------------------------//
  void run_instruction(uint16_t opcode, bool debug = false);
  //--------------------------------------------------------------------------//
  // void update ( bool debug = false )
  // Emulates chip8 instruction cycle. Handles Keyboard input, timing,
  // and sound. If the debug flag is set to true, run_instruction will print
  // the chip8 instruction to the standard output.
  //--------------------------------------------------------------------------//
  void update(bool debug = false);
  //--------------------------------------------------------------------------//
  // uint16_t get_PC ()
  // getter for PC_reg.
  //--------------------------------------------------------------------------//
  uint16_t get_PC();
  //--------------------------------------------------------------------------//
  // uint8_t get_mem ( uint16_t address )
  // Returns contents of mem at address.
  //--------------------------------------------------------------------------//
  uint8_t get_mem(uint16_t address);
  //--------------------------------------------------------------------------//
  // uint8_t get_V_regs ( int index )
  // Returns the contents of DATA_reg at index.
  //--------------------------------------------------------------------------//
  uint8_t get_V_regs(int index);
  //--------------------------------------------------------------------------//
  // uint8_t get_I ()
  // Returns the contents of I_reg.
  //--------------------------------------------------------------------------//
  uint16_t get_I();
  //--------------------------------------------------------------------------//
  // uint16_t get_SP ()
  // returns the contents of SP_reg.
  //--------------------------------------------------------------------------//
  uint16_t get_SP();
  //--------------------------------------------------------------------------//
  // uint8_t get_timer ()
  // returns the contents of TIMER_reg.
  //--------------------------------------------------------------------------//
  uint8_t get_timer();
  //--------------------------------------------------------------------------//
  // uint8_t get_sound ()
  // returns the contents of SOUND_reg.
  //--------------------------------------------------------------------------//
  uint8_t get_sound();
  //--------------------------------------------------------------------------//
  // void print_state ()
  // Prints the state of the chip8 core to standard output.
  //--------------------------------------------------------------------------//
  void print_state();
};

#endif
