#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP
#include <string>
#include <iostream>
#include "../Chip8.hpp"

class Debugger {

  enum Command {
    STEP,
    EXIT,
    CONTINUE,
    NOTHING,
    INVALID
  };

  Command last_command;
  
  void parse_line(Chip8 &cpu);

public:

  Debugger();
  ~Debugger();


  void run(Chip8 &cpu);
  
};

#endif
