#include "Debugger.hpp"

Debugger::Debugger() { last_command = NOTHING; }

Debugger::~Debugger() {}

void Debugger::run(Chip8 &cpu) {
  while (true) {
    std::cout << "Vigilant-Chip8> ";
    parse_line(cpu);
  }
}

void Debugger::parse_line(Chip8 &cpu) {
  std::string line;
  std::getline(std::cin, line);

  if (line.compare("step") == 0 || line.compare("s") == 0 ||
      (line.compare("") == 0 && last_command == STEP)) {
    cpu.update(true);
    last_command = STEP;
  } else if (line.compare("exit") == 0 || last_command == EXIT) {
    std::cout << "leaving Emulator at: " << std::setbase(16) << cpu.get_PC()
              << std::endl;
    exit(0);
  } else if (line.compare(0, 6, "print ", 6) == 0) {
    line = line.substr(6, line.size());

    if (line.compare("PC") == 0) {
      std::cout << "PC: " << std::setbase(16) << cpu.get_PC() << std::endl;

    } else if (line.compare("SP") == 0) {
      std::cout << "SP: " << std::setbase(16) << cpu.get_SP() << std::endl;

    } else if (line.compare("I") == 0) {
      std::cout << "I: " << std::setbase(16) << cpu.get_I() << std::endl;

    } else if (line.compare("timer") == 0) {
      std::cout << "TIMER: " << std::setbase(16) << cpu.get_timer()
                << std::endl;

    } else if (line.compare("sound") == 0) {
      std::cout << "SOUND: " << std::setbase(16) << cpu.get_sound()
                << std::endl;

    } else if (line.find_first_of("[") != line.npos) {
      std::string addr =
          line.substr(line.find_first_of("[") + 1, line.find_last_of("]") - 1);
      unsigned int num = std::stoul(addr, 0, 16);
      std::cout << "memory@address[" << std::setbase(16) << num
                << "]: " << (int)cpu.get_mem(num) << std::endl;

    } else if (line.find("V(") != line.npos) {
      std::string addr =
          line.substr(line.find_first_of("(") + 1, line.find_last_of(")") - 1);
      unsigned int num = std::stoul(addr, 0, 16);
      std::cout << "register V(" << std::setbase(16) << num
                << "): " << (int)cpu.get_V_regs(num) << std::endl;

    } else if (line.compare("cpu") == 0) {
      cpu.print_state();
    }
  } else if (line.compare("continue") == 0 || last_command == CONTINUE) {
    while (true) {
      cpu.update();
      last_command = CONTINUE;
    }

  } else {
    std::cout << "Vigilant-Chip8> Command not recognized.\n";
    last_command = INVALID;
  }
}