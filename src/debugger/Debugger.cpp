#include "Debugger.hpp"

Debugger::Debugger() {



}

Debugger::~Debugger() {}

void Debugger::step_cpu_instruction() {



}

void Debugger::run(Chip8 &cpu) {



}

void parse_line(Chip8 &cpu) {

  std::string line;
  std::getline(std::cin, line);

  if(line.compare("step")==0) {
    cpu.update(true);
  }
  else if(line.compare("exit")==0) {
    std::cout<<"leaving Emulator at: "<<std::setbase(16)<< cpu.get_pc() << std::endl;
  }

}
