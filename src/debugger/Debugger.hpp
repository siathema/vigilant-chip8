#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP
#include <string>
#include <iostream>
#include "../Chip8.hpp"

class Debugger {
  
  void parse_line(Chip8 &cpu);

public:

  Debugger();
  ~Debugger();

  void step_cpu_instruction();

  void run(Chip8 &cpu);
  
};

#endif
